#ifndef STRUCTURED_CLOUD_EVENT_H
#define STRUCTURED_CLOUD_EVENT_H

#include "proto/cloud_event.pb.h"

namespace cloud_events {
namespace format {

enum CloudEventFormat {JSON};

class StructuredCloudEvent {
    private:
        CloudEventFormat format_;
        std::string serialized_cloud_event_;
    public:
        StructuredCloudEvent(CloudEventFormat format, std::string serialized_cloud_event);
        CloudEventFormat GetCloudEventFormat();
        std::string GetSerializedCloudEvent();
};

} // format
} // cloud_events

#endif