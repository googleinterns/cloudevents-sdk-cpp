#include "http_binder.h"

#include "third_party/statusor/statusor.h"
#include "proto/cloud_event.pb.h"
#include "v1/util/binder_util.h"

namespace cloudevents {
namespace binding {

using ::absl::Status;
using ::absl::OkStatus;
using ::cloudevents_absl::StatusOr;
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
Status HttpBinder<IsReq>::BindMetadata(const std::string& key,
    const CeAttr& val, message<IsReq, string_body>& http_msg) {
  StatusOr<std::string> val_str = CloudEventsUtil::ToString(val);
  if (!val_str.ok()) {
    return val_str.status();
  }
  http_msg.base().set(key, *val_str);
  return OkStatus();
}

template <bool IsReq>
Status HttpBinder<IsReq>::BindDataBinary(const std::string& bin_data,
    message<IsReq, string_body>& http_msg) {
  // spec states to place data into body as is
  http_msg.body() = bin_data;
  return OkStatus();
}

template <bool IsReq>
Status HttpBinder<IsReq>::BindDataText(const std::string& text_data,
    message<IsReq, string_body>& http_msg) {
  // spec states to place data into body as is
  http_msg.body() = text_data;
  return OkStatus();
}

}  // namespace binding
}  // namespace cloudevents
