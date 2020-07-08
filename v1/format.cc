#include "format.h"

namespace cloud_events {
namespace format {
using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;

// StructuredCloudEvent

StructuredCloudEvent::StructuredCloudEvent(CloudEventFormat format, std::string serialized_cloud_event)
    : format_(format), serialized_cloud_event_(serialized_cloud_event)
{}


CloudEventFormat StructuredCloudEvent::GetCloudEventFormat(){
    return format_;
}

std::string StructuredCloudEvent::GetSerializedCloudEvent() {
    return serialized_cloud_event_;
} 

// JsonMarshaller
absl::StatusOr<Json::Value> JsonMarshaller::PrintToJson(CloudEvent_CloudEventAttribute attr){
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
            return Json::Value(google::protobuf::util::TimeUtil::ToString(attr.ce_timestamp()));
        case CloudEvent_CloudEventAttribute::AttrOneofCase::ATTR_ONEOF_NOT_SET:
            return absl::InvalidArgumentError("Cloud Event metadata attribute not set.");
    }
    return absl::InvalidArgumentError("Cloud Event metadata attribute not recognized"); // should not be reached as all enums covered
}

absl::StatusOr<StructuredCloudEvent> JsonMarshaller::Serialize(CloudEvent cloud_event) {
    // handle data
    Json::Value root;
    switch (cloud_event.data_oneof_case()) {
        case CloudEvent::DataOneofCase::kBinaryData:
            root["data_base64"] = base64_encode(cloud_event.binary_data());
        case CloudEvent::DataOneofCase::kTextData:
            root["data"] = cloud_event.text_data();
        case CloudEvent::DataOneofCase::kProtoData:
            // TODO (#17): Handle CloudEvent Any in JsonMarshaller
            // Json::Value data_json_p = root["data"];
            // google::protobuf::Any data_proto = cloud_event.proto_data();
            // data_proto.UnpackTo(&data_msg);
            // google::protobuf::util::MessageToJsonString(, &data_json_p);
            return absl::UnimplementedError("protobuf::Any not supported yet.");
        default:
            break;
    }

    // handle metadata
    if (cloud_event.id().empty() || cloud_event.source().empty() || cloud_event.spec_version().empty(), cloud_event.type().empty()) {
        return absl::InvalidArgumentError("Required attribute in Cloud Event cannot be empty");
    }

    root["id"] = cloud_event.id();
    root["source"] = cloud_event.source();
    root["spec_version"] = cloud_event.spec_version();
    root["type"] = cloud_event.type();

    for (auto const& x : cloud_event.attributes()) {
        absl::StatusOr<Json::Value> json_printed = JsonMarshaller::PrintToJson(x.second);
        if (!json_printed) {
            return json_printed.status();
        } else {
            root[x.first] = json_printed.value();
        }
    }
    Json::StreamWriterBuilder builder;
    return StructuredCloudEvent(CloudEventFormat::JSON, Json::writeString(builder, root));
}

absl::StatusOr<CloudEvent> JsonMarshaller::Deserialize(std::string serialized_cloud_event) {
    Json::CharReaderBuilder builder;
    Json::CharReader * reader = builder.newCharReader();
    std::string errors;

    Json::Value root;   
    bool parsingSuccessful = reader->parse(serialized_cloud_event.c_str(), serialized_cloud_event.c_str() + serialized_cloud_event.size(), &root, &errors);
    if (!parsingSuccessful) {
        return absl::InvalidArgumentError(errors);
    }

    CloudEvent cloud_event;

    // handle data
    if (root.isMember("data") && root.isMember("base64_data")) {
        return absl::InvalidArgumentError("Provided input contains two data payloads and is an invalid serialization.");
    } else if (root.isMember("data")) {
        cloud_event.set_text_data(root["data"].asString());
    } else if (root.isMember("base64_data")) {
        cloud_event.set_binary_data(root["base64_data"].asString());
    }

    // handle metadata
    if (!root.isMember("id") || !root.isMember("source") || !root.isMember("spec_version") || !root.isMember("type")) { // using isMember to avoid creating null member, avoids quirk of JsonCpp
        return absl::InvalidArgumentError("Provided input is missing required Cloud Event attributes.");
    }

    for (auto const& member : root.getMemberNames()) {
        if (member == "id") {
            cloud_event.set_id(root[member].asString());
        } else if (member == "source") {
            cloud_event.set_source(root[member].asString());
        } else if (member == "spec_version") {
            cloud_event.set_spec_version(root[member].asString());
        } else if (member == "type") {
            cloud_event.set_type(root[member].asString());
        } else {
            CloudEvent_CloudEventAttribute attr;
            attr.set_ce_string(root[member].asString());
            (*cloud_event.mutable_attributes())[member] = attr;
        }
    }
    return cloud_event;
}

absl::StatusOr<CloudEvent> JsonMarshaller::Deserialize(StructuredCloudEvent structured_cloud_event) {
    return JsonMarshaller::Deserialize(structured_cloud_event.GetSerializedCloudEvent());
}

} // format
} // cloud_events
