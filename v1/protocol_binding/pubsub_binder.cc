#include "pubsub_binder.h"

namespace cloudevents {
namespace binding {

using ::cloudevents_absl::StatusOr;
using ::cloudevents_base64::base64_encode;
using ::cloudevents_base64::base64_decode;
using ::google::pubsub::v1::PubsubMessage;
using ::io::cloudevents::v1::CloudEvent;
using ::cloudevents::binder_util::BinderUtil;
using ::cloudevents::cloudevents_util::CloudEventsUtil;

typedef io::cloudevents::v1::CloudEvent_CloudEventAttribute CeAttr;
typedef absl::flat_hash_map<std::string, CeAttr> CeAttrMap;

inline static const std::string kPubsubContenttypeKey = "content-type";

// _____ Specializations for Bind Binary _____

absl::Status PubsubBinder::BindMetadata(const std::string& key,
    const CeAttr& val, PubsubMessage& pubsub_msg) {
  StatusOr<std::string> val_str = CloudEventsUtil::ToString(val);
  if (!val_str.ok()) {
    return val_str.status();
  }
  (*pubsub_msg.mutable_attributes())[key] = *val_str;
  return absl::OkStatus();
}

absl::Status PubsubBinder::BindDataBinary(const std::string& bin_data,
    PubsubMessage& pubsub_msg) {
  // both CloudEvent.data and pubsub_msg.data are base64 encoded
  pubsub_msg.set_data(bin_data);
  return absl::OkStatus();
}

absl::Status PubsubBinder::BindDataText(const std::string& text_data,
    PubsubMessage& pubsub_msg) {
  StatusOr<std::string> encoded = base64_encode(text_data);
  if (!encoded.ok()) {
    return encoded.status();
  }
  pubsub_msg.set_data(*encoded);
  return absl::OkStatus();
}

// _____ Specializations for Bind Structured _____

absl::Status PubsubBinder::BindContentType(
    const std::string& contenttype, PubsubMessage& pubsub_msg) {
  (*pubsub_msg.mutable_attributes())[kPubsubContenttypeKey] = contenttype;
  return absl::OkStatus();
}

absl::Status PubsubBinder::BindDataStructured(
    const std::string& payload, PubsubMessage& pubsub_msg) {
  pubsub_msg.set_data(payload);
  return absl::OkStatus();
}

}  // namespace binding
}  // namespace cloudevents
