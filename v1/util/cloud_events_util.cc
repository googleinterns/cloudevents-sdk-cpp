#include "cloud_events_util.h"

#include <google/protobuf/util/time_util.h>

namespace cloudevents {
namespace cloudevents_util {

using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;
using ::google::protobuf::util::TimeUtil;

bool CloudEventsUtil::IsValid(const CloudEvent& cloud_event) {
    return !(cloud_event.id().empty() ||
        cloud_event.source().empty() ||
        cloud_event.spec_version().empty() ||
        cloud_event.type().empty());
}

}  // namespace cloudevents_util
}  // namespace cloudevents
