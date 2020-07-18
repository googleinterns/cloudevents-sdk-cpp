#include "pubsub_binder.h"

namespace cloud_events {
namespace transport {
using ::google::pubsub::v1::PubsubMessage;
using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;
using ::cloud_events::format::CloudEventFormat;
using ::cloud_events::format::StructuredCloudEvent;
const char* PubsubBinder::pubsub_content_key_ = "content-type";
const char* PubsubBinder::ce_content_key_ = "datacontenttype";

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
    // check valid CloudEvent
    google::protobuf::Map<std::string, std::string> pubsub_attr = binary_message -> attributes();

    // TODO (#27): encapsulate checking required attributes in CloudEvent
    if (!pubsub_attr.count("ce-id") || !pubsub_attr.count("ce-source") || !pubsub_attr.count("ce-spec_version") || !pubsub_attr.count("ce-type")) {
        return absl::InvalidArgumentError("Message does not contain required CloudEvent attributes");
    }

    CloudEvent cloud_event;

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

    // handle data
    std::string pubsub_data = binary_message -> data();
    if (!pubsub_data.empty()) {
        cloud_event.set_binary_data(pubsub_data);
    }

    return cloud_event;
}

// __________ WRITE ___________

absl::StatusOr<std::unique_ptr<google::protobuf::Message>> PubsubBinder::WriteBinary(CloudEvent cloud_event) {
    // handle metadata
        // check size constraints pubsub
        // add ce prefix
        // add to pubsub attributes

    // handle data
        // base64 encode (if not binary)
        // place in pubsub data payload
    return absl::UnimplementedError("todo");
};

absl::StatusOr<std::unique_ptr<google::protobuf::Message>> PubsubBinder::WriteStructured(StructuredCloudEvent structured_cloud_event) {
    PubsubMessage pubsub_msg;
    
    // set datacontent type
    absl::StatusOr<std::string> formattostr_successful = FormatToStr(structured_cloud_event.GetCloudEventFormat());

    if (!formattostr_successful) {
        return formattostr_successful.status();
    }
    (*pubsub_msg.mutable_attributes())[pubsub_content_key_] = ce_contenttype_prefix_ + *formattostr_successful;

    // dump entire serialized in payload.
    pubsub_msg.set_data(structured_cloud_event.GetSerializedCloudEvent());

    // return unique pointer
    return std::unique_ptr<google::protobuf::Message>(&pubsub_msg);
};


} // binder
} // cloud_events
