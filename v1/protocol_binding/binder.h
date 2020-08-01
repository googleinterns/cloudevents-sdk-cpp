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
        absl::StatusOr<Message> Bind(io::cloudevents::v1::CloudEvent cloud_event);
        absl::StatusOr<Message> Bind(io::cloudevents::v1::CloudEvent cloud_event, cloudevents::format::Format format);
        absl::StatusOr<io::cloudevents::v1::CloudEvent> Unbind(Message message);
    protected:
        // virtual operations for Unbind
        virtual absl::StatusOr<cloudevents::format::Format> GetFormat(Message message) = 0;
        virtual absl::StatusOr<bool> InBinaryContentMode(Message message) = 0;
        virtual absl::StatusOr<std::string> GetPayload(Message message) = 0;
        virtual absl::StatusOr<io::cloudevents::v1::CloudEvent> UnbindBinary(Message binary_message) = 0;

        // virtual operations for Bind
        virtual absl::StatusOr<Message> BindBinary(io::cloudevents::v1::CloudEvent cloud_event) = 0;
        virtual absl::StatusOr<Message> BindStructured(cloudevents::format::StructuredCloudEvent structured_cloud_event) = 0;   
        
        // Constexpr keys used when binding/ unbinding Messages
        inline static constexpr absl::string_view kMetadataPrefix = "ce-";
        inline static constexpr absl::string_view kContenttypePrefix = "application/cloudevents+";
        inline static constexpr absl::string_view kContenttypeKey = "datacontenttype";
};

} // format
} // cloudevents

#endif // CLOUDEVENTSCPPSDK_V1_BINDING_BINDER_H
