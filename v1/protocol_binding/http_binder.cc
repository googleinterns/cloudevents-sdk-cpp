#include "http_binder.h"

#include "third_party/statusor/statusor.h"
#include "proto/cloud_event.pb.h"
#include "v1/util/binder_util.h"

namespace cloudevents {
namespace binding {

using ::io::cloudevents::v1::CloudEvent;
using ::cloudevents::cloudevents_util::CloudEventsUtil;
using ::cloudevents::binder_util::BinderUtil;
using ::boost::beast::http::message;
using ::boost::beast::http::string_body;

typedef io::cloudevents::v1::CloudEvent_CloudEventAttribute CeAttr;
typedef absl::flat_hash_map<std::string, CeAttr> CeAttrMap;

static const char kHttpContentKey[] = "Content-Type";
static const char kErrAmbiguousContentMode[] = "Given Http Message contains indicators for both Binary and Structured Content Mode.";

// _____ Operations used in Bind Binary _____

template <bool IsReq>
absl::Status HttpBinder<IsReq>::BindMetadata(const std::string& key,
    const CeAttr& val, message<IsReq, string_body>& http_msg) {
  cloudevents_absl::StatusOr<std::string> val_str =
    CloudEventsUtil::ToString(val);
  if (!val_str.ok()) {
    return val_str.status();
  }

  // Replace ce-datacontenttype 
  // https://github.com/cloudevents/spec/blob/v1.0/http-protocol-binding.md#311-http-content-type
  if (key == "ce-datacontenttype") {
    http_msg.base().set(kHttpContentKey, *val_str);
    return absl::OkStatus();
  }
  http_msg.base().set(key, *val_str);
  return absl::OkStatus();
}

template <bool IsReq>
absl::Status HttpBinder<IsReq>::BindDataBinary(const std::string& bin_data,
    message<IsReq, string_body>& http_msg) {
  // spec states to place data into body as is
  http_msg.body() = bin_data;
  return absl::OkStatus();
}

template <bool IsReq>
absl::Status HttpBinder<IsReq>::BindDataText(const std::string& text_data,
    message<IsReq, string_body>& http_msg) {
  // spec states to place data into body as is
  http_msg.body() = text_data;
  return absl::OkStatus();
}

// _____ Operations used in Bind Structured _____

template <bool IsReq>
absl::Status HttpBinder<IsReq>::BindContentType(const std::string& contenttype,
    message<IsReq, string_body>& http_msg) {
  http_msg.base().set(kHttpContentKey, contenttype);
  return absl::OkStatus();
}

template <bool IsReq>
absl::Status HttpBinder<IsReq>::BindDataStructured(const std::string& payload,
    message<IsReq, string_body>& http_msg) {
  http_msg.body() = payload;
  return absl::OkStatus();
}

// _____ Operations used in Unbind Binary _____

template <bool IsReq>
absl::Status HttpBinder<IsReq>::UnbindMetadata(
    const message<IsReq, string_body>& http_msg, CloudEvent& cloud_event) {
  for (auto it = http_msg.base().begin(); it != http_msg.base().end(); ++it) {
    std::string header_key = (*it).name_string().to_string();
    std::string header_val = (*it).value().to_string();
    if (header_key == kHttpContentKey) {
      if (auto set_contenttype = CloudEventsUtil::SetContentType(header_val,
          cloud_event); !set_contenttype.ok()) {
        return set_contenttype;
      }
    } else if (BinderUtil::StripMetadataPrefix(header_key).ok()) {
      if (auto set_metadata = CloudEventsUtil::SetMetadata(header_key,
          header_val, cloud_event); !set_metadata.ok()) {
        return set_metadata;
      }
    }
  }
  return absl::OkStatus();
}

template <bool IsReq>
absl::Status HttpBinder<IsReq>::UnbindData(
    const message<IsReq, string_body>& http_msg, CloudEvent& cloud_event) {
  if (!http_msg.body().empty()) {
    cloud_event.set_binary_data(http_msg.body());
  }
  return absl::OkStatus();
}

// _____ Operations used in Unbind Structured _____

template <bool IsReq>
cloudevents_absl::StatusOr<std::string> HttpBinder<IsReq>::GetContentType(
    const message<IsReq, string_body>& http_msg) {
  auto iter = http_msg.base().find(kHttpContentKey);
  if (iter == http_msg.base().end()) {
    // If kHttpContentKey not present, cannot be Structured-ContentMode
    // cloudevents_absl::StatusOr<> requires explicit typecasting
    return std::string("");
  }
  // kHttpContentKey and ce-datacontenttype should be mutually exclusive
  if (http_msg.base().find("ce-datacontenttype") != http_msg.base().end()) {
    return absl::InvalidArgumentError(kErrAmbiguousContentMode);
  }
  return std::string(iter->value());
}

template <bool IsReq>
cloudevents_absl::StatusOr<std::string> HttpBinder<IsReq>::GetPayload(
    const message<IsReq, string_body>& http_msg) {
  return http_msg.body();
}

// _____ explicit instantiations _____
template class HttpBinder<true>;
template class HttpBinder<false>;

}  // namespace binding
}  // namespace cloudevents
