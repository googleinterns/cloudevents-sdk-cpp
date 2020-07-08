#ifndef EVENT_FORMAT_H
#define EVENT_FORMAT_H

#include "proto/cloud_event.pb.h"
#include "third_party/base64/base64.h"
#include "third_party/statusor/statusor.h"

namespace structured_cloud_events {

enum CloudEventFormat {Json};

class StructuredCloudEvent {
    private:
        CloudEventFormat format_;
        io::cloudevents::v1::CloudEvent* cloud_event_;
        std::string serialized_cloud_event_;
    public:
        CloudEventFormat GetCloudEventFormat();
        io::cloudevents::v1::CloudEvent* GetCloudEvent();
        std::string GetSerializedCloudEvent();
        StructuredCloudEvent(io::cloudevents::v1::CloudEvent* cloud_event, CloudEventFormat format, std::string serialized_cloud_event);
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

} // structured_cloud_events

#endif