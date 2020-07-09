#ifndef CLOUDEVENTS_FORMAT_H
#define CLOUDEVENTS_FORMAT_H

#include "proto/cloud_event.pb.h"
#include "third_party/statusor/statusor.h"
#include <json/json.h>
#include <google/protobuf/any.pb.h>
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/time_util.h>

namespace cloud_events {
namespace format {

enum CloudEventFormat {UNFORMATTED, JSON};

class StructuredCloudEvent {
    private:
        CloudEventFormat format_;
        std::string serialized_cloud_event_;
    public:
        StructuredCloudEvent(CloudEventFormat format, std::string serialized_cloud_event);
        CloudEventFormat GetCloudEventFormat();
        std::string GetSerializedCloudEvent();
};

class Marshaller {
    // interface for stand-alone format marshallers
    public:
        virtual absl::StatusOr<StructuredCloudEvent> Serialize(io::cloudevents::v1::CloudEvent cloud_event) = 0;
        virtual absl::StatusOr<io::cloudevents::v1::CloudEvent> Deserialize(std::string serialized_cloud_event) = 0;
        virtual absl::StatusOr<io::cloudevents::v1::CloudEvent> Deserialize(StructuredCloudEvent structured_cloud_event) = 0;
};

class JsonMarshaller: public Marshaller {
    private:
        absl::StatusOr<Json::Value> PrintToJson(io::cloudevents::v1::CloudEvent_CloudEventAttribute attr);
    public: 
        absl::StatusOr<StructuredCloudEvent> Serialize(io::cloudevents::v1::CloudEvent cloud_event) override;
        absl::StatusOr<io::cloudevents::v1::CloudEvent> Deserialize(std::string serialized_cloud_event) override;
        absl::StatusOr<io::cloudevents::v1::CloudEvent> Deserialize(StructuredCloudEvent structured_cloud_event) override;
};

} // format
} // cloud_events

#endif
