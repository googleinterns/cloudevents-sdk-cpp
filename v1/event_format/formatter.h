#ifndef CLOUDEVENTCPPSDK_V1_EVENTFORMAT_FORMATTER_H_
#define CLOUDEVENTCPPSDK_V1_EVENTFORMAT_FORMATTER_H_

#include "proto/cloud_event.pb.h"
#include "v1/event_format/structured_cloud_event.h"
#include "third_party/statusor/statusor.h"

namespace cloudevents {
namespace format {
class Formatter {
    // interface - each stand-alone format will have a concrete implementation of this
    public:
        virtual absl::StatusOr<StructuredCloudEvent> Serialize(io::cloudevents::v1::CloudEvent cloud_event) = 0;
        virtual absl::StatusOr<io::cloudevents::v1::CloudEvent> Deserialize(StructuredCloudEvent structured_cloud_event) = 0;
};

} // format
} // cloudevents

#endif //CLOUDEVENTCPPSDK_V1_EVENTFORMAT_FORMATTER_H_
