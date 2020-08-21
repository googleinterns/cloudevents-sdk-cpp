#ifndef CLOUDEVENTSCPPSDK_V1_BINDING_BINDER_H
#define CLOUDEVENTSCPPSDK_V1_BINDING_BINDER_H

#include <google/protobuf/message.h>

#include "third_party/statusor/statusor.h"
#include "third_party/base64/base64.h"
#include "proto/cloud_event.pb.h"
#include "external/googleapis/google/pubsub/v1/pubsub.pb.h"
#include "v1/event_format/structured_cloud_event.h"
#include "v1/event_format/json_formatter.h"
#include "v1/util/formatter_util.h"
#include "v1/util/cloud_events_util.h"
#include "v1/util/binder_util.h"

namespace cloudevents {
namespace binding {

// Template for marshalling between protocol-specific Messages and CloudEvents.
// Each supported ProtocolBinding will implement template specialization 
// for the protected functions.
//
// User will interact with the public method functions Bind() and Unbind().
// Bind() will marhsal a CloudEvent to a protocol-specific Message.
// Unbind() will marshal a protocol-specific message to a CloudEvent.
// To create a StructuredContentMode Message, pass a EventFormat parameter to Bind().
// Code samples available in README.md.
template <typename Message>
class Binder {
 public:

 private:
  // The following operations are protocol-specific and
  // will be overriden for each supported ProtocolBinding

  // _____ Virtual Operations used in Bind Binary _____
 
  virtual absl::Status BindMetadata(const std::string& key,
      const io::cloudevents::v1::CloudEvent_CloudEventAttribute& val,
      Message& msg) = 0;

  virtual absl::Status BindDataBinary(const std::string& bin_data, Message& msg) = 0;
  virtual absl::Status BindDataText(const std::string& text_data, Message& msg) = 0;
  
  // _____ Virtual Operations used in Bind Structured _____

  virtual absl::Status BindContentType(const std::string& contenttype,
      Message& message) = 0;

  virtual absl::Status BindDataStructured(const std::string& payload,
      Message& message) = 0;

  // _____ Virtual Operations used in Unbind Binary _____

  virtual absl::Status UnbindMetadata(const Message& message,
      io::cloudevents::v1::CloudEvent& cloud_event) = 0;

  virtual absl::Status UnbindData(const Message& message,
      io::cloudevents::v1::CloudEvent& cloud_event) = 0;

  // _____ Virtual Operations used in Unbind Structured _____

  virtual cloudevents_absl::StatusOr<std::string> GetContentType(const Message& message) = 0;

  virtual cloudevents_absl::StatusOr<std::string> GetPayload(const Message& message) = 0;

};

}  // namespace binding
}  // namespace cloudevents

#endif  // CLOUDEVENTSCPPSDK_V1_BINDING_BINDER_H
