#include "binder_util.h"

#include <gtest/gtest.h>

namespace cloudevents {
namespace binder_util {

TEST(AddMetadataPrefix, Valid) {
  std::string key = "test";
  BinderUtil::AddMetadataPrefix(key);
  ASSERT_EQ(key, "ce-test");
}

TEST(StripMetadataPrefix, Valid) {
  std::string ce_key = "ce-test";
  absl::Status strip = BinderUtil::StripMetadataPrefix(ce_key);

  ASSERT_TRUE(strip.ok());
  ASSERT_EQ(ce_key, "test");
}

TEST(StripMetadataPrefix, Invalid) {
  std::string key = "test";
  absl::Status strip = BinderUtil::StripMetadataPrefix(key);

  ASSERT_FALSE(strip.ok());
  ASSERT_TRUE(absl::IsInvalidArgument(strip));
}

TEST(AddContentTypePrefix, Valid) {
  std::string key = "test";
  BinderUtil::AddContentTypePrefix(key);
  ASSERT_EQ(key, "application/cloudevents+test");
}

TEST(StripContentTypePrefix, Valid) {
  std::string ce_key = "application/cloudevents+test";
  absl::Status strip = BinderUtil::StripContentTypePrefix(ce_key);

  ASSERT_TRUE(strip.ok());
  ASSERT_EQ(ce_key, "test");
}

TEST(StripContentTypePrefix, Invalid) {
  std::string key = "test";
  absl::Status strip = BinderUtil::StripContentTypePrefix(key);

  ASSERT_FALSE(strip.ok());
  ASSERT_TRUE(absl::IsInvalidArgument(strip));
}


}  // namespace binder_util
}  // namespace cloudevents
