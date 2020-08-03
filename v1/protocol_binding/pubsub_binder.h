#ifndef CLOUDEVENTSCPPSDK_V1_BINDING_PUBSUBBINDER_H
#define CLOUDEVENTSCPPSDK_V1_BINDING_PUBSUBBINDER_H

#include "proto/cloud_event.pb.h"
#include "external/googleapis/google/pubsub/v1/pubsub.pb.h"
#include "third_party/statusor/statusor.h"
#include "third_party/base64/base64.h"
#include "v1/event_format/structured_cloud_event.h"
#include "v1/protocol_binding/binder.h"

namespace cloudevents {
namespace binding {

template <>
class Binder<google::pubsub::v1::PubsubMessage> {
    // According to spec in https://github.com/google/knative-gcp/blob/master/docs/spec/pubsub-protocol-binding.md#14-event-formats
    // Data encoding referenced from https://cloud.google.com/pubsub/docs/reference/rest/v1/PubsubMessage
    protected:
        // specializations for Unbind
        absl::StatusOr<cloudevents::format::Format> GetFormat(google::pubsub::v1::PubsubMessage message);
        absl::StatusOr<bool> InBinaryContentMode(google::pubsub::v1::PubsubMessage message);
        absl::StatusOr<std::string> GetPayload(google::pubsub::v1::PubsubMessage message);
        absl::StatusOr<io::cloudevents::v1::CloudEvent> UnbindBinary(google::pubsub::v1::PubsubMessage binary_message);

        // specializations for Bind
        absl::StatusOr<google::pubsub::v1::PubsubMessage> BindBinary(io::cloudevents::v1::CloudEvent cloud_event);
        absl::StatusOr<google::pubsub::v1::PubsubMessage> BindStructured(cloudevents::format::StructuredCloudEvent structured_cloud_event);   
};

} // binding
} // cloudevents

#endif // CLOUDEVENTSCPPSDK_V1_BINDING_PUBSUBBINDER_H
