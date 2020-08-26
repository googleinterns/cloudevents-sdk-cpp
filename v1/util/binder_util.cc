#include "binder_util.h"

namespace cloudevents {
namespace binder_util {

constexpr char kMetadataPrefix[] = "ce-";
constexpr char kContentTypePrefix[] = "application/cloudevents+";

// Length of prefix constants are made constexpr to avoid
// multiple calls to sizeof().
// Does not count the NULL character.
constexpr size_t kMetadataPrefixLen = sizeof(kMetadataPrefix) - 1;
constexpr size_t kContentTypePrefixLen = sizeof(kContentTypePrefix) - 1;

constexpr char kErrNoPrefix[] = "Prefix is not present in the given value.";

void BinderUtil::AddMetadataPrefix(std::string& key) {
  key = kMetadataPrefix + key;
}

absl::Status BinderUtil::StripMetadataPrefix(std::string& key) {
  if (key.rfind(kMetadataPrefix, 0) != 0) {
    return absl::InvalidArgumentError(kErrNoPrefix);
  }
  key.erase(0, kMetadataPrefixLen);
  return absl::OkStatus();
}

void BinderUtil::AddContentTypePrefix(std::string& key) {
  key = kContentTypePrefix + key;
}

absl::Status BinderUtil::StripContentTypePrefix(std::string& key) {
  if (key.rfind(kContentTypePrefix, 0) != 0) {
    return absl::InvalidArgumentError(kErrNoPrefix);
  }
  key.erase(0, kContentTypePrefixLen);
  return absl::OkStatus();
}

}  // namespace binder_util
}  // namespace cloudevents
