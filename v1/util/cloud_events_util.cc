#include "cloud_events_util.h"

#include <google/protobuf/util/time_util.h>

namespace cloudevents {
namespace util {

using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;

bool CloudEventsUtil::IsValid(CloudEvent cloud_event) {
    return !(cloud_event.id().empty() || 
        cloud_event.source().empty() || 
        cloud_event.spec_version().empty() ||
        cloud_event.type().empty());
}

absl::StatusOr<std::map<std::string, CloudEvent_CloudEventAttribute>> CloudEventsUtil::GetMetadata(
    CloudEvent cloud_event) {
    if (!CloudEventsUtil::IsValid(cloud_event)) {
        return absl::InvalidArgumentError("GetMetadata can only be called with valid Cloud Event.");
    }

    // create std::map from protobuf map of optional/ extensionattrs
    std::map<std::string, CloudEvent_CloudEventAttribute> attrs(
        (cloud_event.attributes()).begin(),
        cloud_event.attributes().end()
    );

    // insert required attrs
    CloudEvent_CloudEventAttribute attr_val;
    attr_val.set_ce_string(cloud_event.id());
    attrs["id"] = attr_val;
    attr_val.set_ce_string(cloud_event.source());
    attrs["source"] = attr_val;
    attr_val.set_ce_string(cloud_event.spec_version());
    attrs["spec_version"] = attr_val;
    attr_val.set_ce_string(cloud_event.type());
    attrs["type"] = attr_val;

    return attrs;
}

}

} // util
} // cloudevents
