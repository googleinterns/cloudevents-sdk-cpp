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

absl::StatusOr<
        absl::flat_hash_map<std::string, CloudEvent_CloudEventAttribute>>
        CloudEventsUtil::GetMetadata(const CloudEvent& cloud_event) {
    if (!CloudEventsUtil::IsValid(cloud_event)) {
        return absl::InvalidArgumentError(
            "GetMetadata can only be called with valid Cloud Event.");
    }

    // create absl::flat_hash_map from protobuf map of optional/ extensionattrs
    absl::flat_hash_map<std::string, CloudEvent_CloudEventAttribute> attrs(
        (cloud_event.attributes()).begin(),
        cloud_event.attributes().end());

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

}  // namespace cloudevents_util
}  // namespace cloudevents
