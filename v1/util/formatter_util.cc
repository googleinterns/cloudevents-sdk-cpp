#include "formatter_util.h"

namespace cloudevents {
namespace util {

using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::format::JsonFormatter;

absl::StatusOr<Format> FormatterUtil::DestringifyFormat(std::string format_str) {
    if (format_str == "json") {
        return Format::kJson;
    }
    return absl::InvalidArgumentError("The given format is not currently recognized by the SDK.");
}

std::string FormatterUtil::StringifyFormat(Format format) {
    switch (format) {
        case Format::kJson:
            return std::string("json");
    }
}

absl::StatusOr<
        std::unique_ptr<Formatter>> 
        FormatterUtil::GetFormatter(Format format) {
    switch (format) {
        case Format::kJson:
            auto m = new JsonFormatter;
            return std::unique_ptr<Formatter>(m);
    }
    return absl::InternalError("Could not find formatter for given format.");
}

} // util
} // cloudevents
