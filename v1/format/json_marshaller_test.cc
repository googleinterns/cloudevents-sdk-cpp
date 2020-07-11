#include "json_marshaller.h"
#include <iostream>
#include <gtest/gtest.h>

namespace cloud_events {
namespace format {
using ::io::cloudevents::v1::CloudEvent;

TEST(JsonMarshaller, Serialize) {
    JsonMarshaller m;
    CloudEvent ce1;
    ce1.set_id("1");
    ce1.set_source("/test");
    ce1.set_spec_version("1.0");
    ce1.set_type("test");
    absl::StatusOr<StructuredCloudEvent> sce1 = m.Serialize(ce1);
    ASSERT_TRUE(sce1);
    ASSERT_EQ(sce1.value().GetCloudEventFormat(), CloudEventFormat::JSON);
    ASSERT_EQ(sce1.value().GetSerializedCloudEvent(),"{\n\t\"id\" : \"1\",\n\t\"source\" : \"/test\",\n\t\"spec_version\" : \"1.0\",\n\t\"type\" : \"test\"\n}");
    
    CloudEvent ce2;
    ce2.set_id("9999999");
    ce2.set_source("/test/qwertyuiop");
    ce2.set_spec_version("2.xxxxx");
    ce2.set_type("not_a_type");
    absl::StatusOr<StructuredCloudEvent> sce2 = m.Serialize(ce2);
    ASSERT_TRUE(sce2);
    ASSERT_EQ(sce2.value().GetCloudEventFormat(), CloudEventFormat::JSON);
    ASSERT_EQ(sce2.value().GetSerializedCloudEvent(), "{\n\t\"id\" : \"9999999\",\n\t\"source\" : \"/test/qwertyuiop\",\n\t\"spec_version\" : \"2.xxxxx\",\n\t\"type\" : \"not_a_type\"\n}");

    CloudEvent ce3;
    ce3.set_id("9999999");
    ce3.set_source("/test/qwertyuiop");
    ce3.set_spec_version("3.xxxxx");
    ce3.set_type("not_a_type");
    ce3.set_binary_data("binary_data_wow");
    absl::StatusOr<StructuredCloudEvent> sce3 = m.Serialize(ce3);
    ASSERT_TRUE(sce3);
    ASSERT_EQ(sce3.value().GetCloudEventFormat(), CloudEventFormat::JSON);
    ASSERT_EQ(sce3.value().GetSerializedCloudEvent(), "{\n\t\"data_base64\" : \"binary_data_wow\",\n\t\"id\" : \"9999999\",\n\t\"source\" : \"/test/qwertyuiop\",\n\t\"spec_version\" : \"3.xxxxx\",\n\t\"type\" : \"not_a_type\"\n}");

    CloudEvent ce4;
    ce4.set_id("9999999");
    ce4.set_source("/test/qwertyuiop");
    ce4.set_spec_version("4.xxxxx");
    ce4.set_type("not_a_type");
    ce4.set_text_data("this is text");
    absl::StatusOr<StructuredCloudEvent> sce4 = m.Serialize(ce4);
    ASSERT_TRUE(sce4);
    ASSERT_EQ(sce4.value().GetCloudEventFormat(), CloudEventFormat::JSON);
    ASSERT_EQ(sce4.value().GetSerializedCloudEvent(), "{\n\t\"data\" : \"this is text\",\n\t\"id\" : \"9999999\",\n\t\"source\" : \"/test/qwertyuiop\",\n\t\"spec_version\" : \"4.xxxxx\",\n\t\"type\" : \"not_a_type\"\n}");

}

TEST(JsonMarshaller, Deserialize) {
    JsonMarshaller m;
    std::string s1 = "{\n\t\"id\" : \"1\",\n\t\"source\" : \"/test\",\n\t\"spec_version\" : \"1.0\",\n\t\"type\" : \"test\"\n}";
    StructuredCloudEvent sce1 = StructuredCloudEvent(CloudEventFormat::JSON, s1);
    absl::StatusOr<CloudEvent> ce1 = m.Deserialize(sce1);
    ASSERT_TRUE(ce1);
    ASSERT_EQ(ce1.value().id(), "1");
    ASSERT_EQ(ce1.value().source(), "/test");
    ASSERT_EQ(ce1.value().spec_version(), "1.0");
    ASSERT_EQ(ce1.value().type(), "test");

    std::string s2 = "{\n\t\"data_base64\" : \"binary_data_wow\",\n\t\"id\" : \"9999999\",\n\t\"source\" : \"/test/qwertyuiop\",\n\t\"spec_version\" : \"3.xxxxx\",\n\t\"type\" : \"not_a_type\"\n}";
    StructuredCloudEvent sce2 = StructuredCloudEvent(CloudEventFormat::JSON, s2);
    absl::StatusOr<CloudEvent> ce2 = m.Deserialize(sce2);
    ASSERT_TRUE(ce2);
    ASSERT_EQ(ce2.value().id(), "9999999");
    ASSERT_EQ(ce2.value().source(), "/test/qwertyuiop");
    ASSERT_EQ(ce2.value().spec_version(), "3.xxxxx");
    ASSERT_EQ(ce2.value().type(), "not_a_type");
    ASSERT_EQ(ce2.value().binary_data(), "binary_data_wow");

    std::string s3 = "{\n\t\"data\" : \"this is text\",\n\t\"id\" : \"9999999\",\n\t\"source\" : \"/test/qwertyuiop\",\n\t\"spec_version\" : \"4\",\n\t\"type\" : \"not_a_type\"\n}";
    StructuredCloudEvent sce3 = StructuredCloudEvent(CloudEventFormat::JSON, s3);
    absl::StatusOr<CloudEvent> ce3 = m.Deserialize(sce3);
    ASSERT_TRUE(ce3);
    ASSERT_EQ(ce3.value().id(), "9999999");
    ASSERT_EQ(ce3.value().source(), "/test/qwertyuiop");
    ASSERT_EQ(ce3.value().spec_version(), "4");
    ASSERT_EQ(ce3.value().type(), "not_a_type");
    ASSERT_EQ(ce3.value().text_data(), "this is text");
}

} // format
} // cloud_events
