#include "structured_cloud_event.h"
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

} // format
} // cloud_events
