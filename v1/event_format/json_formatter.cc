#include "json_formatter.h"

#include <google/protobuf/any.pb.h>
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/time_util.h>

#include "v1/util/cloud_events_util.h"

namespace cloudevents {
namespace format {

using ::io::cloudevents::v1::CloudEvent;
using ::cloudevents::cloudevents_util::CloudEventsUtil;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;
using ::google::protobuf::util::TimeUtil;

typedef absl::flat_hash_map<std::string, CloudEvent_CloudEventAttribute> CeAttrMap;

absl::StatusOr<Json::Value> JsonFormatter::PrintToJson(CloudEvent_CloudEventAttribute attr){    
    switch (attr.attr_oneof_case()) {
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeBoolean:
            return Json::Value(attr.ce_boolean());
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeInteger:
            return Json::Value(attr.ce_integer());
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeString:
            return Json::Value(attr.ce_string());
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeBinary:
            return Json::Value(attr.ce_binary());
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeUri:
            return Json::Value(attr.ce_uri());
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeUriReference:
            return Json::Value(attr.ce_uri_reference());
        case CloudEvent_CloudEventAttribute::AttrOneofCase::kCeTimestamp:
            return Json::Value(TimeUtil::ToString(attr.ce_timestamp()));
        case CloudEvent_CloudEventAttribute::AttrOneofCase::ATTR_ONEOF_NOT_SET:
            return absl::InvalidArgumentError("Cloud Event metadata attribute not set.");
    }
    return absl::InternalError("A CloudEventAttribute type has not been handled in PrintToJson().");
}

absl::StatusOr<StructuredCloudEvent> JsonFormatter::Serialize(CloudEvent cloud_event) {
    // validate CloudEvent by ensuring all required metadata is present
    if (!CloudEventsUtil::IsValid(cloud_event)) {
        return absl::InvalidArgumentError("Required attribute in Cloud Event cannot be empty");
    }

    absl::StatusOr<CeAttrMap> attrs;
    attrs = CloudEventsUtil::GetMetadata(cloud_event);
    if (!attrs.ok()) {
        return attrs.status();
    }

    Json::Value root;
    for (auto const& attr : (*attrs)) {
        absl::StatusOr<Json::Value> json_printed;
        json_printed = JsonFormatter::PrintToJson(attr.second);
        if (!json_printed.ok()) {
            return json_printed.status();
        }
        root[attr.first] = *json_printed;
    }

    switch (cloud_event.data_oneof_case()) {
        case CloudEvent::DataOneofCase::kBinaryData:
            root[kBinaryDataKey.data()] = cloud_event.binary_data();
            break;
        case CloudEvent::DataOneofCase::kTextData:
            root[kJsonDataKey.data()] = cloud_event.text_data();
            break;
        case CloudEvent::DataOneofCase::kProtoData:
            // TODO (#17): Handle CloudEvent Any in JsonFormatter
            return absl::UnimplementedError("protobuf::Any not supported yet.");
        case CloudEvent::DATA_ONEOF_NOT_SET:
            break;
    }

    // Write JSON serialization as std::string
    Json::StreamWriterBuilder builder;
    StructuredCloudEvent structured_ce;
    structured_ce.format = Format::kJson;
    structured_ce.serialization = Json::writeString(builder, root);
    return structured_ce;
}

absl::StatusOr<CloudEvent> JsonFormatter::Deserialize(StructuredCloudEvent structured_cloud_event) {
    // Validate that this is the right format to be handled by this object
    if (structured_cloud_event.format != Format::kJson) {
        return absl::InternalError("This structured cloud event is not JSON formatted and should not be handled by a Json Formatter.");
    }

    // Create a Json::Value from the serialization string
    std::string serialization = structured_cloud_event.serialization;
    Json::CharReaderBuilder builder;
    Json::CharReader * reader = builder.newCharReader();
    std::string errors;
    Json::Value root;   

    bool parsingSuccessful = reader->parse(
        serialization.c_str(), 
        serialization.c_str() + serialization.size(), 
        &root, 
        &errors);
        
    if (!parsingSuccessful) {
        return absl::InvalidArgumentError(errors);
    }

    CloudEvent cloud_event;
    
    // TODO (#39): Should we try to infer CE Type from serialization?
    for (auto const& member : root.getMemberNames()) {
        CloudEventsUtil::SetMetadata(&cloud_event, member, root[member].asString());
    }

    if (root.isMember(kJsonDataKey.data()) && root.isMember(kBinaryDataKey.data())) {
        return absl::InvalidArgumentError("Provided input contains two data payloads and is an invalid serialization.");
    } else if (root.isMember(kJsonDataKey.data())) {
        cloud_event.set_text_data(root[kJsonDataKey.data()].asString());
    } else if (root.isMember(kBinaryDataKey.data())) {
        cloud_event.set_binary_data(root[kBinaryDataKey.data()].asString());
    }

    if (!CloudEventsUtil::IsValid(cloud_event)) {
        return absl::InvalidArgumentError("The serialization does not contain a valid CloudEvent");
    }

    return cloud_event;
}

} // format
} // cloudevents
