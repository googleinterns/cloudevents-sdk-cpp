#ifndef CLOUDEVENTSCPPSDK_V1_UTIL_FORMATTERUTIL
#define CLOUDEVENTSCPPSDK_V1_UTIL_FORMATTERUTIL

#include "third_party/statusor/statusor.h"
// #include "third_party/base64/base64.h"
#include "proto/cloud_event.pb.h"
#include "v1/event_format/json_formatter.h"

namespace cloudevents {
namespace util {

class FormatterUtil {
    public:
        // Converts a string that represents a format into a Format enum
        // Main use case is inferring EventFormat from Message metadata
        // Returns argument error if string is not recognized
        absl::StatusOr<cloud_events::format::Format> DestringifyFormat(std::string format_str) const;

        // Converts a Format enum into a string
        // Main use case is writing EventFormat into Message metadata
        std::string StringifyFormat(cloud_events::format::Format format) const;

        // Returns the Formatter that handles the given Format enum
        absl::StatusOr<
                std::unique_ptr<cloud_events::format::Formatter>> 
            GetFormatterFor(cloud_events::format::CloudEventFormat format) const;
};

} // util
} // cloudevents

#endif //CLOUDEVENTSCPPSDK_V1_UTIL_FORMATTERUTIL
