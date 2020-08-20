#include "formatter_util.h"

#include <boost/algorithm/string.hpp>    

namespace cloudevents {
namespace formatter_util {

using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::format::JsonFormatter;

constexpr char kJsonStr[] = "json";

constexpr char kErrUnkFormat[] = "A Format has not been handled.";
constexpr char kErrUnkFormatStr[] = "The given format is not recognized by the SDK.";
constexpr char kErrUnkFormatter[] = "Could not find formatter for given format.";

cloudevents_absl::StatusOr<Format> FormatterUtil::FormatFromStr(
    absl::string_view format_str) {
  if (boost::iequals(format_str, kJsonStr)) {
    return Format::kJson;
  }
  return absl::InvalidArgumentError(kErrUnkFormatStr);
}

cloudevents_absl::StatusOr<std::string> FormatterUtil::FormatToStr(
    const Format& format) {
  if (format == Format::kJson) {
    // explicit type conversion to accomodate cloudevents_absl::StatusOr
    return std::string(kJsonStr);
  }
  return absl::InternalError(kErrUnkFormat);
}

cloudevents_absl::StatusOr<std::unique_ptr<Formatter>> FormatterUtil::GetFormatter(
    const Format& format) {
  if (format == Format::kJson) {
    std::unique_ptr<Formatter> formatter = absl::make_unique<JsonFormatter>();
    return formatter;
  }
  return absl::InternalError(kErrUnkFormatter);
}

}  // namespace formatter_util
}  // namespace cloudevents
