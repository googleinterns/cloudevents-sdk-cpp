#ifndef CLOUDEVENTSCPPSDK_V1_UTIL_BINDERUTIL
#define CLOUDEVENTSCPPSDK_V1_UTIL_BINDERUTIL

#include "third_party/statusor/statusor.h"

namespace cloudevents {
namespace binder_util {

class BinderUtil {
 public:
  // add prefix to Message attributes containing
  // Binary-ContentMode CloudEvent Metadata
  static void AddMetadataPrefix(std::string& key);

  // if Message attribute has CloudEvent prefix, strip
  // else, return InvalidArgument status
  static absl::Status StripMetadataPrefix(std::string& key);

  // add prefix to content-type value to indicate
  // Structured-ContentMode payload
  static void AddContentTypePrefix(std::string& val);

  // // if contenttype value has CloudEvent prefix, strip
  // // else, return InvalidArgument status
  // static absl::Status StripContentTypePrefix(std::string& key);
};

}  // namespace binder_util
}  // namespace cloudevents

#endif  // CLOUDEVENTSCPPSDK_V1_UTIL_BINDERUTIL
