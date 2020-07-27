#include "json_formatter.h"
#include <gtest/gtest.h>

namespace cloudevents {
namespace format {
using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;

TEST(JsonFormatterTest, Serialize_NoData) {
    CloudEvent cloud_event;
    cloud_event.set_id("1");
    cloud_event.set_source("/test");
    cloud_event.set_spec_version("1.0");
    cloud_event.set_type("test");
    JsonFormatter json_formatter;
    absl::StatusOr<StructuredCloudEvent> serialize;

    serialize = json_formatter.Serialize(cloud_event);
    
    ASSERT_TRUE(serialize.ok());
    ASSERT_EQ((*serialize).format, Format::JSON);
    ASSERT_EQ((*serialize).serialization,"{\n\t\"id\" : \"1\",\n\t\"source\" : \"/test\",\n\t\"spec_version\" : \"1.0\",\n\t\"type\" : \"test\"\n}");
}

TEST(JsonFormatterTest, Serialize_BinaryData) {
    CloudEvent cloud_event;
    cloud_event.set_id("4545");
    cloud_event.set_source("/bonk+bonk");
    cloud_event.set_spec_version("3.xxxxx");
    cloud_event.set_type("new");
    cloud_event.set_binary_data("0110");
    JsonFormatter json_formatter;
    absl::StatusOr<StructuredCloudEvent> serialize;

    serialize = json_formatter.Serialize(cloud_event);

    ASSERT_TRUE(serialize.ok());
    ASSERT_EQ((*serialize).format, Format::JSON);
    ASSERT_EQ((*serialize).serialization, "{\n\t\"data_base64\" : \"0110\",\n\t\"id\" : \"4545\",\n\t\"source\" : \"/bonk+bonk\",\n\t\"spec_version\" : \"3.xxxxx\",\n\t\"type\" : \"new\"\n}");
}

TEST(JsonFormatterTest, Serialize_TextData) {
    CloudEvent cloud_event;
    cloud_event.set_id("9999999");
    cloud_event.set_source("/test/qwertyuiop");
    cloud_event.set_spec_version("4.xxxxx");
    cloud_event.set_type("not_a_type");
    cloud_event.set_text_data("this is text");
    JsonFormatter json_formatter;
    absl::StatusOr<StructuredCloudEvent> serialize;
    
    serialize = json_formatter.Serialize(cloud_event);

    ASSERT_TRUE(serialize.ok());
    ASSERT_EQ((*serialize).format, Format::JSON);
    ASSERT_EQ((*serialize).serialization, "{\n\t\"data\" : \"this is text\",\n\t\"id\" : \"9999999\",\n\t\"source\" : \"/test/qwertyuiop\",\n\t\"spec_version\" : \"4.xxxxx\",\n\t\"type\" : \"not_a_type\"\n}");
}

TEST(JsonFormatterTest, Deserialize_NoData) {
    StructuredCloudEvent structured_ce;
    structured_ce.format = Format::JSON;
    structured_ce.serialization = "{\n\t\"id\" : \"1\",\n\t\"source\" : \"/test\",\n\t\"spec_version\" : \"1.0\",\n\t\"type\" : \"test\"\n}";
    JsonFormatter json_formatter;
    absl::StatusOr<CloudEvent> deserialize;

    deserialize = json_formatter.Deserialize(structured_ce);
    
    ASSERT_TRUE(deserialize.ok());
    ASSERT_EQ((*deserialize).id(), "1");
    ASSERT_EQ((*deserialize).source(), "/test");
    ASSERT_EQ((*deserialize).spec_version(), "1.0");
    ASSERT_EQ((*deserialize).type(), "test");
}

TEST(JsonFormatterTest, Deserialize_BinaryData) {
    StructuredCloudEvent structured_ce;
    structured_ce.format = Format::JSON;
    structured_ce.serialization = "{\n\t\"data_base64\" : \"binary_data_wow\",\n\t\"id\" : \"9999999\",\n\t\"source\" : \"/test/qwertyuiop\",\n\t\"spec_version\" : \"3.xxxxx\",\n\t\"type\" : \"not_a_type\"\n}";
    JsonFormatter json_formatter;
    absl::StatusOr<CloudEvent> deserialize;

    deserialize = json_formatter.Deserialize(structured_ce);

    ASSERT_TRUE(deserialize);
    ASSERT_EQ((*deserialize).id(), "9999999");
    ASSERT_EQ((*deserialize).source(), "/test/qwertyuiop");
    ASSERT_EQ((*deserialize).spec_version(), "3.xxxxx");
    ASSERT_EQ((*deserialize).type(), "not_a_type");
    ASSERT_EQ((*deserialize).binary_data(), "binary_data_wow");
}

TEST(JsonFormatterTest, Deserialize_TextData) {
    StructuredCloudEvent structured_ce;
    structured_ce.format = Format::JSON;
    structured_ce.serialization = "{\n\t\"data\" : \"this is text\",\n\t\"id\" : \"9999999\",\n\t\"source\" : \"/test/qwertyuiop\",\n\t\"spec_version\" : \"4\",\n\t\"type\" : \"not_a_type\"\n}";
    JsonFormatter json_formatter;
    absl::StatusOr<CloudEvent> deserialize;

    deserialize = json_formatter.Deserialize(structured_ce);
    
    ASSERT_TRUE(deserialize);
    ASSERT_EQ((*deserialize).id(), "9999999");
    ASSERT_EQ((*deserialize).source(), "/test/qwertyuiop");
    ASSERT_EQ((*deserialize).spec_version(), "4");
    ASSERT_EQ((*deserialize).type(), "not_a_type");
    ASSERT_EQ((*deserialize).text_data(), "this is text");
}

} // format
} // cloudevents
