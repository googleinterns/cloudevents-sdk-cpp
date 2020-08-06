#include "json_formatter.h"

#include <google/protobuf/any.pb.h>
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/time_util.h>

#include "v1/util/cloud_events_util.h"

namespace cloudevents {
namespace format {

using ::io::cloudevents::v1::CloudEvent;
using ::cloudevents::util::CloudEventsUtil;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;
using ::google::protobuf::util::TimeUtil;

// TODO (#41): Move this to a CE Util, as it is used across formatters/ binders
static constexpr absl::string_view kCeIdKey = "id";
static constexpr absl::string_view kCeSourceKey = "source";
static constexpr absl::string_view kCeSpecKey = "spec_version";
static constexpr absl::string_view kCeTypeKey = "type";

static constexpr absl::string_view kBinaryDataKey = "data_base64";
static constexpr absl::string_view kJsonDataKey = "data";

absl::StatusOr<Json::Value> JsonFormatter::PrintToJson(
        const CloudEvent_CloudEventAttribute& attr){    
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
    return absl::InternalError("A Cloud Event attribute has not been handled.");
}

absl::StatusOr<StructuredCloudEvent> JsonFormatter::Serialize(
        const CloudEvent& cloud_event) {
    // validate CloudEvent by ensuring all required metadata is present
    if (!CloudEventsUtil::IsValid(cloud_event)) {
        return absl::InvalidArgumentError("Required attribute in Cloud Event cannot be empty");
    }

    Json::Value root;
    root[kCeIdKey.data()] = cloud_event.id();
    root[kCeSourceKey.data()] = cloud_event.source();
    root[kCeSpecKey.data()] = cloud_event.spec_version();
    root[kCeTypeKey.data()] = cloud_event.type();

    for (auto const& attr : cloud_event.attributes()) {
        absl::StatusOr<Json::Value> json_printed = PrintToJson(attr.second);
        if (!json_printed.ok()) {
            return json_printed.status();
        }
        root[attr.first] = (*json_printed);
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

absl::StatusOr<CloudEvent> JsonFormatter::Deserialize(
        const StructuredCloudEvent& structured_ce) {
    // Validate that this is the right format to be handled by this object
    if (structured_ce.format != Format::kJson) {
        return absl::InternalError(
            "This structured cloud event is not JSON formatted and should not be handled by a Json Formatter.");
    }

    // Create a Json::Value from the serialization string
    std::string serialization = structured_ce.serialization;
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
    
    for (auto const& member : root.getMemberNames()) {
        if (member == kCeIdKey.data()) {
            cloud_event.set_id(root[member].asString());
        } else if (member == kCeSourceKey.data()) {
            cloud_event.set_source(root[member].asString());
        } else if (member == kCeSpecKey.data()) {
            cloud_event.set_spec_version(root[member].asString());
        } else if (member == kCeTypeKey.data()) {
            cloud_event.set_type(root[member].asString());
        } else {
            CloudEvent_CloudEventAttribute attr;
            attr.set_ce_string(root[member].asString());
            (*cloud_event.mutable_attributes())[member] = attr;
        }
    }

    if (root.isMember(kJsonDataKey.data()) && root.isMember(kBinaryDataKey.data())) {
        return absl::InvalidArgumentError(
            "Provided input contains two data payloads and is an invalid serialization.");
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
