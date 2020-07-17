#ifndef CLOUDEVENTS_BINDER_H
#define CLOUDEVENTS_BINDER_H

#include "proto/cloud_event.pb.h"
#include "proto/pubsub_message.pb.h"

#include "v1/format/structured_cloud_event.h"
#include "v1/format/json_marshaller.h"
#include "third_party/statusor/statusor.h"
#include "third_party/base64/base64.h"
#include <google/protobuf/message.h>
#include <memory>
#include <regex> 

namespace cloud_events {
namespace binder {

// union Message {
//     google::pubsub::v1::PubsubMessage pubsub;
// };

// union CloudEventInMode {
//     io::cloudevents::v1::CloudEvent binary;
//     cloud_events::format::StructuredCloudEvent structured;
// }

class Binder {
    // Abstract class, each protocol should implement a concrete version of this.

    // In general built off of a template method design pattern.  
    // Main function methods are Read and Write.

    // Write takes a CloudEvent [and CloudEventFormat], or StructuredCloudEvent as input, and creates a XMessage as ouput
    // Read takes an XMessage as input, and creates a CloudEvent or StructuredCloudEvent.
    private:
        // implemented operations for Read/ Write
        absl::StatusOr<std::unique_ptr<cloud_events::format::Marshaller>> GetMarshallerForFormat(cloud_events::format::CloudEventFormat format) const;

        // virtual operations for Read
        virtual absl::StatusOr<cloud_events::format::CloudEventFormat> GetFormat(google::protobuf::Message* message) const = 0;
        virtual absl::StatusOr<std::string> GetPayload(google::protobuf::Message* message) const = 0;
        virtual absl::StatusOr<io::cloudevents::v1::CloudEvent> ReadBinary(google::protobuf::Message* binary_message) const = 0;

        // virtual operations for Write
        virtual absl::StatusOr<std::unique_ptr<google::protobuf::Message>> WriteBinary(io::cloudevents::v1::CloudEvent cloud_event) = 0;
        virtual absl::StatusOr<std::unique_ptr<google::protobuf::Message>> WriteStructured(cloud_events::format::StructuredCloudEvent structured_cloud_event) = 0;   
    public:
        const std::string ce_prefix_ = "ce-";
        absl::StatusOr<std::string> CeTypeToString(io::cloudevents::v1::CloudEvent_CloudEventAttribute attr);

        absl::StatusOr<std::unique_ptr<google::protobuf::Message>> Write(io::cloudevents::v1::CloudEvent cloud_event);
        absl::StatusOr<std::unique_ptr<google::protobuf::Message>> Write(io::cloudevents::v1::CloudEvent cloud_event, cloud_events::format::CloudEventFormat format);
        absl::StatusOr<std::unique_ptr<google::protobuf::Message>> Write(cloud_events::format::StructuredCloudEvent structured_cloud_event);

        absl::StatusOr<std::unique_ptr<google::protobuf::Message>> Read(google::protobuf::Message* message, bool deserialize = true) const;
        // absl::Status Read(google::protobuf::Message message, io::cloudevents::v1::CloudEvent cloud_event);
        // absl::Status Read(google::protobuf::Message message, cloud_events::format::StructuredCloudEvent structured_cloud_event);
};

} // format
} // cloud_events

#endif