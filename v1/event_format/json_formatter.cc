#include "json_formatter.h"

#include <google/protobuf/any.pb.h>
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/time_util.h>

namespace cloudevents {
namespace format {

using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;

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
            return Json::Value(google::protobuf::util::TimeUtil::ToString(attr.ce_timestamp()));
        case CloudEvent_CloudEventAttribute::AttrOneofCase::ATTR_ONEOF_NOT_SET:
            return absl::InvalidArgumentError("Cloud Event metadata attribute not set.");
    }
    return absl::InternalError("A CloudEventAttribute type has not been handled in PrintToJson().");
}

absl::StatusOr<StructuredCloudEvent> JsonFormatter::Serialize(CloudEvent cloud_event) {
    // validate CloudEvent by ensuring all required metadata is present
    if (cloud_event.id().empty() || cloud_event.source().empty() || cloud_event.spec_version().empty(), cloud_event.type().empty()) {
        return absl::InvalidArgumentError("Required attribute in Cloud Event cannot be empty");
    }

    Json::Value root;
    root["id"] = cloud_event.id();
    root["source"] = cloud_event.source();
    root["spec_version"] = cloud_event.spec_version();
    root["type"] = cloud_event.type();

    for (auto const& x : cloud_event.attributes()) {
        absl::StatusOr<Json::Value> json_printed = JsonFormatter::PrintToJson(x.second);
        if (!json_printed.ok()) {
            return json_printed.status();
        }
        root[x.first] = *json_printed;
    }

    switch (cloud_event.data_oneof_case()) {
        case CloudEvent::DataOneofCase::kBinaryData:
            root["data_base64"] = cloud_event.binary_data();
            break;
        case CloudEvent::DataOneofCase::kTextData:
            root["data"] = cloud_event.text_data();
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
    structured_ce.format = Format::JSON;
    structured_ce.serialization = Json::writeString(builder, root);
    return structured_ce;
}

absl::StatusOr<CloudEvent> JsonFormatter::Deserialize(StructuredCloudEvent structured_cloud_event) {
    // Validate that this is the right format to be handled by this object
    if (structured_cloud_event.format != Format::JSON) {
        return absl::InternalError("This structured cloud event is not JSON formatted and should not be handled by a Json Formatter.");
    }

    // Create a Json::Value from the serialization string
    std::string serialization = structured_cloud_event.serialization;
    Json::CharReaderBuilder builder;
    Json::CharReader * reader = builder.newCharReader();
    std::string errors;
    Json::Value root;   
    bool parsingSuccessful = reader->parse(serialization.c_str(), serialization.c_str() + serialization.size(), &root, &errors);
    if (!parsingSuccessful) {
        return absl::InvalidArgumentError(errors);
    }


    // check that serialization contains a valid CE
    if (!root.isMember("id") || !root.isMember("source") || !root.isMember("spec_version") || !root.isMember("type")) { // using isMember to avoid creating null member, avoids quirk of JsonCpp
        return absl::InvalidArgumentError("Provided input is missing required Cloud Event attributes.");
    }

    CloudEvent cloud_event;
    
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

    if (root.isMember("data") && root.isMember("data_base64")) {
        return absl::InvalidArgumentError("Provided input contains two data payloads and is an invalid serialization.");
    } else if (root.isMember("data")) {
        cloud_event.set_text_data(root["data"].asString());
    } else if (root.isMember("data_base64")) {
        cloud_event.set_binary_data(root["data_base64"].asString());
    }

    return cloud_event;
}

} // format
} // cloudevents
