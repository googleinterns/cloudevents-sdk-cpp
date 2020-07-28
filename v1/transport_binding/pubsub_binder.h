#ifndef PUBSUB_BINDER_H
#define PUBSUB_BINDER_H

#include "v1/transport/binder.h"
#include "v1/format/structured_cloud_event.h"

#include "proto/cloud_event.pb.h"
#include "proto/pubsub_message.pb.h"

#include "third_party/statusor/statusor.h"
#include "third_party/base64/base64.h"

namespace cloud_events {
namespace transport {

class PubsubBinder: public Binder {
    // According to spec in https://github.com/google/knative-gcp/blob/master/docs/spec/pubsub-protocol-binding.md#14-event-formats
    // Data encoding referenced from https://cloud.google.com/pubsub/docs/reference/rest/v1/PubsubMessage
    protected:
        // Read
        absl::StatusOr<cloud_events::format::CloudEventFormat> GetFormat(google::pubsub::v1::PubsubMessage* message) const;
        absl::StatusOr<std::string> GetPayload(google::pubsub::v1::PubsubMessage* message) const;
        absl::StatusOr<io::cloudevents::v1::CloudEvent> ReadBinary(google::pubsub::v1::PubsubMessage* binary_message) const;
        
        // Write
        absl::StatusOr<std::unique_ptr<google::protobuf::Message>> WriteBinary(io::cloudevents::v1::CloudEvent cloud_event);
        absl::StatusOr<std::unique_ptr<google::protobuf::Message>> WriteStructured(cloud_events::format::StructuredCloudEvent structured_cloud_event);

    public:
        const static char* pubsub_content_key_;
        const static char* ce_content_key_; 

};

} // transport
} // cloud_events

#endif