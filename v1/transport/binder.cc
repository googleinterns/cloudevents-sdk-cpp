#include "binder.h"

namespace cloud_events {
namespace binder {

using ::io::cloudevents::v1::CloudEvent;
using ::cloud_events::format::StructuredCloudEvent;
using ::cloud_events::format::CloudEventFormat;
using ::cloud_events::format::Marshaller;
using ::cloud_events::format::JsonMarshaller;
using ::google::protobuf::Message;

absl::StatusOr<std::unique_ptr<Marshaller>> Binder::GetMarshallerForFormat(CloudEventFormat format) {
    switch (format) {
        case CloudEventFormat::UNFORMATTED:
            return absl::InvalidArgumentError("No cloud event format provided. Unformatted is not a format.");
        case CloudEventFormat::JSON:
            auto m = new JsonMarshaller;
            return std::unique_ptr<Marshaller>(m);
    }
    return absl::InternalError("Could not find marshaller for given format.");
};

absl::StatusOr<std::unique_ptr<google::protobuf::Message>> WriteBinary(CloudEvent cloud_event) {
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

// absl::StatusOr<std::unique_ptr<Message>> Binder::Read(std::string message) {
//     absl::StatusOr<Marshaller> get_format_successful Binder::GetFormat(std::string message);
//     if (!get_format_successful) {
//         return get_format_successful.status();
//     }

//     if (get_format_successful.value() == CloudEventFormat::UNFORMATTED) {
//         return Binder::ReadBinary(std::string message);
//     } else {
//         get_marshaller_successful = Binder::GetMarshallerForFormat(format)
//         if (!get_marshaller_successful) {
//             return get_marshaller_successful.status();
//         }
//         return Binder::ReadStructured(get_marshaller_successful.value().Serialize(std::string message));
//     }
// }

} // format
} // cloud_events
