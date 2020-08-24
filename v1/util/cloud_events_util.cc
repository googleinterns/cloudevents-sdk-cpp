#include "cloud_events_util.h"

#include <google/protobuf/util/time_util.h>

namespace cloudevents {
namespace cloudevents_util {

using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;
using ::google::protobuf::Timestamp;
using ::google::protobuf::util::TimeUtil;

constexpr char kErrCeInvalid[] = "Given Cloud Event is missing required attributes.";
constexpr char kErrTimeInvalid[] = "Given time is invalid because it does not comply to RFC 3339.";
constexpr char kErrAttrNotSet[] = "Given Cloud Event attribute is not set.";
constexpr char kErrAttrNotHandled[] = "A Cloud Event type is not handled.";

absl::Status CloudEventsUtil::IsValid(const CloudEvent& cloud_event) {
  if (cloud_event.id().empty() ||
      cloud_event.source().empty() ||
      cloud_event.spec_version().empty() ||
      cloud_event.type().empty()) {
    return absl::InvalidArgumentError(kErrCeInvalid);
  }
  return absl::OkStatus();
}

cloudevents_absl::StatusOr<
    absl::flat_hash_map<std::string, CloudEvent_CloudEventAttribute>>
    CloudEventsUtil::GetMetadata(const CloudEvent& cloud_event) {
  if (auto is_valid = CloudEventsUtil::IsValid(cloud_event); !is_valid.ok()) {
    return is_valid;
  }

  // create absl::flat_hash_map from protobuf map of optional/ extensionattrs
  absl::flat_hash_map<std::string, CloudEvent_CloudEventAttribute> attrs(
    (cloud_event.attributes()).begin(),
    cloud_event.attributes().end());

  // insert required attrs
  attrs["id"] = ToCeString(cloud_event.id());
  attrs["source"] = ToCeString(cloud_event.source());
  attrs["spec_version"] = ToCeString(cloud_event.spec_version());
  attrs["type"] = ToCeString(cloud_event.type());

  return attrs;
}

absl::Status CloudEventsUtil::SetMetadata(const std::string& key,
    const std::string& val, CloudEvent& cloud_event) {
  // TODO (#39): Recognize URI and URI Reference types
  if (key == "id") {
    cloud_event.set_id(val);
  } else if (key == "source") {
    cloud_event.set_source(val);
  } else if (key == "spec_version") {
    cloud_event.set_spec_version(val);
  } else if (key == "type") {
    cloud_event.set_type(val);
  } else if (key == "time") {
    CloudEvent_CloudEventAttribute attr;
    Timestamp timestamp;
    if (!TimeUtil::FromString(val, &timestamp)) {
      return absl::InvalidArgumentError(kErrTimeInvalid);
    }
    *attr.mutable_ce_timestamp() = timestamp;
    (*cloud_event.mutable_attributes())[key] = attr;
  } else {
    // default assumes unrecognized attributes to be of type string
    CloudEvent_CloudEventAttribute attr;
    attr.set_ce_string(val);
    (*cloud_event.mutable_attributes())[key] = attr;
  }
  return absl::OkStatus();
}

absl::Status CloudEventsUtil::SetContentType(const std::string& val,
    CloudEvent& cloud_event) {
  return SetMetadata("datacontenttype", val, cloud_event);
}

cloudevents_absl::StatusOr<std::string> CloudEventsUtil::ToString(
    const CloudEvent_CloudEventAttribute& attr){
  switch (attr.attr_oneof_case()) {
    case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeBoolean:
      // StatusOr requires explicit conversion
      return std::string(attr.ce_boolean() ? "true" : "false");
    case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeInteger:
      // skipping validity checks as protobuf generates int32 for sfixed32
      return std::to_string(attr.ce_integer());
    case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeString:
      return attr.ce_string();
    case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeBinary:
      return cloudevents_base64::base64_encode(attr.ce_binary());
    case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeUri:
      return attr.ce_uri();
    case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeUriReference:
      return attr.ce_uri_reference();
    case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeTimestamp:
      // protobuf also uses RFC3339 representation
      return TimeUtil::ToString(attr.ce_timestamp());
    case CloudEvent_CloudEventAttribute::AttrOneofCase::ATTR_ONEOF_NOT_SET:
      return absl::InvalidArgumentError(kErrAttrNotSet);
  }
  return absl::InternalError(kErrAttrNotHandled);
}

CloudEvent_CloudEventAttribute CloudEventsUtil::ToCeString(
    const std::string& val) {
  CloudEvent_CloudEventAttribute ce_str;
  ce_str.set_ce_string(val);
  return ce_str;
}

}  // namespace cloudevents_util
}  // namespace cloudevents
