#include "pubsub_binder.h"

namespace cloud_events {
namespace transport {
using ::google::pubsub::v1::PubsubMessage;
using ::io::cloudevents::v1::CloudEvent;
using ::cloud_events::format::CloudEventFormat;

// __________ READ ___________

absl::StatusOr<CloudEventFormat> PubsubBinder::GetFormat(PubsubMessage* message) const {    
    // if content-type attribute not found or does not have the prefix "application/cloudevents", treat as binary mode.
    auto i = (message -> attributes()).find(content_attr_key_);
    if (i == (message -> attributes()).end() || (i -> second).rfind(ce_contenttype_prefix_, 0) != 0) { 
        return CloudEventFormat::UNFORMATTED;
    } else {
        // strip prefix and return
        std::string format_str = i -> second; // copy to maintain const
        return StrToFormat(format_str.erase(0,strlen(ce_contenttype_prefix_))); // will return error status if format string not recognized
    }
}

absl::StatusOr<std::string> PubsubBinder::GetPayload(PubsubMessage* message) const {
    // get payload and base64 decode
    return base64::base64_decode(message -> data());
}

// absl::StatusOr<CloudEvent> PubsubBinder::ReadBinary(PubsubMessage* binary_message) const {
//     // https://github.com/google/knative-gcp/blob/master/docs/spec/pubsub-protocol-binding.md#311-content-type

//     CloudEvent cloud_event;
//     // handle metadata
//     map<std::string,std::string>::iterator i = message.attributes().find(content_attr_key_);

//     if (i != m.end()) {

//     } 


//     // handle data
// }

// __________ WRITE ___________

// virtual absl::StatusOr<std::unique_ptr<google::protobuf::Message>> WriteBinary(io::cloudevents::v1::CloudEvent cloud_event) = 0;
// virtual absl::StatusOr<std::unique_ptr<google::protobuf::Message>> WriteStructured(cloud_events::format::StructuredCloudEvent structured_cloud_event) = 0;   


} // binder
} // cloud_events
