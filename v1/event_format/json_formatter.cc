#include "json_formatter.h"

#include <google/protobuf/any.pb.h>
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/time_util.h>

#include "v1/util/cloud_events_util.h"

namespace cloudevents {
namespace format {

using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;
using ::google::protobuf::util::TimeUtil;
using ::cloudevents::cloudevents_util::CloudEventsUtil;

typedef absl::flat_hash_map<std::string, CloudEvent_CloudEventAttribute>
  CeAttrMap;

// Attribute keys
constexpr char kJsonBinaryKey[] = "data_base64";
constexpr char kJsonTextKey[] = "data";

// Error statuses
constexpr char kErrProtobufAny[] = "protobuf::Any not supported yet.";
constexpr char kErrNotJson[] = "The given serialized data should not be handled by JsonFormatter because it is not JSON-formatted.";
constexpr char kErrTwoPayloads[] = "The given serialized data is invalid because it contains two data payloads.";

}  // namespace format
}  // namespace cloudevents
