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
//
// To create a StructuredContentMode Message
// pass an additional EventFormat parameter to Bind().
// Code samples available in README.md.
template <typename Message>
class Binder {
 public:
  // Create Binary-ContentMode Message containing CloudEvent
  cloudevents_absl::StatusOr<Message> Bind(
      const io::cloudevents::v1::CloudEvent& cloud_event) {
    if (auto valid = cloudevents::cloudevents_util::CloudEventsUtil::IsValid(
        cloud_event); !valid.ok()) {
      return valid;
    }

    cloudevents_absl::StatusOr<absl::flat_hash_map<
      std::string, io::cloudevents::v1::CloudEvent_CloudEventAttribute>>
      attrs = cloudevents::cloudevents_util::CloudEventsUtil::
      GetMetadata(cloud_event);

    if (!attrs.ok()) {
      return attrs.status();
    }

    Message msg;
    for (auto const& attr : (*attrs)) {
      std::string key = attr.first;
      cloudevents::binder_util::BinderUtil::AddMetadataPrefix(key);
      if (auto bind_metadata = BindMetadata(key, attr.second, msg);
          !bind_metadata.ok()) {
        return bind_metadata;
      }
    }

    switch (cloud_event.data_oneof_case()) {
      case io::cloudevents::v1::CloudEvent::DataOneofCase::kBinaryData:
        if (auto set_bin_data = BindDataBinary(cloud_event.binary_data(), msg);
          !set_bin_data.ok()) {
          return set_bin_data;
        }
        break;
      case io::cloudevents::v1::CloudEvent::DataOneofCase::kTextData:
        if (auto set_text_data = BindDataText(cloud_event.text_data(), msg);
          !set_text_data.ok()) {
          return set_text_data;
        }
        break;
      case io::cloudevents::v1::CloudEvent::DataOneofCase::kProtoData:
        // TODO (#17): CloudEvent Any in JsonFormatter
        return absl::UnimplementedError("protobuf::Any not supported yet.");
      case io::cloudevents::v1::CloudEvent::DATA_ONEOF_NOT_SET:
        break;
    }

    return msg;
  }

  // Create Structured-ContentMode Message
  // containing Format-serialized CloudEvents
  cloudevents_absl::StatusOr<Message> Bind(
      const io::cloudevents::v1::CloudEvent& cloud_event,
      const cloudevents::format::Format& format) {
    if (auto valid = cloudevents::cloudevents_util::CloudEventsUtil::IsValid(
        cloud_event); !valid.ok()) {
      return valid;
    }

    cloudevents_absl::StatusOr<std::unique_ptr<cloudevents::format::Formatter>>
      get_formatter = cloudevents::formatter_util::FormatterUtil::
      GetFormatter(format);
    if (!get_formatter.ok()) {
      return get_formatter.status();
    }

    cloudevents_absl::StatusOr<std::unique_ptr<
      cloudevents::format::StructuredCloudEvent>> serialization =
      (*get_formatter)->Serialize(cloud_event);
    if (!serialization.ok()) {
      return serialization.status();
    }

    cloudevents_absl::StatusOr<std::string> format_str = cloudevents::
      formatter_util::FormatterUtil::FormatToStr((*serialization)->format);
    if (!format_str.ok()) {
        return format_str.status();
    }

    std::string contenttype = *format_str;
    cloudevents::binder_util::BinderUtil::AddContentTypePrefix(contenttype);

    Message msg;
    absl::Status set_contenttype = BindContentType(contenttype, msg);
    if (!set_contenttype.ok()) {
      return set_contenttype;
    }

    if (auto set_payload = BindDataStructured(
        (*serialization)->serialized_data, msg); !set_payload.ok()) {
      return set_payload;
    }

    return msg;
  }

  // Create CloudEvent from given Message
  cloudevents_absl::StatusOr<io::cloudevents::v1::CloudEvent> Unbind(
      const Message& message) {
    cloudevents_absl::StatusOr<std::string> contenttype =
      GetContentType(message);
    if (!contenttype.ok()) {
      return contenttype.status();
    }
    std::string format_str = *contenttype;
    if (!cloudevents::binder_util::BinderUtil::
        StripContentTypePrefix(format_str).ok()) {
      // Unbind Binary-ContentMode Message
      io::cloudevents::v1::CloudEvent cloud_event;
      if (auto unbind_metadata = UnbindMetadata(message, cloud_event);
          !unbind_metadata.ok()) {
        return unbind_metadata;
      }
      if (auto unbind_data = UnbindData(message, cloud_event);
          !unbind_data.ok()) {
        return unbind_data;
      }
      if (auto is_valid = cloudevents::cloudevents_util::CloudEventsUtil::
          IsValid(cloud_event); !is_valid.ok()) {
        return is_valid;
      }
      return cloud_event;
    }

    cloudevents_absl::StatusOr<cloudevents::format::Format> format =
      cloudevents::formatter_util::FormatterUtil::FormatFromStr(format_str);
    if (!format.ok()){
      return format.status();
    }

    cloudevents_absl::StatusOr<std::string> get_payload = GetPayload(message);
    if (!get_payload.ok()) {
      return get_payload.status();
    }

    cloudevents_absl::StatusOr<std::unique_ptr<cloudevents::format::Formatter>>
      get_formatter = cloudevents::formatter_util::FormatterUtil::
      GetFormatter(*format);
    if (!get_formatter.ok()) {
      return get_formatter.status();
    }

    cloudevents::format::StructuredCloudEvent structured_cloud_event;
    structured_cloud_event.format = *format;
    structured_cloud_event.serialized_data = *get_payload;

    cloudevents_absl::StatusOr<io::cloudevents::v1::CloudEvent> deserialization
      = (*get_formatter)->Deserialize(structured_cloud_event);
    if (!deserialization.ok()){
      return deserialization.status();
    }

    if (auto valid = cloudevents::cloudevents_util::CloudEventsUtil::IsValid(
        *deserialization); !valid.ok()) {
      return valid;
    }

    return *deserialization;
  }

 private:
  // The following operations are protocol-specific and
  // will be overriden for each supported ProtocolBinding

  // _____ Virtual Operations used in Bind Binary _____

  virtual absl::Status BindMetadata(const std::string& key,
      const io::cloudevents::v1::CloudEvent_CloudEventAttribute& val,
      Message& msg) = 0;

  virtual absl::Status BindDataBinary(const std::string& bin_data,
    Message& msg) = 0;

  virtual absl::Status BindDataText(const std::string& text_data,
    Message& msg) = 0;

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

  virtual cloudevents_absl::StatusOr<std::string> GetContentType(
    const Message& message) = 0;

  virtual cloudevents_absl::StatusOr<std::string> GetPayload(
    const Message& message) = 0;
};

}  // namespace binding
}  // namespace cloudevents

#endif  // CLOUDEVENTSCPPSDK_V1_BINDING_BINDER_H
