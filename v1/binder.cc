#include "binder.h"

namespace cloud_events {
namespace binder {

using ::cloud_events::format::CloudEventFormat;
using ::cloud_events::format::JsonMarshaller;

absl::StatusOr<Marshaller> Binder::GetMarshallerForFormat(CloudEventFormat format) {
    switch (format) {
        case CloudEventFormat::JSON:
            return JsonMarshaller();
        default:
            return absl::InternalError("Can't find Marshaller for given Cloud Event Format");
    }
};


absl::StatusOr<Message> Binder::Write(io::cloudevents::v1::CloudEvent cloud_event) {
    return Binder::WriteBinary(cloud_event);
}

absl::StatusOr<Message> Binder::Write(io::cloudevents::v1::CloudEvent cloud_event, cloud_events::format::CloudEventFormat format) {
    return Binder::WriteStructured(Binder::GetMarshallerForFormat(format).serialize(cloud_event));
}

absl::StatusOr<Message> Binder::Write(cloudevents::format::StructuredCloudEvent structured_cloud_event) {
    return Binder::WriteStructured(structured_cloud_event);
}

absl::StatusOr<Message> Binder::Read(std::string message) {
    absl::StatusOr<Marshaller> get_format_successful Binder::GetFormat(std::string message);
    if (!get_format_successful) {
        return get_format_successful.status();
    }

    if (get_format_successful.value() == CloudEventFormat::UNFORMATTED) {
        return Binder::ReadBinary(std::string message);
    } else {
        return Binder::ReadStructured(std::string message);
    }
}

} // format
} // cloud_events
