#include "cloud_events_util.h"

#include <google/protobuf/util/time_util.h>

namespace cloudevents {
namespace util {

using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;

bool CloudEventsUtil::IsValid(CloudEvent cloud_event) {
    return !(cloud_event.id().empty() || 
        cloud_event.source().empty() || 
        cloud_event.spec_version().empty() ||
        cloud_event.type().empty());
}

absl::StatusOr<std::map<std::string, CloudEvent_CloudEventAttribute>> CloudEventsUtil::GetMetadata(
    CloudEvent cloud_event) {
    if (!CloudEventsUtil::IsValid(cloud_event)) {
        return absl::InvalidArgumentError("GetMetadata can only be called with valid Cloud Event.");
    }

    // create std::map from protobuf map of optional/ extensionattrs
    std::map<std::string, CloudEvent_CloudEventAttribute> attrs(
        (cloud_event.attributes()).begin(),
        cloud_event.attributes().end()
    );

    // insert required attrs
    CloudEvent_CloudEventAttribute attr_val;
    attr_val.set_ce_string(cloud_event.id());
    attrs["id"] = attr_val;
    attr_val.set_ce_string(cloud_event.source());
    attrs["source"] = attr_val;
    attr_val.set_ce_string(cloud_event.spec_version());
    attrs["spec_version"] = attr_val;
    attr_val.set_ce_string(cloud_event.type());
    attrs["type"] = attr_val;

    return attrs;
}

absl::StatusOr<std::string> CloudEventsUtil::StringifyCeType(CloudEvent_CloudEventAttribute attr){
    switch (attr.attr_oneof_case()) {
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeBoolean:
            return std::string(attr.ce_boolean() ? "true" : "false");  // StatusOr requires explicit conversion
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeInteger:
            return std::to_string(attr.ce_integer());  // skipping validity checks as protobuf generates int32 for sfixed32
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeString:
            return attr.ce_string();
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeBinary:
            return base64::base64_encode(attr.ce_binary());
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeUri:
            return attr.ce_uri();
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeUriReference:
            return attr.ce_uri_reference();
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeTimestamp:
            return google::protobuf::util::TimeUtil::ToString(attr.ce_timestamp());  // take advantage of protobuf using RFC3339 representation
        case CloudEvent_CloudEventAttribute::AttrOneofCase::ATTR_ONEOF_NOT_SET:
            return absl::InvalidArgumentError("Cloud Event metadata attribute not set.");
    }
}

} // util
} // cloudevents
