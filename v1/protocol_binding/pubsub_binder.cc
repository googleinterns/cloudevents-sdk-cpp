#include "pubsub_binder.h"

#include "v1/util/binder_util.h"
#include "v1/util/cloud_events_util.h"

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
  if (!bin_data.empty()) {
    pubsub_msg.set_data(bin_data);
  }
  return absl::OkStatus();
}

absl::Status PubsubBinder::BindDataText(const std::string& text_data,
    PubsubMessage& pubsub_msg) {
  if (!text_data.empty()) {
    pubsub_msg.set_data(text_data);
  }
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
  if (!pubsub_msg.data().empty()) {
    cloud_event.set_binary_data(pubsub_msg.data());
  }
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
  return ind->second;
}

cloudevents_absl::StatusOr<std::string> PubsubBinder::GetPayload(
    const PubsubMessage& pubsub_msg) {
  // get payload
  return pubsub_msg.data();
}

}  // namespace binding
}  // namespace cloudevents
