#ifndef PUBSUB_BINDER_H
#define PUBSUB_BINDER_H

#include "v1/format/structured_cloud_event.h"

#include "proto/cloud_event.pb.h"
#include "proto/pubsub_message.pb.h"

#include "third_party/statusor/statusor.h"

class PubsubBinder: public Binder {
    private:
        absl::StatusOr<cloud_events::format::CloudEventFormat> GetFormat(google::pubsub:::v1::PubsubMessage* message) const;
    public:
        std::string format_attr_key_ = "content-type";

}

#endif