#include "format.h"

namespace structured_cloud_events {
using ::io::cloudevents::v1::CloudEvent;

// StructuredCloudEvent

StructuredCloudEvent::StructuredCloudEvent(CloudEvent* cloud_event, CloudEventFormat format, std::string serialized_cloud_event)
    : format_(format), cloud_event_(cloud_event), serialized_cloud_event_(serialized_cloud_event)
{}

CloudEventFormat StructuredCloudEvent::GetCloudEventFormat(){
    return format_;
}

CloudEvent* StructuredCloudEvent::GetCloudEvent() {
    return cloud_event_;
}

std::string StructuredCloudEvent::GetSerializedCloudEvent() {
    return serialized_cloud_event_;
} 


} // structured_cloud_events