#include "binder.h"

namespace cloud_events {
namespace binder {

using ::cloud_events::format::CloudEventFormat;
using ::cloud_events::format::Marshaller;
using ::cloud_events::format::JsonMarshaller;
using ::google::protobuf::Message;

absl::StatusOr<std::unique_ptr<Marshaller>> Binder::GetMarshallerForFormat(CloudEventFormat format) {
    switch (format) {
        case CloudEventFormat::UNFORMATTED:
            return absl::Status();
        case CloudEventFormat::JSON:
            auto jm = new JsonMarshaller();
            auto m = static_cast<Marshaller*>(jm);
            return std::unique_ptr<Marshaller>(m);
    }
    return absl::InternalError("Could not find marshaller for given format.");
};

// absl::StatusOr<Message> Binder::Write(io::cloudevents::v1::CloudEvent cloud_event) {
//     return Binder::WriteBinary(cloud_event);
// }

// absl::StatusOr<Message> Binder::Write(io::cloudevents::v1::CloudEvent cloud_event, cloud_events::format::CloudEventFormat format) {
//     return Binder::WriteStructured(Binder::GetMarshallerForFormat(format).serialize(cloud_event));
// }
// absl::StatusOr<Message> Binder::Write(cloudevents::format::StructuredCloudEvent structured_cloud_event) {
//     return Binder::WriteStructured(structured_cloud_event);
// }
// absl::StatusOr<Message> Binder::Read(std::string message) {
//     absl::StatusOr<Marshaller> get_format_successful Binder::GetFormat(std::string message);
//     if (!get_format_successful) {
//         return get_format_successful.status();
//     }

//     if (get_format_successful.value() == CloudEventFormat::UNFORMATTED) {
//         return Binder::ReadBinary(std::string message);
//     } else {
//         return Binder::ReadStructured(std::string message);
//     }
// }

} // format
} // cloud_events
