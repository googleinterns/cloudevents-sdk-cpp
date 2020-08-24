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

}  // namespace binding
}  // namespace cloudevents
