#include "binder.h"

namespace cloud_events {
namespace binder {
using ::google::pubsub:::v1::PubsubMessage;
using ::io::cloudevents::v1::CloudEvent;
// using ::cloud_events::format::StructuredCloudEvent;

absl::StatusOr<CloudEventFormat> GetFormat(PubsubMessage* message) const {
    // According to spec in https://github.com/google/knative-gcp/blob/master/docs/spec/pubsub-protocol-binding.md#14-event-formats
    
    // if content-type attribute not found or does not have the prefix "application/cloudevents", treat as binary mode.
    map<std::string,std::string>::iterator i = message.attributes().find(format_attr_key_);
    if (i == m.end() || (i -> second).rfind(ce_attribute_key_prefix_, 0) != 0) { 
        return CloudEventFormat::UNFORMATTED;
    } else {
        // strip prefix
        std::string format_str = (i -> second).erase(0, ce_attribute_key_prefix_.length());
        return StrToFormat(format_str); // will return error status if format string not recognized
    }
}

} // binder
} // cloud_events
