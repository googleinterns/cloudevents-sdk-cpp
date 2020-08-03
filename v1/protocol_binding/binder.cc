#include "binder.h"

namespace cloudevents {
namespace binding {

using ::google::pubsub::v1::PubsubMessage;
using ::cloudevents::format::StructuredCloudEvent;
using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::formatter_util::FormatterUtil;

const char* kPubsubContentKey = "content-type";

template <>
absl::StatusOr<bool> Binder<PubsubMessage>::InStructuredContentMode(PubsubMessage* pubsub_msg) {
    google::protobuf::Map<std::string,std::string> attrs;
    attrs = pubsub_msg -> attributes();
    auto ind = attrs.find(kPubsubContentKey);
    return (ind != attrs.end() && (ind -> second).rfind(kContenttypePrefix.data(), 0) == 0); 
}

// template <>
// absl::StatusOr<Format> Binder<PubsubMessage>::GetFormat(PubsubMessage pubsub_msg) {
//     return Format::kJson;
// }

} // binding
} // cloudevents