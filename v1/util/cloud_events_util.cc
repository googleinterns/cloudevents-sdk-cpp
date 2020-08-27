#include "cloud_events_util.h"

#include <google/protobuf/util/time_util.h>

#include <regex>

namespace cloudevents {
namespace cloudevents_util {

using ::io::cloudevents::v1::CloudEvent;
using ::google::protobuf::Timestamp;
using ::google::protobuf::util::TimeUtil;

// Taken from https://tools.ietf.org/html/rfc3986#appendix-B
// Adapted to only accept absolute URIs
constexpr char kAbsUriRegex[] = R"(^(([^:/?#]+):)(//([^/?#]*))?([^?#]*)(\?([^#]*))?)";

constexpr char kErrCeInvalid[] = "Given Cloud Event is missing required attributes.";
constexpr char kErrTimeInvalid[] = "Given time is invalid because it does not comply to RFC 3339.";
constexpr char kErrAttrNotSet[] = "Given Cloud Event attribute is not set.";
constexpr char kErrAttrNotHandled[] = "A Cloud Event type is not handled.";
constexpr char kErrInvalidDataschema[] = "The given value for Dataschema is not a valid absolute URI.";

typedef ::io::cloudevents::v1::CloudEvent_CloudEventAttribute CeAttr;

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
    absl::flat_hash_map<std::string, CeAttr>>
    CloudEventsUtil::GetMetadata(const CloudEvent& cloud_event) {
  if (auto is_valid = CloudEventsUtil::IsValid(cloud_event); !is_valid.ok()) {
    return is_valid;
  }

  // create absl::flat_hash_map from protobuf map of optional/ extensionattrs
  absl::flat_hash_map<std::string, CeAttr> attrs(
    (cloud_event.attributes()).begin(),
    cloud_event.attributes().end());

  // insert required attrs
  // CE Spec defines attribute as "specversion" while
  // proprietary proto defines it as "spec_version"
  // https://github.com/cloudevents/spec/blob/master/spec.md#specversion
  attrs["id"] = ToCeString(cloud_event.id());
  attrs["source"] = ToCeString(cloud_event.source());
  attrs["specversion"] = ToCeString(cloud_event.spec_version());
  attrs["type"] = ToCeString(cloud_event.type());

  return attrs;
}

absl::Status CloudEventsUtil::SetMetadata(const std::string& key,
    const std::string& val, CloudEvent& cloud_event) {
  // CE Spec defines attribute as "specversion" while
  // proprietary proto defines it as "spec_version"
  // https://github.com/cloudevents/spec/blob/master/spec.md#specversion
  if (key == "id") {  // required attributes
    cloud_event.set_id(val);
  } else if (key == "source") {
    cloud_event.set_source(val);
  } else if (key == "specversion") {
    cloud_event.set_spec_version(val);
  } else if (key == "type") {
    cloud_event.set_type(val);
  } else if (key == "dataschema") {  // optional attributes
    // TODO (#60): Use external library to validate URIs.
    std::regex uri_regex (kAbsUriRegex, std::regex::extended);

    if (!std::regex_match(val, uri_regex)) {
      return absl::InvalidArgumentError(kErrInvalidDataschema);
    }
    CeAttr abs_uri;
    abs_uri.set_ce_uri(val);
    (*cloud_event.mutable_attributes())[key] = abs_uri;
  } else if (key == "time") {
    CeAttr time_str;
    Timestamp timestamp;
    if (!TimeUtil::FromString(val, &timestamp)) {
      return absl::InvalidArgumentError(kErrTimeInvalid);
    }
    *time_str.mutable_ce_timestamp() = timestamp;
    (*cloud_event.mutable_attributes())[key] = time_str;
  } else {  // default assumes type ce string
    (*cloud_event.mutable_attributes())[key] = ToCeString(val);
  }
  return absl::OkStatus();
}

absl::Status CloudEventsUtil::SetContentType(const std::string& val,
    CloudEvent& cloud_event) {
  return SetMetadata("datacontenttype", val, cloud_event);
}

cloudevents_absl::StatusOr<std::string> CloudEventsUtil::ToString(
    const CeAttr& attr){
  switch (attr.attr_oneof_case()) {
    case CeAttr::AttrOneofCase::kCeBoolean:
      // StatusOr requires explicit conversion
      return std::string(attr.ce_boolean() ? "true" : "false");
    case CeAttr::AttrOneofCase::kCeInteger:
      // skipping validity checks as protobuf generates int32 for sfixed32
      return std::to_string(attr.ce_integer());
    case CeAttr::AttrOneofCase::kCeString:
      return attr.ce_string();
    case CeAttr::AttrOneofCase::kCeBinary:
      return cloudevents_base64::base64_encode(attr.ce_binary());
    case CeAttr::AttrOneofCase::kCeUri:
      return attr.ce_uri();
    case CeAttr::AttrOneofCase::kCeUriReference:
      return attr.ce_uri_reference();
    case CeAttr::AttrOneofCase::kCeTimestamp:
      // protobuf also uses RFC3339 representation
      return TimeUtil::ToString(attr.ce_timestamp());
    case CeAttr::AttrOneofCase::ATTR_ONEOF_NOT_SET:
      return absl::InvalidArgumentError(kErrAttrNotSet);
  }
  return absl::InternalError(kErrAttrNotHandled);
}

CeAttr CloudEventsUtil::ToCeString(
    const std::string& val) {
  CeAttr ce_str;
  ce_str.set_ce_string(val);
  return ce_str;
}

}  // namespace cloudevents_util
}  // namespace cloudevents
