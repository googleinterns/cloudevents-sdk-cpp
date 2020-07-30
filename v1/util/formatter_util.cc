#include "cloud_events_util.h"

#include <google/protobuf/util/time_util.h>

namespace cloudevents {
namespace util {

using ::io::cloudevents::v1::CloudEvent;
using ::cloudevents::format::Format;
// using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;

absl::StatusOr<Format> DestringifyFormat(std::string format_str) const;

// Converts a Format enum into a string
// Main use case is writing EventFormat into Message metadata
std::string StringifyFormat(Format format) const;

// Returns the Formatter that handles the given Format enum
absl::StatusOr<
        std::unique_ptr<Formatter>> 
    GetFormatterFor(CloudEventFormat format) const;


} // util
} // cloudevents
