#include "formatter_util.h"

#include <gtest/gtest.h>

namespace cloudevents {
namespace util {

using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::format::JsonFormatter;

TEST(FormatterUtilTest, DestringifyFormat_Json) {
    absl::StatusOr<Format> format_str = FormatterUtil::DestringifyFormat("json");
    ASSERT_TRUE(format_str.ok());
    ASSERT_EQ((*format_str), Format::kJson);
}

TEST(FormatterUtilTest, DestringifyFormat_Invalid) {
    ASSERT_TRUE(absl::IsInvalidArgument(
        FormatterUtil::DestringifyFormat("not a format").status()));
}

TEST(FormatterUtilTest, StringifyFormat_Json) {
    ASSERT_EQ(FormatterUtil::StringifyFormat(Format::kJson), "json");
}

TEST(FormatterUtilTest, GetFormatter_Json) {
    absl::StatusOr<std::unique_ptr<Formatter>> get_formatter;
    get_formatter = FormatterUtil::GetFormatter(Format::kJson);
    ASSERT_TRUE(get_formatter.ok());
    ASSERT_EQ(typeid(*(*(get_formatter))), typeid(JsonFormatter));  // dereference twice
}

} // util
} // cloudevents
