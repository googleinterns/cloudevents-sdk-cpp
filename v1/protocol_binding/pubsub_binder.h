#ifndef CLOUDEVENTSCPPSDK_V1_BINDING_PUBSUBBINDER_H
#define CLOUDEVENTSCPPSDK_V1_BINDING_PUBSUBBINDER_H

#include "external/googleapis/google/pubsub/v1/pubsub.pb.h"
#include "proto/cloud_event.pb.h"
#include "third_party/statusor/statusor.h"
#include "v1/protocol_binding/binder.h"

namespace cloudevents {
namespace binding {

// Inheritence from template specialization to handle PubsubMessages.
// For the full template code and comments,
// view `//v1/protocol_binding/binder.h`.
//
// Uses GoogleApi's PubsubMessage proto.
class PubsubBinder: public Binder<google::pubsub::v1::PubsubMessage> {
 private:
  // _____ Operations used in Bind Binary _____

  absl::Status BindMetadata(const std::string& key,
    const io::cloudevents::v1::CloudEvent_CloudEventAttribute& val,
    google::pubsub::v1::PubsubMessage& pubsub_msg) override;

  absl::Status BindDataBinary(const std::string& bin_data,
    google::pubsub::v1::PubsubMessage& pubsub_msg) override;


  absl::Status BindDataText(const std::string& text_data,
    google::pubsub::v1::PubsubMessage& pubsub_msg) override;

  // _____ Operations used in Bind Structured _____

  absl::Status BindContentType(const std::string& contenttype,
    google::pubsub::v1::PubsubMessage& pubsub_msg) override;

  absl::Status BindDataStructured(const std::string& payload,
    google::pubsub::v1::PubsubMessage& pubsub_msg) override;

  // _____ Operations used in Unbind Binary _____

  absl::Status UnbindMetadata(
    const google::pubsub::v1::PubsubMessage& pubsub_msg,
    io::cloudevents::v1::CloudEvent& cloud_event) override;

  absl::Status UnbindData(
    const google::pubsub::v1::PubsubMessage& pubsub_msg,
    io::cloudevents::v1::CloudEvent& cloud_event) override;

  // _____ Operations used in Unbind Structured _____

  cloudevents_absl::StatusOr<std::string> GetContentType(
    const google::pubsub::v1::PubsubMessage& pubsub_msg) override;

  cloudevents_absl::StatusOr<std::string> GetPayload(
    const google::pubsub::v1::PubsubMessage& pubsub_msg) override;
};

}  // namespace binding
}  // namespace cloudevents

#endif  // CLOUDEVENTSCPPSDK_V1_BINDING_PUBSUBBINDER_H
