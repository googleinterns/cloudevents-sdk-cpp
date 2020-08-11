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

void CloudEventsUtil::SetMetadata(const std::string& key,
        const std::string& val, CloudEvent& cloud_event){
    // TODO (#39): Should we try to infer CE Type from serialization?
    CloudEvent_CloudEventAttribute attr;
    attr.set_ce_string(val);
    if (key == "id") {
        cloud_event.set_id(val);
    } else if (key == "source") {
        cloud_event.set_source(val);
    } else if (key == "spec_version") {
        cloud_event.set_spec_version(val);
    } else if (key == "type") {
        cloud_event.set_type(val);
    } else {
        (*cloud_event.mutable_attributes())[key] = attr;
    }
}

}  // namespace cloudevents_util
}  // namespace cloudevents
