#include "pubsub_binder.h"

namespace cloudevents {
namespace binding {

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
  cloudevents_absl::StatusOr<std::string> val_str =
    CloudEventsUtil::ToString(val);
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
  cloudevents_absl::StatusOr<std::string> encoded =
    cloudevents_base64::base64_encode(text_data);
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

// _____ Specializations for Unbind Binary _____

absl::Status PubsubBinder::UnbindMetadata(
    const PubsubMessage& pubsub_msg, CloudEvent& cloud_event) {
  for (auto const& attr : pubsub_msg.attributes()) {
    std::string key = attr.first;
    if (key == kPubsubContenttypeKey) {
        if (auto set_contenttype = CloudEventsUtil::SetContentType(attr.second,
          cloud_event); !set_contenttype.ok()) {
        return set_contenttype;
      }
    } else if (BinderUtil::StripMetadataPrefix(key).ok()){
      if (auto set_md = CloudEventsUtil::SetMetadata(key, attr.second,
          cloud_event); !set_md.ok()) {
        return set_md;
      }
    }
  }
  return absl::OkStatus();
}

absl::Status PubsubBinder::UnbindData(
    const PubsubMessage& pubsub_msg, CloudEvent& cloud_event) {
  // both CloudEvent.binary_data and Pubsub.payload uses base64 encoding
  cloud_event.set_text_data(
    cloudevents_base64::base64_decode(pubsub_msg.data()));
  return absl::OkStatus();
}

// _____ Specializations for Unbind Structured _____

cloudevents_absl::StatusOr<std::string> PubsubBinder::GetContentType(
    const PubsubMessage& pubsub_msg) {
  google::protobuf::Map<std::string, std::string> attrs =
    pubsub_msg.attributes();
  auto ind = attrs.find(kPubsubContenttypeKey);
  if (ind == attrs.end()) {
    return std::string("");
  }
  return ind -> second;
}

cloudevents_absl::StatusOr<std::string> PubsubBinder::GetPayload(
    const PubsubMessage& pubsub_msg) {
  // get payload and base64 decode
  return cloudevents_base64::base64_decode(pubsub_msg.data());
}

}  // namespace binding
}  // namespace cloudevents
