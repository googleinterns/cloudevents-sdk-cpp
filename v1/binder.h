#ifndef CLOUDEVENTS_BINDER_H
#define CLOUDEVENTS_BINDER_H

#include "proto/cloud_event.pb.h"
#include "v1/format.h"
#include "third_party/statusor/statusor.h"

namespace cloud_events {
namespace binder {

enum CloudEventMessageType {PUBSUB};

template <class Message>
class Binder {
    private:
        absl::StatusOr<Marshaller> GetMarshallerForFormat(cloud_events::format::CloudEventFormat format);
        virtual absl::StatusOr<cloud_events::format::CloudEventFormat> GetFormat(std::string message) = 0;
        virtual io::cloudevents::v1::CloudEvent ReadBinary(std::string binary_message) = 0;
        virtual io::cloudevents::v1::CloudEvent ReadStructured(std::string structured_message) = 0;
        virtual absl::StatusOr<Message> WriteBinary(io::cloudevents::v1::CloudEvent cloud_event) = 0;
        virtual absl::Status<Message> WriteStructured(cloudevents::format::StructuredCloudEvent structured_cloud_event) = 0;
    public:
        const std::string ce_prefix_ = "ce-";
        std::string CeTypeToString(io::cloudevents::v1::CloudEvent_CloudEventAttribute attr);
        io::cloudevents::v1::CloudEvent_CloudEventAttribute StringToCeType(std::string str_attr);
        absl::StatusOr<Message> Write(io::cloudevents::v1::CloudEvent cloud_event);
        absl::StatusOr<Message> Write(io::cloudevents::v1::CloudEvent cloud_event, cloud_events::format::CloudEventFormat format);
        absl::StatusOr<Message> Write(cloudevents::format::StructuredCloudEvent cloudstructured_cloud_event_event);
        absl::StatusOr<Message> Read(std::string message);
}


} // format
} // cloud_events

#endif