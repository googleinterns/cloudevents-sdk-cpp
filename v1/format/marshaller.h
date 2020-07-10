#ifndef MARSHALLER_H
#define MARSHALLER_H

#include "proto/cloud_event.pb.h"
#include "v1/format/structured_cloud_event.h"
#include "third_party/statusor/statusor.h"

namespace cloud_events {
namespace format {
class Marshaller {
    // interface for stand-alone format marshallers
    public:
        virtual absl::StatusOr<StructuredCloudEvent> Serialize(io::cloudevents::v1::CloudEvent cloud_event) = 0;
        virtual absl::StatusOr<io::cloudevents::v1::CloudEvent> Deserialize(cloud_events::format::StructuredCloudEvent structured_cloud_event) = 0;
};

} // format
} // cloud_events

#endif