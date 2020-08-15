#include "formatter_util.h"

#include <gtest/gtest.h>

namespace cloudevents {
namespace formatter_util {

using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::format::JsonFormatter;

TEST(FormatterUtilTest, FormatFromStr_Json) {
  absl::StatusOr<Format> format = FormatterUtil::FormatFromStr("json");
  ASSERT_TRUE(format.ok());
  ASSERT_EQ(*format, Format::kJson);
}

TEST(FormatterUtilTest, FormatFromStr_Invalid) {
  ASSERT_TRUE(absl::IsInvalidArgument(
    FormatterUtil::FormatFromStr("not a format").status()));
}

TEST(FormatterUtilTest, FormatToStr_Json) {
  absl::StatusOr<std::string> format_str = FormatterUtil::FormatToStr(
    Format::kJson);
  ASSERT_TRUE(format_str.ok());
  ASSERT_EQ(*format_str, "json");
}

TEST(FormatterUtilTest, GetFormatter_Json) {
  absl::StatusOr<std::unique_ptr<Formatter>> get_formatter;
  get_formatter = FormatterUtil::GetFormatter(Format::kJson);
  ASSERT_TRUE(get_formatter.ok());
  // dereference twice
  ASSERT_EQ(typeid(**get_formatter), typeid(JsonFormatter));
}

}  // namespace formatter_util
}  // namespace cloudevents
