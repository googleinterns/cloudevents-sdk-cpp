#include "binder.h"

#include "v1/util/formatter_util.h"
namespace cloudevents {
namespace binding {

using ::io::cloudevents::v1::CloudEvent;
using ::cloudevents::format::StructuredCloudEvent;
using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::formatter_util::FormatterUtil;
// using ::cloudevents::format::Formatter;
// using ::cloudevents::format::JsonFormatter;
// using ::google::protobuf::Message;
// using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;

template <class Message>
absl::StatusOr<Message> Binder<Message>::Bind(CloudEvent cloud_event) {
    return BindBinary(cloud_event);
}

template <class Message>
absl::StatusOr<Message> Binder<Message>::Bind(CloudEvent cloud_event, Format format) {
    absl::StatusOr<std::unique_ptr<Formatter>> get_formatter;
    get_formatter = FormatterUtil::GetFormatter(format);
    if (!get_formatter.ok()) {
        return get_formatter.status();
    }
    absl::StatusOr<StructuredCloudEvent> serialization;
    serialization = (*get_formatter) -> Serialize(cloud_event);
    if (!serialization.ok()) {
        return serialization.status();
    }
    return BindStructured(*serialization);
}

template <class Message>
absl::StatusOr<CloudEvent> Binder<Message>::Unbind(Message message) {
    // get content-mode of the message
    absl::StatusOr<bool> in_binary_content_mode; 
    in_binary_content_mode = InBinaryContentMode(message);
    if (!in_binary_content_mode.ok()) {
        return in_binary_content_mode.status();
    }

    if (*in_binary_content_mode) {
        return UnbindBinary(message);
    }

    absl::StatusOr<Format> get_format = GetFormat(message);
    if (!get_format.ok()) {
        return get_format.status();
    }

    absl::StatusOr<std::string> get_payload = GetPayload(message);
    if (!get_payload.ok()) {
        return get_payload.status();
    }

    absl::StatusOr<std::unique_ptr<Formatter>> get_formatter;
    get_formatter = FormatterUtil::GetFormatter(*get_format);
    if (!get_formatter.ok()) {
        return get_formatter.status();
    }

    StructuredCloudEvent structured_cloud_event;
    structured_cloud_event.format = (*get_format);
    structured_cloud_event.serialization = (*get_payload);
    absl::StatusOr<CloudEvent> deserialization = (*get_formatter) -> Deserialize(structured_cloud_event);
    if (!deserialization.ok()){
        return deserialization.status();
    }
    return (*deserialization);
}

 
//template void Binder<PubsubMessage>();

} // binding
} // cloudevents