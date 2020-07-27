#ifndef CLOUDEVENTCPPSDK_V1_EVENTFORMAT_JSONFORMATTER_H_
#define CLOUDEVENTCPPSDK_V1_EVENTFORMAT_JSONFORMATTER_H_

#include "v1/event_format/formatter.h"
#include "v1/event_format/structured_cloud_event.h"

#include "proto/cloud_event.pb.h"

#include "third_party/statusor/statusor.h"

#include <json/json.h>

#include <google/protobuf/any.pb.h>
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/time_util.h>

namespace cloudevents {
namespace format {

/*
 * Implementation of Formatter for JSON EventFormat
 */
class JsonFormatter: public Formatter {
    public: 
        // Create Json-formatted serialization from CloudEvent
        absl::StatusOr<StructuredCloudEvent> Serialize(io::cloudevents::v1::CloudEvent cloud_event) override;

        // Create CloudEvent from Json-formatted serialization
        absl::StatusOr<io::cloudevents::v1::CloudEvent> Deserialize(StructuredCloudEvent) override;

    private:
        // Convert from CE to JSON Type System according to https://github.com/cloudevents/spec/blob/master/json-format.md#22-type-system-mapping
        // Relies mostly on the overloaded constructor for Json::Value in jsoncpp
        absl::StatusOr<Json::Value> PrintToJson(io::cloudevents::v1::CloudEvent_CloudEventAttribute attr);
};

} // format
} // cloudevents

#endif //CLOUDEVENTCPPSDK_V1_EVENTFORMAT_JSONFORMATTER_H_
