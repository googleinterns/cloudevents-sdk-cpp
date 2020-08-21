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
  // validate if given CloudEvent fulfills requirements to be valid
  static absl::Status IsValid(
    const io::cloudevents::v1::CloudEvent& cloud_event);

  // get metadata from CloudEvent in a single map
  static cloudevents_absl::StatusOr<absl::flat_hash_map<
    std::string, io::cloudevents::v1::CloudEvent_CloudEventAttribute>>
    GetMetadata(const io::cloudevents::v1::CloudEvent& cloud_event);

  // TODO (#44): Overload SetMetadata to accept a map of attributes

  // set metadata without dealing with CloudEvent proto structure
  static absl::Status SetMetadata(const std::string& key,
    const std::string& val,
    io::cloudevents::v1::CloudEvent& cloud_event);

  // convert CloudEvent attributes to canonical string representaiton
  // https://github.com/cloudevents/spec/blob/master/spec.md#type-system
  static cloudevents_absl::StatusOr<std::string> ToString(
    const io::cloudevents::v1::CloudEvent_CloudEventAttribute& attr);

  // convert std::string to ce-string.
  // ce-string is the default ce-type for unrecognized metadata
 private:
  static io::cloudevents::v1::CloudEvent_CloudEventAttribute ToCeString(
    const std::string& val);
};

}  // namespace cloudevents_util
}  // namespace cloudevents

#endif  // CLOUDEVENTSCPPSDK_V1_UTIL_CLOUDEVENTSUTIL
