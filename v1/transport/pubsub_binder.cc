#include "pubsub_binder.h"

namespace cloud_events {
namespace transport {
using ::google::pubsub::v1::PubsubMessage;
using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;
using ::cloud_events::format::CloudEventFormat;

// __________ READ ___________

absl::StatusOr<CloudEventFormat> PubsubBinder::GetFormat(PubsubMessage* message) const {    
    // if content-type attribute not found or does not have the prefix "application/cloudevents", treat as binary mode.
    google::protobuf::Map<std::string,std::string> attr_map = message -> attributes();
    auto i = attr_map.find(pubsub_content_key_);
    if (i == attr_map.end() || (i -> second).rfind(ce_contenttype_prefix_, 0) != 0) { 
        return CloudEventFormat::UNFORMATTED;
    } else {
        // strip prefix and return
        // StrToFormat will return error status if format string not recognized
        return StrToFormat((i -> second).erase(0,strlen(ce_contenttype_prefix_)));
    }
}

absl::StatusOr<std::string> PubsubBinder::GetPayload(PubsubMessage* message) const {
    // get payload and base64 decode
    return base64::base64_decode(message -> data());
}

absl::StatusOr<CloudEvent> PubsubBinder::ReadBinary(PubsubMessage* binary_message) const {
    // https://github.com/google/knative-gcp/blob/master/docs/spec/pubsub-protocol-binding.md#311-content-type

    CloudEvent cloud_event;
    // google::protobuf::Map<std::string,std::string> pubsub_attr = binary_message -> attributes();

    // handle metadata
    for (auto const& pubsub_attr : binary_message -> attributes()) {
        if (pubsub_attr.first == pubsub_content_key_) { // todo: check for duplicates?
        CloudEvent_CloudEventAttribute content_val;
        content_val.set_ce_string(pubsub_attr.second);
        (*cloud_event.mutable_attributes())[ce_content_key_] = content_val;
        } else if (pubsub_attr.first.rfind(ce_attr_prefix_, 0) == 0){
            // attr key: remove prefix
            // attr val: set CE type string
            std::string attr_key = pubsub_attr.first;
            attr_key.erase(0,strlen(ce_attr_prefix_));
            CloudEvent_CloudEventAttribute attr_val;
            attr_val.set_ce_string(pubsub_attr.second);
        (*cloud_event.mutable_attributes())[attr_key] = attr_val;
        }  
    }

    // auto i = attr_map.find(pubsub_content_key_);

    // if (i != attr_map.end()) {
    //     CloudEvent_CloudEventAttribute content_val;
    //     content_val.set_ce_string(i -> second);
    //     (*cloud_event.mutable_attributes())[ce_content_key_] = content_val;

    // } 


    // handle data
    return absl::UnimplementedError("wip");
}

// __________ WRITE ___________

// virtual absl::StatusOr<std::unique_ptr<google::protobuf::Message>> WriteBinary(io::cloudevents::v1::CloudEvent cloud_event) = 0;
// virtual absl::StatusOr<std::unique_ptr<google::protobuf::Message>> WriteStructured(cloud_events::format::StructuredCloudEvent structured_cloud_event) = 0;   


} // binder
} // cloud_events
