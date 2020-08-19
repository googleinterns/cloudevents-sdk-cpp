#ifndef CLOUDEVENTCPPSDK_V1_EVENTFORMAT_STRUCTUREDCLOUDEVENT_H_
#define CLOUDEVENTCPPSDK_V1_EVENTFORMAT_STRUCTUREDCLOUDEVENT_H_

#include "v1/event_format/format.h"

namespace cloudevents {
namespace format {

/*
 * An internal representation of a CloudEvent that has been formatted/ serialized
 * according to an EventFormat spec.
 */
struct StructuredCloudEvent {
  // The EventFormat spec used to create this serialization
  Format format;

  // The serialization of the CloudEvent
  std::string serialized_data;
};

}  // namespace format
}  // namespace cloudevents

#endif  // CLOUDEVENTCPPSDK_V1_EVENTFORMAT_STRUCTUREDCLOUDEVENT_H_
