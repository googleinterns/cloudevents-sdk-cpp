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

// Pubsub specialization
template <>
absl::StatusOr<bool> 
    Binder<google::pubsub::v1::PubsubMessage>::InStructuredContentMode(
    google::pubsub::v1::PubsubMessage* pubsub_msg);

template <>
absl::StatusOr<cloudevents::format::Format> 
    Binder<google::pubsub::v1::PubsubMessage>::GetFormat(
    google::pubsub::v1::PubsubMessage* pubsub_msg);

template <>
absl::StatusOr<std::string> 
    Binder<google::pubsub::v1::PubsubMessage>::GetPayload(
    google::pubsub::v1::PubsubMessage* pubsub_msg);

template <>
absl::StatusOr<io::cloudevents::v1::CloudEvent> 
    Binder<google::pubsub::v1::PubsubMessage>::UnbindBinary(
    google::pubsub::v1::PubsubMessage* pubsub_msg);

template <>
absl::StatusOr<google::pubsub::v1::PubsubMessage> 
    Binder<google::pubsub::v1::PubsubMessage>::BindBinary(
    io::cloudevents::v1::CloudEvent* cloud_event);

template <>
absl::StatusOr<google::pubsub::v1::PubsubMessage> 
    Binder<google::pubsub::v1::PubsubMessage>::BindStructured(
    cloudevents::format::StructuredCloudEvent* structured_ce);

} // binding
} // cloudevents

#endif // CLOUDEVENTSCPPSDK_V1_BINDING_PUBSUBBINDER_H
