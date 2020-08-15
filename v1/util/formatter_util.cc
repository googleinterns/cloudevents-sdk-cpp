#include "formatter_util.h"

namespace cloudevents {
namespace formatter_util {

using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::format::JsonFormatter;

constexpr char kErrUnkFormat[] = "A Format has not been handled.";
constexpr char kErrUnkFormatStr[] = "The given format is not recognized by the SDK.";
constexpr char kErrUnkFormatter[] = "Could not find formatter for given format.";

absl::StatusOr<Format> FormatterUtil::FormatFromStr(
    absl::string_view format_str) {
  if (format_str == "json") {
    return Format::kJson;
  }
  return absl::InvalidArgumentError(kErrUnkFormatStr);
}

absl::StatusOr<std::string> FormatterUtil::FormatToStr(
    const Format& format) {
  if (format == Format::kJson) {
    return std::string("json");
  }
  return absl::InternalError(kErrUnkFormat);
}

}  // namespace formatter_util
}  // namespace cloudevents
