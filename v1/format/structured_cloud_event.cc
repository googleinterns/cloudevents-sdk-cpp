#include "structured_cloud_event.h"

namespace cloud_events {
namespace format {
    
using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;

StructuredCloudEvent::StructuredCloudEvent(CloudEventFormat format, std::string serialized_cloud_event)
    : format_(format), serialized_cloud_event_(serialized_cloud_event)
{}


CloudEventFormat StructuredCloudEvent::GetCloudEventFormat(){
    return format_;
}

std::string StructuredCloudEvent::GetSerializedCloudEvent() {
    return serialized_cloud_event_;
} 

} // format
} // cloud_events
