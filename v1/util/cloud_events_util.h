#ifndef CLOUDEVENTSCPPSDK_V1_UTIL_CLOUDEVENTSUTIL
#define CLOUDEVENTSCPPSDK_V1_UTIL_CLOUDEVENTSUTIL

#include "third_party/statusor/statusor.h"
#include "third_party/base64/base64.h"
#include "proto/cloud_event.pb.h"

namespace cloudevents {
namespace util {

class CloudEventsUtil {
    public:
        // validate if given CloudEvent fulfills requirements to be valid
        static bool IsValid(const io::cloudevents::v1::CloudEvent cloud_event);

        // consolidate metadata in CloudEvent to a single map
        static absl::StatusOr<
            std::map<std::string, io::cloudevents::v1::CloudEvent_CloudEventAttribute>>
        GetMetadata(io::cloudevents::v1::CloudEvent cloud_event);

        // // convert CloudEvent Attributes to their canonical string representaiton
        // // as per outlined in https://github.com/cloudevents/spec/blob/master/spec.md#type-system
        static absl::StatusOr<std::string> StringifyCeType(io::cloudevents::v1::CloudEvent_CloudEventAttribute attr);    

        // Constexpr keys used when interacting with CloudEvent
        inline static constexpr absl::string_view kCeIdKey = "id";
        inline static constexpr absl::string_view kCeSourceKey = "source";
        inline static constexpr absl::string_view kCeSpecKey = "spec_version";
        inline static constexpr absl::string_view kCeTypeKey = "type";
};

} // util
} // cloudevents

#endif //CLOUDEVENTSCPPSDK_V1_UTIL_CLOUDEVENTSUTIL
