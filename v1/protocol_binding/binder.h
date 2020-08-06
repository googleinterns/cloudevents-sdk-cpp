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
};

} // format
} // cloudevents

#endif // CLOUDEVENTSCPPSDK_V1_BINDING_BINDER_H
