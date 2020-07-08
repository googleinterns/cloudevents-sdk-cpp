#include "structured_cloud_event.h"
#include <iostream>

#include <gtest/gtest.h>
namespace cloud_events {
namespace format {
using ::io::cloudevents::v1::CloudEvent;

TEST(test, runs) {
    ASSERT_EQ(1,1);
}

TEST(StructuredCloudEvent, constructor) {
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

TEST(JsonMarshaller, serialize) {
    JsonMarshaller m;
    CloudEvent ce1;
    ce1.set_id("1");
    ce1.set_source("/test");
    ce1.set_spec_version("1.0");
    ce1.set_type("test");
    absl::StatusOr<StructuredCloudEvent> sce1 = m.Serialize(ce1);
    ASSERT_TRUE(sce1);
    ASSERT_EQ(sce1.value().GetCloudEventFormat(), CloudEventFormat::JSON);
    ASSERT_EQ(sce1.value().GetSerializedCloudEvent(),"{\"id\":\"1\",\"source\":\"/test\",\"spec_version\":\"1.0\",\"type\":\"test\"}\n");
    
    CloudEvent ce2;
    ce2.set_id("9999999");
    ce2.set_source("/test/qwertyuiop");
    ce2.set_spec_version("2.xxxxx");
    ce2.set_type("not_a_type");
    absl::StatusOr<StructuredCloudEvent> sce2 = m.Serialize(ce2);
    ASSERT_TRUE(sce2);
    ASSERT_EQ(sce2.value().GetCloudEventFormat(), CloudEventFormat::JSON);
    ASSERT_EQ(sce2.value().GetSerializedCloudEvent(),"{\"id\":\"9999999\",\"source\":\"/test/qwertyuiop\",\"spec_version\":\"2.xxxxx\",\"type\":\"not_a_type\"}\n");

}

TEST(JsonMarshaller, deserialize) {
}

} // format
} // cloud_events
