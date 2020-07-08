#ifndef EVENT_FORMAT_H
#define EVENT_FORMAT_H

#include "proto/cloud_event.pb.h"
#include "third_party/base64/base64.h"
#include "third_party/statusor/statusor.h"

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

class Marshaller {
    // interface for stand-alone format marshallers
    public:
        virtual absl::StatusOr<StructuredCloudEvent> Serialize(io::cloudevents::v1::CloudEvent cloud_event) = 0;
        virtual absl::StatusOr<StructuredCloudEvent> Deserialize(std::string serialized_cloud_event) = 0;
};

// class JsonMarshaller: public Marshaller {
//     public: 
//         absl::StatusOr<StructuredCloudEvent> Serialize(io::cloudevents::v1::CloudEvent cloud_event) override;
//         absl::StatusOr<StructuredCloudEvent> Deserialize(StructuredCloudEvent) override;
// };

} // format
} // cloud_events

#endif