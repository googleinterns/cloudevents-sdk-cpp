#include "binder_util.h"

namespace cloudevents {
namespace binder_util {

constexpr char kMetadataPrefix[] = "ce-";
constexpr char kContentTypePrefix[] = "application/cloudevents+";

// length of prefix constants are made constexpr to avoid
// multiple calls to strlen()
constexpr size_t kMetadataPrefixLen = strlen(kMetadataPrefix);
constexpr size_t kContentTypePrefixLen = strlen(kContentTypePrefix);

constexpr char kErrNoPrefix[] = "Prefix is not present in the given value.";

void BinderUtil::AddMetadataPrefix(std::string& key) {
  key = kMetadataPrefix+key;
}

}  // namespace binder_util
}  // namespace cloudevents
