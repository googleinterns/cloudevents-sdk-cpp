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

absl::StatusOr<std::unique_ptr<StructuredCloudEvent>> JsonFormatter::Serialize(
    const CloudEvent& cloud_event) {
  absl::Status is_valid = CloudEventsUtil::IsValid(cloud_event);
  if (!is_valid.ok()) {
    return is_valid;
  }

  absl::StatusOr<CeAttrMap> attrs = CloudEventsUtil::GetMetadata(
    cloud_event);
  if (!attrs.ok()) {
    return attrs.status();
  }

  Json::Value root;
  for (auto const& attr : (*attrs)) {
    absl::StatusOr<Json::Value> json_printed = PrintToJson(attr.second);
    if (!json_printed.ok()) {
      return json_printed.status();
    }
    root[attr.first] = (*json_printed);
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

}  // namespace format
}  // namespace cloudevents
