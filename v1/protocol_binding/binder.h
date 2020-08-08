#ifndef CLOUDEVENTSCPPSDK_V1_BINDING_BINDER_H
#define CLOUDEVENTSCPPSDK_V1_BINDING_BINDER_H

#include <memory>
#include <regex> 

#include <google/protobuf/message.h>

#include "third_party/statusor/statusor.h"
#include "third_party/base64/base64.h"
#include "proto/cloud_event.pb.h"
#include "external/googleapis/google/pubsub/v1/pubsub.pb.h"
#include "v1/event_format/structured_cloud_event.h"
#include "v1/event_format/json_formatter.h"
#include "v1/util/formatter_util.h"
#include "v1/util/cloud_events_util.h"

namespace cloudevents {
namespace binding {

/* 
 * Template for marshalling between protocol-specific Messages and CloudEvents.
 * Each supported ProtocolBinding will implement template specialization 
 * for the protected functions.
 *
 * User will interact with the public method functions Bind() and Unbind().
 * Bind() will marhsal a CloudEvent to a protocol-specific Message.
 * Unbind() will marshal a protocol-specific message to a CloudEvent.
 * To create a StructuredContentMode Message, pass a EventFormat parameter to Bind().
 * Code samples available in README.md.
 */
template <class Message>
class Binder {
    public:
        // Create BinaryContentMode Message containing CloudEvent
        absl::StatusOr<Message> Bind(io::cloudevents::v1::CloudEvent cloud_event) {
            return BindBinary(cloud_event);
        }

        // Create StructuredContentMode Message containing Format-serialized CloudEvent  
        absl::StatusOr<Message> Bind(io::cloudevents::v1::CloudEvent cloud_event, cloudevents::format::Format format) {
            absl::StatusOr<std::unique_ptr<cloudevents::format::Formatter>> get_formatter;
            get_formatter = cloudevents::formatter_util::FormatterUtil::GetFormatter(format);
            if (!get_formatter.ok()) {
                return get_formatter.status();
            }
            
            absl::StatusOr<cloudevents::format::StructuredCloudEvent> serialization;
            serialization = (*get_formatter) -> Serialize(cloud_event);
            if (!serialization.ok()) {
                return serialization.status();
            }

            Message msg;
            std::string contenttype = kContenttypePrefix.data();
            contenttype += cloudevents::formatter_util::FormatterUtil::StringifyFormat(
                (*serialization).format);

            absl::Status set_contenttype = SetContentType(msg, contenttype);
            if (!set_contenttype.ok()) {
                return set_contenttype;
            }

            absl::Status set_payload = SetPayload(msg, (*serialization).serialization);
            if (!set_payload.ok()) {
                return set_payload;
            }
        
            return msg;
        }

        // Create CloudEvent from given Message
        absl::StatusOr<io::cloudevents::v1::CloudEvent> Unbind(Message& message) {
            absl::StatusOr<std::string> contenttype = GetContentType(message);
            if (!contenttype.ok()) {
                return contenttype.status();
            }
            if ((*contenttype).empty() || 
                    (*contenttype).rfind(kContenttypePrefix.data(), 0) != 0) {
                return UnbindBinary(message);
            }

            std::string format_str = (*contenttype).erase(
                0, strlen(kContenttypePrefix.data()));

            absl::StatusOr<cloudevents::format::Format> format;
            format = cloudevents::formatter_util::
                FormatterUtil::DestringifyFormat(format_str);
            if (!format.ok()){
                return format.status();
            }

            absl::StatusOr<std::string> get_payload = GetPayload(message);

            if (!get_payload.ok()) {
                return get_payload.status();
            }

            absl::StatusOr<std::unique_ptr<cloudevents::format::Formatter>> get_formatter;
            get_formatter = cloudevents::formatter_util::FormatterUtil::GetFormatter(*format);
            if (!get_formatter.ok()) {
                return get_formatter.status();
            }

            cloudevents::format::StructuredCloudEvent structured_cloud_event;
            structured_cloud_event.format = (*format);
            structured_cloud_event.serialization = (*get_payload);

            absl::StatusOr<io::cloudevents::v1::CloudEvent> deserialization = (*get_formatter) -> Deserialize(structured_cloud_event);
            if (!deserialization.ok()){
                return deserialization.status();
            }
            return (*deserialization);
        }


    // The following operations are protocol-specific and
    // will be overriden for each supported ProtocolBinding
    protected:
        absl::StatusOr<std::string> GetContentType(Message& message);
        
        absl::StatusOr<std::string> GetPayload(Message& message) {
            return absl::InternalError("Unimplemented operation");
        }

        // Marshals a BinaryContentMode message into a CloudEvent
        absl::StatusOr<io::cloudevents::v1::CloudEvent> UnbindBinary(Message& binary_message) {
            return absl::InternalError("Unimplemented operation");
        }

        // _____ Operations used in Bind _____

        absl::Status SetContentType(Message& message, std::string contenttype) {
            return absl::InternalError("Unimplemented operation");
        }

        absl::Status SetPayload(Message& message, std::string payload) {
            return absl::InternalError("Unimplemented operation");
        }

        // Marshals a CloudEvent into a BinaryContentMode message
        absl::StatusOr<Message> BindBinary(io::cloudevents::v1::CloudEvent& cloud_event) {
            return absl::InternalError("Unimplemented operation");
        }

        // Constexpr keys used accross ProtocolBindings
        static inline constexpr absl::string_view kMetadataPrefix = "ce-";
        static inline constexpr absl::string_view kContenttypePrefix = "application/cloudevents+";
        static inline constexpr absl::string_view kContenttypeKey = "datacontenttype";
};

} // format
} // cloudevents

#endif // CLOUDEVENTSCPPSDK_V1_BINDING_BINDER_H
