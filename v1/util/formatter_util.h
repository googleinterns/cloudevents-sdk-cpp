#ifndef CLOUDEVENTSCPPSDK_V1_UTIL_FORMATTERUTIL
#define CLOUDEVENTSCPPSDK_V1_UTIL_FORMATTERUTIL

#include "third_party/statusor/statusor.h"
#include "proto/cloud_event.pb.h"
#include "v1/event_format/format.h"
#include "v1/event_format/formatter.h"
#include "v1/event_format/json_formatter.h"

namespace cloudevents {
namespace util {

class FormatterUtil {
    public:
        // Converts a given string to a cloudevents::format::Format
        // Throws InternalError() if string cannot be mapped succesfully.
        static absl::StatusOr<
                cloudevents::format::Format> 
            DestringifyFormat(std::string format_str);

        // Converts a cloudevents::format::Format to its string representation
        static std::string StringifyFormat(cloudevents::format::Format format);
        
        // Returns Formatter that handles the given Format
        static absl::StatusOr<
                std::unique_ptr<cloudevents::format::Formatter>> 
            GetFormatter(cloudevents::format::Format format);
};

} // util
} // cloudevents

#endif //CLOUDEVENTSCPPSDK_V1_UTIL_FORMATTERUTIL
