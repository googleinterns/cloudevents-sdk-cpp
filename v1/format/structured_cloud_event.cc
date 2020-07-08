#include "structured_cloud_event.h"

namespace cloud_events {
namespace format {
using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;

// StructuredCloudEvent

StructuredCloudEvent::StructuredCloudEvent(CloudEventFormat format, std::string serialized_cloud_event)
    : format_(format), serialized_cloud_event_(serialized_cloud_event)
{}


CloudEventFormat StructuredCloudEvent::GetCloudEventFormat(){
    return format_;
}

std::string StructuredCloudEvent::GetSerializedCloudEvent() {
    return serialized_cloud_event_;
} 

// JsonMarshaller
absl::StatusOr<Json::Value> JsonMarshaller::PrintToJson(CloudEvent_CloudEventAttribute attr){
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
            return Json::Value(google::protobuf::util::TimeUtil::ToString(attr.ce_timestamp()));
        case CloudEvent_CloudEventAttribute::AttrOneofCase::ATTR_ONEOF_NOT_SET:
            return absl::InvalidArgumentError("Cloud Event metadata attribute not set.");
    }
    return absl::InvalidArgumentError("Cloud Event metadata attribute not recognized"); // should not be reached as all enums covered
}

} // format
} // cloud_events
