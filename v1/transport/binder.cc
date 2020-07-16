#include "binder.h"

namespace cloud_events {
namespace binder {

using ::io::cloudevents::v1::CloudEvent;
using ::cloud_events::format::StructuredCloudEvent;
using ::cloud_events::format::CloudEventFormat;
using ::cloud_events::format::Marshaller;
using ::cloud_events::format::JsonMarshaller;
using ::google::protobuf::Message;

absl::StatusOr<std::unique_ptr<Marshaller>> Binder::GetMarshallerForFormat(CloudEventFormat format) const {
    switch (format) {
        case CloudEventFormat::UNFORMATTED:
            return absl::InvalidArgumentError("No cloud event format provided. Unformatted is not a format.");
        case CloudEventFormat::JSON:
            auto m = new JsonMarshaller;
            return std::unique_ptr<Marshaller>(m);
    }
    return absl::InternalError("Could not find marshaller for given format.");
};

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
    // get format of the message
    absl::StatusOr<CloudEventFormat> get_format_successful = this->GetFormat(message);
    if (!get_format_successful) {
        return get_format_successful.status();
    }
    CloudEventFormat format = get_format_successful.value();

    // parse message into [Structured]CloudEvent
    if (format == CloudEventFormat::UNFORMATTED) { // read from binary mode: protocol specific
        absl::StatusOr<io::cloudevents::v1::CloudEvent> read_binary_successful = ReadBinary(message);
        if (!read_binary_successful) {
            return read_binary_successful.status();
        } else {
            auto cloud_event = read_binary_successful.value();
            return std::unique_ptr<Message>(&cloud_event);
        }
    } 

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
