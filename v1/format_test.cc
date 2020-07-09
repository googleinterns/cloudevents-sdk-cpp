#include "format.h"
#include <iostream>

#include <gtest/gtest.h>
namespace cloud_events {
namespace format {
using ::io::cloudevents::v1::CloudEvent;

TEST(test, runs) {
    ASSERT_EQ(1,1);
}

TEST(StructuredCloudEvent, Constructor) {
    StructuredCloudEvent sce1 = StructuredCloudEvent(CloudEventFormat::JSON, "test");
    StructuredCloudEvent sce2 = StructuredCloudEvent(CloudEventFormat::JSON, "");
    StructuredCloudEvent sce3 = StructuredCloudEvent(CloudEventFormat::JSON, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
}

TEST(StructuredCloudEvent, GetFormat) {
    StructuredCloudEvent sce1 = StructuredCloudEvent(CloudEventFormat::JSON, "test");
    ASSERT_EQ(sce1.GetCloudEventFormat(), CloudEventFormat::JSON);
    
    StructuredCloudEvent sce2 = StructuredCloudEvent(CloudEventFormat::JSON, "");
    ASSERT_EQ(sce2.GetCloudEventFormat(), CloudEventFormat::JSON);
    
    StructuredCloudEvent sce3 = StructuredCloudEvent(CloudEventFormat::JSON, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
    ASSERT_EQ(sce3.GetCloudEventFormat(), CloudEventFormat::JSON);
}

TEST(StructuredCloudEvent, GetSerialized) {
    StructuredCloudEvent sce1 = StructuredCloudEvent(CloudEventFormat::JSON, "test");
    ASSERT_EQ(sce1.GetSerializedCloudEvent(), "test");

    StructuredCloudEvent sce2 = StructuredCloudEvent(CloudEventFormat::JSON, "");
    ASSERT_EQ(sce2.GetSerializedCloudEvent(), "");

    StructuredCloudEvent sce3 = StructuredCloudEvent(CloudEventFormat::JSON, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
    ASSERT_EQ(sce3.GetSerializedCloudEvent(), "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
}

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
    absl::StatusOr<CloudEvent> ce1a = m.Deserialize(s1);
    ASSERT_TRUE(ce1a);
    ASSERT_EQ(ce1a.value().id(), "1");
    ASSERT_EQ(ce1a.value().source(), "/test");
    ASSERT_EQ(ce1a.value().spec_version(), "1.0");
    ASSERT_EQ(ce1a.value().type(), "test");

    StructuredCloudEvent sce1 = StructuredCloudEvent(CloudEventFormat::JSON, s1);
    absl::StatusOr<CloudEvent> ce1b = m.Deserialize(sce1);
    ASSERT_TRUE(ce1b);
    ASSERT_EQ(ce1b.value().id(), "1");
    ASSERT_EQ(ce1b.value().source(), "/test");
    ASSERT_EQ(ce1b.value().spec_version(), "1.0");
    ASSERT_EQ(ce1b.value().type(), "test");

    std::string s2 = "{\n\t\"data_base64\" : \"binary_data_wow\",\n\t\"id\" : \"9999999\",\n\t\"source\" : \"/test/qwertyuiop\",\n\t\"spec_version\" : \"3.xxxxx\",\n\t\"type\" : \"not_a_type\"\n}";
    absl::StatusOr<CloudEvent> ce2a = m.Deserialize(s2);
    ASSERT_TRUE(ce2a);
    ASSERT_EQ(ce2a.value().id(), "9999999");
    ASSERT_EQ(ce2a.value().source(), "/test/qwertyuiop");
    ASSERT_EQ(ce2a.value().spec_version(), "3.xxxxx");
    ASSERT_EQ(ce2a.value().type(), "not_a_type");
    ASSERT_EQ(ce2a.value().binary_data(), "binary_data_wow");

    StructuredCloudEvent sce2 = StructuredCloudEvent(CloudEventFormat::JSON, s2);
    absl::StatusOr<CloudEvent> ce2b = m.Deserialize(sce2);
    ASSERT_TRUE(ce2b);
    ASSERT_EQ(ce2b.value().id(), "9999999");
    ASSERT_EQ(ce2b.value().source(), "/test/qwertyuiop");
    ASSERT_EQ(ce2b.value().spec_version(), "3.xxxxx");
    ASSERT_EQ(ce2b.value().type(), "not_a_type");
    ASSERT_EQ(ce2b.value().binary_data(), "binary_data_wow");

    std::string s3 = "{\n\t\"data\" : \"this is text\",\n\t\"id\" : \"9999999\",\n\t\"source\" : \"/test/qwertyuiop\",\n\t\"spec_version\" : \"4\",\n\t\"type\" : \"not_a_type\"\n}";
    absl::StatusOr<CloudEvent> ce3a = m.Deserialize(s3);
    ASSERT_TRUE(ce3a);
    ASSERT_EQ(ce3a.value().id(), "9999999");
    ASSERT_EQ(ce3a.value().source(), "/test/qwertyuiop");
    ASSERT_EQ(ce3a.value().spec_version(), "4");
    ASSERT_EQ(ce3a.value().type(), "not_a_type");
    ASSERT_EQ(ce3a.value().text_data(), "this is text");

    StructuredCloudEvent sce3 = StructuredCloudEvent(CloudEventFormat::JSON, s3);
    absl::StatusOr<CloudEvent> ce3b = m.Deserialize(sce3);
    ASSERT_TRUE(ce3b);
    ASSERT_EQ(ce3b.value().id(), "9999999");
    ASSERT_EQ(ce3b.value().source(), "/test/qwertyuiop");
    ASSERT_EQ(ce3b.value().spec_version(), "4");
    ASSERT_EQ(ce3b.value().type(), "not_a_type");
    ASSERT_EQ(ce3b.value().text_data(), "this is text");
}

} // format
} // cloud_events

