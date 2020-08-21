#include "json_formatter.h"

#include <google/protobuf/any.pb.h>
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/time_util.h>

#include "v1/util/cloud_events_util.h"

namespace cloudevents {
namespace format {

using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;
using ::google::protobuf::util::TimeUtil;
using ::cloudevents::cloudevents_util::CloudEventsUtil;

typedef absl::flat_hash_map<std::string, CloudEvent_CloudEventAttribute>
  CeAttrMap;

// Attribute keys
constexpr char kJsonBinaryKey[] = "data_base64";
constexpr char kJsonTextKey[] = "data";

// Error statuses
constexpr char kErrProtobufAny[] = "protobuf::Any not supported yet.";
constexpr char kErrNotJson[] = "The given serialized data should not be handled by JsonFormatter because it is not JSON-formatted.";
constexpr char kErrTwoPayloads[] = "The given serialized data is invalid because it contains two data payloads.";

cloudevents_absl::StatusOr<
    std::unique_ptr<StructuredCloudEvent>> JsonFormatter::Serialize(
    const CloudEvent& cloud_event) {
  if (auto is_valid = CloudEventsUtil::IsValid(cloud_event); !is_valid.ok()) {
    return is_valid;
  }

  cloudevents_absl::StatusOr<CeAttrMap> attrs = CloudEventsUtil::GetMetadata(
    cloud_event);
  if (!attrs.ok()) {
    return attrs.status();
  }

  Json::Value root;
  for (auto const& attr : *attrs) {
    cloudevents_absl::StatusOr<Json::Value> json_printed =
      PrintToJson(attr.second);
    if (!json_printed.ok()) {
      return json_printed.status();
    }
    root[attr.first] = *json_printed;
  }

  switch (cloud_event.data_oneof_case()) {
    case CloudEvent::DataOneofCase::kBinaryData:
      root[kJsonBinaryKey] = cloud_event.binary_data();
      break;
    case CloudEvent::DataOneofCase::kTextData:
      root[kJsonTextKey] = cloud_event.text_data();
      break;
    case CloudEvent::DataOneofCase::kProtoData:
      // TODO (#17): Handle CloudEvent Any in JsonFormatter
      return absl::UnimplementedError(kErrProtobufAny);
    case CloudEvent::DATA_ONEOF_NOT_SET:
      break;
  }

  // Write JSON serialized data as std::string
  Json::StreamWriterBuilder builder;
  auto structured_ce = absl::make_unique<StructuredCloudEvent>();
  (*structured_ce).format = Format::kJson;
  (*structured_ce).serialized_data = Json::writeString(builder, root);

  return structured_ce;
}

cloudevents_absl::StatusOr<CloudEvent> JsonFormatter::Deserialize(
    const StructuredCloudEvent& structured_ce) {
  // Validate that this is the right format to be handled by this object
  if (structured_ce.format != Format::kJson) {
    return absl::InternalError(kErrNotJson);
  }

  // Create a Json::Value from the serialized data string
  std::string serialized_data = structured_ce.serialized_data;
  Json::CharReaderBuilder builder;
  Json::CharReader* reader = builder.newCharReader();
  std::string errors;
  Json::Value root;

  bool parsingSuccessful = reader->parse(
    serialized_data.c_str(),
    serialized_data.c_str() + serialized_data.size(),
    &root,
    &errors);
  if (!parsingSuccessful) {
    return absl::InvalidArgumentError(errors);
  }

  CloudEvent cloud_event;

  // TODO (#39): Should we try to infer CE Type from serialized_data?
  for (auto const& member : root.getMemberNames()) {
    if (auto set_metadata = CloudEventsUtil::SetMetadata(member,
      root[member].asString(), cloud_event); !set_metadata.ok()) {
      return set_metadata;
    }
  }

  if (root.isMember(kJsonTextKey) && root.isMember(kJsonBinaryKey)) {
    return absl::InvalidArgumentError(kErrTwoPayloads);
  } else if (root.isMember(kJsonTextKey)) {
    cloud_event.set_text_data(root[kJsonTextKey].asString());
  } else if (root.isMember(kJsonBinaryKey)) {
    cloud_event.set_binary_data(root[kJsonBinaryKey].asString());
  }

  if (auto is_valid = CloudEventsUtil::IsValid(cloud_event); !is_valid.ok()) {
    return is_valid;
  }

  return cloud_event;
}

cloudevents_absl::StatusOr<Json::Value> JsonFormatter::PrintToJson(
    const CloudEvent_CloudEventAttribute& attr){
  switch (attr.attr_oneof_case()) {
    case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeBoolean:
      return Json::Value(attr.ce_boolean());
    case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeInteger:
      return Json::Value(attr.ce_integer());
    case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeString:
      return Json::Value(attr.ce_string());
    case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeBinary:
      return Json::Value(attr.ce_binary());
    case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeUri:
      return Json::Value(attr.ce_uri());
    case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeUriReference:
      return Json::Value(attr.ce_uri_reference());
    case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeTimestamp:
      return Json::Value(TimeUtil::ToString(attr.ce_timestamp()));
    case CloudEvent_CloudEventAttribute::AttrOneofCase::ATTR_ONEOF_NOT_SET:
      return absl::InvalidArgumentError(
        "Cloud Event metadata attribute not set.");
  }
  return absl::InternalError("A Cloud Event attribute was not handled.");
}

}  // namespace format
}  // namespace cloudevents
