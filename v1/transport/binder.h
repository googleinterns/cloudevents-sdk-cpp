#ifndef CLOUDEVENTS_BINDER_H
#define CLOUDEVENTS_BINDER_H

#include "proto/cloud_event.pb.h"
#include "proto/pubsub_message.pb.h"

#include "v1/format/structured_cloud_event.h"
#include "v1/format/json_marshaller.h"
#include "third_party/statusor/statusor.h"
#include <google/protobuf/message.h>
#include <memory>

namespace cloud_events {
namespace binder {

enum CloudEventMessageType {PUBSUB};

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class Binder {
    private:
        absl::StatusOr<std::unique_ptr<cloud_events::format::Marshaller>> GetMarshallerForFormat(cloud_events::format::CloudEventFormat format);
        virtual absl::StatusOr<cloud_events::format::CloudEventFormat> GetFormat(std::string message) = 0;
        // virtual io::cloudevents::v1::CloudEvent ReadBinary(std::string binary_message) = 0;
        // virtual io::cloudevents::v1::CloudEvent ReadStructured(std::string structured_message) = 0;
        // virtual absl::StatusOr<google::protobuf::Message> WriteBinary(io::cloudevents::v1::CloudEvent cloud_event) = 0;
        // virtual absl::StatusOr<google::protobuf::Message> WriteStructured(cloud_events::format::StructuredCloudEvent structured_cloud_event) = 0;
    public:
        // const std::string ce_prefix_ = "ce-";
        // std::string CeTypeToString(io::cloudevents::v1::CloudEvent_CloudEventAttribute attr);
        // io::cloudevents::v1::CloudEvent_CloudEventAttribute StringToCeType(std::string str_attr);
        // absl::StatusOr<google::protobuf::Message> Write(io::cloudevents::v1::CloudEvent cloud_event);
        // absl::StatusOr<google::protobuf::Message> Write(io::cloudevents::v1::CloudEvent cloud_event, cloud_events::format::CloudEventFormat format);
        // absl::StatusOr<google::protobuf::Message> Write(cloud_events::format::StructuredCloudEvent structured_cloud_event);
        // absl::StatusOr<google::protobuf::Message> Read(std::string message);
};


} // format
} // cloud_events

#endif