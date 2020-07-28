#ifndef CLOUDEVENTSCPPSDK_V1_UTIL_CFORMATTERTIL
#define CLOUDEVENTSCPPSDK_V1_UTIL_CFORMATTERTIL
namespace cloudevents {
namespace util {

class FormatterUtil {
    // interfacing with Formatters

    // Converts a given string to a cloudevents::format::Format
    // Throws InternalError() if string cannot be mapped succesfully.
    absl::StatusOr<cloudevents::format::Format> DestringifyFormat(std::string format_str) const;

    // Converts a cloudevents::format::Format to its string representation
    absl::StatusOr<std::string> StringifyFormat(cloudevents::format::Format format) const;
    absl::StatusOr<std::unique_ptr<cloudevents::format::Formatter>> GetFormatter(cloudevents::format::Format format) const;

};

} // util
} // cloudevents

#endif //CLOUDEVENTSCPPSDK_V1_UTIL_FORMATTERUTIL