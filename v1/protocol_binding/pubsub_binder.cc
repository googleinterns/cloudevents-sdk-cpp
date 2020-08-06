#include "pubsub_binder.h"

namespace cloudevents {
namespace binding {

using ::google::pubsub::v1::PubsubMessage;
using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;
using ::cloudevents::format::StructuredCloudEvent;
using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::formatter_util::FormatterUtil;
using ::cloudevents::cloudevents_util::CloudEventsUtil;

typedef absl::flat_hash_map<std::string, CloudEvent_CloudEventAttribute> CeAttrMap;

constexpr absl::string_view kPubsubContentKey = "content-type";

template <>
absl::StatusOr<bool> Binder<PubsubMessage>::InStructuredContentMode(PubsubMessage* pubsub_msg) {
    google::protobuf::Map<std::string,std::string> attrs;
    attrs = pubsub_msg -> attributes();
    auto ind = attrs.find(kPubsubContentKey.data());
    return (ind != attrs.end() && (ind -> second).rfind(kContenttypePrefix.data(), 0) == 0); 
}

template <>
absl::StatusOr<Format> Binder<PubsubMessage>::GetFormat(PubsubMessage* pubsub_msg) {
    // makes assumption that message is in structured content mode
    google::protobuf::Map<std::string,std::string> attrs;
    attrs = pubsub_msg -> attributes();
    auto ind = attrs.find(kPubsubContentKey.data());

    std::string format_str = (ind -> second).erase(0,strlen(kContenttypePrefix.data()));
    return FormatterUtil::DestringifyFormat(format_str);
}

template <>
absl::StatusOr<std::string> Binder<PubsubMessage>::GetPayload(PubsubMessage* pubsub_msg) {
    // get payload and base64 decode
    return base64::base64_decode(pubsub_msg -> data()); // no need to unpack due to matching return type
}

template <>
absl::StatusOr<CloudEvent> Binder<PubsubMessage>::UnbindBinary(PubsubMessage* pubsub_msg) {
    CloudEvent cloud_event;

    for (auto const& attr : pubsub_msg -> attributes()) {
        std::string key;
        if (attr.first == kPubsubContentKey.data()) {
            key = kContenttypeKey.data();
        } else if (attr.first.rfind(kMetadataPrefix.data(), 0) == 0){
            size_t len_prefix = strlen(kMetadataPrefix.data());
            key= attr.first.substr(len_prefix, std::string::npos);
        }
        CloudEventsUtil::SetMetadata(&cloud_event,key, attr.second);
    }

    std::string pubsub_data = pubsub_msg -> data();
    if (!pubsub_data.empty()) {
        absl::StatusOr<std::string> decoded = base64::base64_decode(pubsub_data);
        if (!decoded.ok()) {
            return decoded.status();
        }
        cloud_event.set_binary_data((*decoded));
    }
    
    if (!CloudEventsUtil::IsValid(cloud_event)) {
        return absl::InvalidArgumentError("Pubsub Message given does not contain a valid binary Cloud Event");
    }
    return cloud_event;
}

template <>
absl::StatusOr<PubsubMessage> Binder<PubsubMessage>::BindBinary(CloudEvent* cloud_event) {
    if (!CloudEventsUtil::IsValid(*cloud_event)) {
        return absl::InvalidArgumentError("Cloud Event given is not valid.");
    }

    PubsubMessage pubsub_msg;

    absl::StatusOr<CeAttrMap> attrs;
    attrs = CloudEventsUtil::GetMetadata(*cloud_event);
    if (!attrs.ok()) {
        return attrs.status();
    }

    for (auto const& attr : (*attrs)) {
        absl::StatusOr<std::string> val = CloudEventsUtil::StringifyCeType(attr.second);
        if (!val.ok()) {
            return val.status();
        }
        std::string key = kMetadataPrefix.data() + attr.first;
        (*pubsub_msg.mutable_attributes())[key] = (*val);
    }

    std::string data;
    switch (cloud_event -> data_oneof_case()) {
        case CloudEvent::DataOneofCase::kBinaryData:
            // cloud event spec uses base64 encoding for binary data as well 
            data = cloud_event -> binary_data();
            break;
        case CloudEvent::DataOneofCase::kTextData:
            absl::StatusOr<std::string> encoded;
            encoded = base64::base64_encode(cloud_event -> text_data());
            if (!encoded.ok()) {
                return encoded.status();
            }
            data = (*encoded);
            break;
        case CloudEvent::DataOneofCase::kProtoData:
            // TODO (#17): Handle CloudEvent Any in JsonFormatter
            return absl::UnimplementedError("protobuf::Any not supported yet.");
        case CloudEvent::DATA_ONEOF_NOT_SET:
            break;
    }

    pubsub_msg.set_data(data);
    return pubsub_msg;
}

template <>
absl::StatusOr<PubsubMessage> Binder<PubsubMessage>::BindStructured(StructuredCloudEvent* structured_ce) {
    PubsubMessage pubsub_msg;
    
    // set content type
    std::string format_str;
    format_str = FormatterUtil::StringifyFormat(structured_ce -> format);

    (*pubsub_msg.mutable_attributes())[kPubsubContentKey.data()] = kContenttypePrefix.data() + format_str;

    // dump entire serialized in payload.
    pubsub_msg.set_data(structured_ce -> serialization);

    return pubsub_msg;
}

} // binding
} // cloudevents