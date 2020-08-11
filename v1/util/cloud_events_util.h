#ifndef CLOUDEVENTSCPPSDK_V1_UTIL_CLOUDEVENTSUTIL
#define CLOUDEVENTSCPPSDK_V1_UTIL_CLOUDEVENTSUTIL

#include "absl/container/flat_hash_map.h"
#include "third_party/statusor/statusor.h"
#include "third_party/base64/base64.h"
#include "proto/cloud_event.pb.h"

namespace cloudevents {
namespace cloudevents_util {

class CloudEventsUtil {
    public:
        // // validate if given CloudEvent fulfills requirements to be valid
        // static bool IsValid(const io::cloudevents::v1::CloudEvent& cloud_event);

        // // get metadata from CloudEvent in a single map
        // static absl::StatusOr<absl::flat_hash_map<
        //     std::string, io::cloudevents::v1::CloudEvent_CloudEventAttribute>>
        //     GetMetadata(const io::cloudevents::v1::CloudEvent& cloud_event);

        // // TODO (#44): Overload SetMetadata to accept a map of attributes

        // // set metadata without dealing with CloudEvent proto structure
        // static void SetMetadata(const std::string& key, const std::string& val,
        //     io::cloudevents::v1::CloudEvent& cloud_event);

        // // convert CloudEvent attributes to canonical string representaiton
        // // https://github.com/cloudevents/spec/blob/master/spec.md#type-system
        // static absl::StatusOr<std::string> StringifyCeType(
        //     const io::cloudevents::v1::CloudEvent_CloudEventAttribute& attr);
};

}  // namespace cloudevents_util
}  // namespace cloudevents

#endif  // CLOUDEVENTSCPPSDK_V1_UTIL_CLOUDEVENTSUTIL
