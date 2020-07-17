#ifndef PUBSUB_BINDER_H
#define PUBSUB_BINDER_H

#include "v1/format/structured_cloud_event.h"

#include "proto/cloud_event.pb.h"
#include "proto/pubsub_message.pb.h"

#include "third_party/statusor/statusor.h"
#include "third_party/base64/base64.h"

class PubsubBinder: public Binder {
    // According to spec in https://github.com/google/knative-gcp/blob/master/docs/spec/pubsub-protocol-binding.md#14-event-formats
    // Data encoding referenced from https://cloud.google.com/pubsub/docs/reference/rest/v1/PubsubMessage
    private:
        absl::StatusOr<cloud_events::format::CloudEventFormat> GetFormat(google::pubsub:::v1::PubsubMessage* message) const;
        absl::StatusOr<std::string> GetPayload(google::pubsub:::v1::PubsubMessage* message) const;
    public:
        std::string format_attr_key_ = "content-type";

}

#endif