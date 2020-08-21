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

}  // namespace binding
}  // namespace cloudevents
