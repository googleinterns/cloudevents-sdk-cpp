#include "formatter_util.h"

namespace cloudevents {
namespace util {

using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;

absl::StatusOr<Format> FormatterUtil::StringifyFormat(std::string format_str) const {

}

absl::StatusOr<std::string> FormatterUtil::DestringifyFormat(Format format) const {

}

absl::StatusOr<std::unique_ptr<Formatter>> FormatterUtil::GetFormatter(Format format) const {

}

} // util
} // cloudevents
