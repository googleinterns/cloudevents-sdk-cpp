#ifndef JSON_MARSHALLER_H
#define JSON_MARSHALLER_H

#include "proto/cloud_event.pb.h"
#include "third_party/base64/base64.h"
#include "third_party/statusor/statusor.h"
#include <json/json.h>
#include <google/protobuf/any.pb.h>
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/time_util.h>

namespace cloud_events {
namespace format {

class JsonMarshaller: public Marshaller {
    private:
        absl::StatusOr<Json::Value> PrintToJson(io::cloudevents::v1::CloudEvent_CloudEventAttribute attr);
    public: 
        absl::StatusOr<StructuredCloudEvent> Serialize(io::cloudevents::v1::CloudEvent cloud_event) override;
        absl::StatusOr<io::cloudevents::v1::CloudEvent> Deserialize(std::string serialized_cloud_event) override;
};

#endif