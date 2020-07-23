#ifndef CLOUDEVENTCPPSDK_V1_EVENTFORMAT_STRUCTUREDCLOUDEVENT_H_
#define CLOUDEVENTCPPSDK_V1_EVENTFORMAT_STRUCTUREDCLOUDEVENT_H_

#include "v1/event_format/format.h"

namespace cloudevents {
namespace format {

struct StructuredCloudEvent {
    Format format;
    std::string serialization;
};

} // format
} // cloudevents

#endif //CLOUDEVENTCPPSDK_V1_EVENTFORMAT_STRUCTUREDCLOUDEVENT_H_