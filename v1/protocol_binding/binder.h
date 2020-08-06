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

template <class Message>
class Binder {
    // Template, each protocol should implement a concrete version of this.

    // In general built off of a template method design pattern.  
    // Main function methods are Unbind and Bind.

    // Bind takes a CloudEvent [and Format], or StructuredCloudEvent as input, and creates a XMessage as ouput
    // Unbind takes an XMessage as input, and creates a CloudEvent or StructuredCloudEvent.
    public:
        absl::StatusOr<Message> Bind(io::cloudevents::v1::CloudEvent cloud_event) {
            return BindBinary(cloud_event);
        }

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
            return BindStructured(*serialization);
        }

        absl::StatusOr<io::cloudevents::v1::CloudEvent> Unbind(Message* message) {
            // get content-mode of the message
            absl::StatusOr<bool> in_binary_content_mode; 
            in_binary_content_mode = InBinaryContentMode(message);
            if (!in_binary_content_mode.ok()) {
                return in_binary_content_mode.status();
            }

            if (*in_binary_content_mode) {
                return UnbindBinary(message);
            }

            absl::StatusOr<cloudevents::format::Format> get_format = GetFormat(message);
            if (!get_format.ok()) {
                return get_format.status();
            }

            absl::StatusOr<std::string> get_payload = GetPayload(message);
            if (!get_payload.ok()) {
                return get_payload.status();
            }

            absl::StatusOr<std::unique_ptr<cloudevents::format::Formatter>> get_formatter;
            get_formatter = cloudevents::formatter_util::FormatterUtil::GetFormatter(*get_format);
            if (!get_formatter.ok()) {
                return get_formatter.status();
            }

            cloudevents::format::StructuredCloudEvent structured_cloud_event;
            structured_cloud_event.format = (*get_format);
            structured_cloud_event.serialization = (*get_payload);
            absl::StatusOr<io::cloudevents::v1::CloudEvent> deserialization = (*get_formatter) -> Deserialize(structured_cloud_event);
            if (!deserialization.ok()){
                return deserialization.status();
            }
            return (*deserialization);
        }


    protected:
        // operations for Unbind that need to be overriden
        absl::StatusOr<bool> InStructuredContentMode(Message* message) {
            return absl::InternalError("Unimplemented operation");
        }
        absl::StatusOr<cloudevents::format::Format> GetFormat(Message* message) {
            return absl::InternalError("Unimplemented operation");
        }
        absl::StatusOr<std::string> GetPayload(Message* message) {
            return absl::InternalError("Unimplemented operation");
        }
        absl::StatusOr<io::cloudevents::v1::CloudEvent> UnbindBinary(Message* binary_message) {
            return absl::InternalError("Unimplemented operation");
        }

        // operations for Bind that need to be overriden
        absl::StatusOr<Message> BindBinary(io::cloudevents::v1::CloudEvent* cloud_event) {
            return absl::InternalError("Unimplemented operation");
        }
        absl::StatusOr<Message> BindStructured(cloudevents::format::StructuredCloudEvent* structured_ce) {
            return absl::InternalError("Unimplemented operation");
        }   

        // Constexpr keys used when binding/ unbinding Messages
        static inline constexpr absl::string_view kMetadataPrefix = "ce-";
        static inline constexpr absl::string_view kContenttypePrefix = "application/cloudevents+";
        static inline constexpr absl::string_view kContenttypeKey = "datacontenttype";
};

} // format
} // cloudevents

#endif // CLOUDEVENTSCPPSDK_V1_BINDING_BINDER_H
