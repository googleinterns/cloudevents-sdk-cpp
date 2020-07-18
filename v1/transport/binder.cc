#include "binder.h"

namespace cloud_events {
namespace transport {

using ::io::cloudevents::v1::CloudEvent;
using ::cloud_events::format::StructuredCloudEvent;
using ::cloud_events::format::CloudEventFormat;
using ::cloud_events::format::Marshaller;
using ::cloud_events::format::JsonMarshaller;
using ::google::protobuf::Message;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;

const char* Binder::ce_attr_prefix_ = "ce-";
const char* Binder::ce_contenttype_prefix_ = "application/cloudevents+";


absl::StatusOr<CloudEventFormat> Binder::StrToFormat(std::string format_str) const {
    if (format_str == "json") {
        return CloudEventFormat::JSON;
    }
    return absl::InvalidArgumentError("The given format is not currently supported by the SDK.");
}

absl::StatusOr<std::string> Binder::FormatToStr(CloudEventFormat format) const {
    switch (format) {
        case CloudEventFormat::JSON:
            return "json";
        case CloudEventFormat::UNFORMATTED:
            return absl::InternalError("A request was made for a stringified format on an unformatted cloud event.");
    }
}

absl::StatusOr<std::unique_ptr<Marshaller>> Binder::GetMarshallerForFormat(CloudEventFormat format) const {
    switch (format) {
        case CloudEventFormat::UNFORMATTED:
            return absl::InvalidArgumentError("No cloud event format provided. Unformatted is not a format.");
        case CloudEventFormat::JSON:
            auto m = new JsonMarshaller;
            return std::unique_ptr<Marshaller>(m);
    }
    return absl::InternalError("Could not find marshaller for given format.");
}


absl::StatusOr<std::string> CeTypeToString(io::cloudevents::v1::CloudEvent_CloudEventAttribute attr) {
    // std::string protocol = "https?://(www.)?[-a-zA-Z0-9@:%._+~#=]{2,256}.[a-z]{2,4}b([-a-zA-Z0-9@:%_+.~#?&//=]*)";
    // std::regex url_regex(pattern);
    switch (attr.attr_oneof_case()) {
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeBoolean:
            return std::string(attr.ce_boolean() ? "true" : "false"); // statusOr requires explicit type conversion (?)
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeInteger:
            return std::to_string(attr.ce_integer()); // skipping validity checks as protobuf generates int32 for sfixed32
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeString:
            // TODO (Michelle): Handle Unicode
            return absl::UnimplementedError("Gotta learn unicode first");
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeBinary:
            return base64::base64_encode(attr.ce_binary());
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeUri:
            // if(!regex_match(attr.ce_uri(), url_regex)) {
            //     break;
            // }
            // return attr.ce_uri();
            return absl::UnimplementedError("Looking for a more verified solution.");
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeUriReference:
            return absl::UnimplementedError("Gotta learn regex first");
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeTimestamp:
            return google::protobuf::util::TimeUtil::ToString(attr.ce_timestamp()); // take advantage of protobuf using RFC3339 representation
        case CloudEvent_CloudEventAttribute::AttrOneofCase::ATTR_ONEOF_NOT_SET:
            return absl::InvalidArgumentError("Cloud Event metadata attribute not set.");
    }
    return absl::InvalidArgumentError("Cloud Event attribute provided is invalid."); // catch all when any constraint unmet
}

absl::StatusOr<std::unique_ptr<Message>> Binder::Write(CloudEvent cloud_event) {
    return WriteBinary(cloud_event);
}

absl::StatusOr<std::unique_ptr<Message>> Binder::Write(CloudEvent cloud_event, CloudEventFormat format) {
    auto get_marshaller_successful = Binder::GetMarshallerForFormat(format);
    if (!get_marshaller_successful) {
        return get_marshaller_successful.status();
    }
    absl::StatusOr<StructuredCloudEvent> serialization_successful = get_marshaller_successful.value() -> Serialize(cloud_event);
    if (!serialization_successful) {
        return serialization_successful.status();
    }
    return WriteStructured(serialization_successful.value());
}

absl::StatusOr<std::unique_ptr<Message>> Binder::Write(StructuredCloudEvent structured_cloud_event) {
    return WriteStructured(structured_cloud_event);
}

absl::StatusOr<std::unique_ptr<Message>> Binder::Read(google::protobuf::Message* message, bool deserialize) const {
    // get content-mode of the message
    absl::StatusOr<CloudEventFormat> get_format_successful = this->GetFormat(message);
    if (!get_format_successful) {
        return get_format_successful.status();
    }
    CloudEventFormat format = get_format_successful.value();

    // if binary content-mode, parse directly
    if (format == CloudEventFormat::UNFORMATTED) { // read from binary mode: protocol specific
        absl::StatusOr<io::cloudevents::v1::CloudEvent> read_binary_successful = ReadBinary(message);
        if (!read_binary_successful) {
            return read_binary_successful.status();
        } else {
            auto cloud_event = read_binary_successful.value();
            return std::unique_ptr<Message>(&cloud_event);
        }
    } 

    // if structured content-mode, get payload and unmarshal
    absl::StatusOr<std::string> get_payload_successful = this->GetPayload(message);

    if (!get_payload_successful) {
        return get_payload_successful.status();
    }

    std::string payload = get_payload_successful.value();

    if (deserialize) {
        absl::StatusOr<std::unique_ptr<cloud_events::format::Marshaller>> get_marshaller_successful = Binder::GetMarshallerForFormat(format);
        if (!get_marshaller_successful) {
            return get_marshaller_successful.status();
        }
        auto structured_cloud_event = StructuredCloudEvent(format, payload);
        absl::StatusOr<CloudEvent> deserialize_successful = get_marshaller_successful.value() -> Deserialize(structured_cloud_event);
        if (!deserialize_successful){
            return deserialize_successful.status();
        }
        return std::unique_ptr<Message>(&deserialize_successful.value());

    } else {
        // TODO (Michelle): Need to rewrite structured cloud event to be a proto for this to work...
        // StructuredCloudEvent structured_cloud_event = StructuredCloudEvent(format, payload);
        // auto m = static_cast<Message*>(structured_cloud_event);
        // return std::unique_ptr<Message>(m);
        return absl::UnimplementedError("retaining structured payload is not yet supported.");
    }
    return absl::UnimplementedError("todo");
}

} // format
} // cloud_events
