#ifndef CLOUDEVENTCPPSDK_V1_EVENTFORMAT_JSONFORMATTER_H_
#define CLOUDEVENTCPPSDK_V1_EVENTFORMAT_JSONFORMATTER_H_

#include <json/json.h>

#include "proto/cloud_event.pb.h"
#include "third_party/statusor/statusor.h"
#include "v1/event_format/formatter.h"
#include "v1/event_format/structured_cloud_event.h"

namespace cloudevents {
namespace format {

/*
 * Implementation of Formatter for JSON EventFormat
 */
class JsonFormatter: public Formatter {
 public:
  // Create Json-formatted serialization from CloudEvent
  absl::StatusOr<std::unique_ptr<StructuredCloudEvent>> Serialize(
    const io::cloudevents::v1::CloudEvent& cloud_event) override;

//   // Create CloudEvent from Json-formatted serialization
//   absl::StatusOr<io::cloudevents::v1::CloudEvent> Deserialize(
//     const StructuredCloudEvent& structured_ce) override;

 private:
//   // Convert from CE to JSON Type System according to
//   // Relies on the overloaded constructor for Json::Value in jsoncpp
//   // https://github.com/cloudevents/spec/blob/master/json-format.md#22-type-system-mapping
//   absl::StatusOr<Json::Value> PrintToJson(
//     const io::cloudevents::v1::CloudEvent_CloudEventAttribute& attr);
};

}  // namespace format
}  // namespace cloudevents

#endif  // CLOUDEVENTCPPSDK_V1_EVENTFORMAT_JSONFORMATTER_H_
