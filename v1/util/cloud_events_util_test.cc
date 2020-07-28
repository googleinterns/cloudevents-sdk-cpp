#include "cloud_events_util.h"
#include <gtest/gtest.h>

namespace cloudevents {
namespace util {

using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;

TEST(CloudEventsUtilTest, IsValid_NoId) {
    CloudEvent cloud_event;
    cloud_event.set_source("/test");
    cloud_event.set_spec_version("1.0");
    cloud_event.set_type("test");
    ASSERT_FALSE(CloudEventsUtil::IsValid(cloud_event));
}

TEST(CloudEventsUtilTest, IsValid_NoSource) {
    CloudEvent cloud_event;
    cloud_event.set_id("1");
    cloud_event.set_spec_version("1.0");
    cloud_event.set_type("test");
    ASSERT_FALSE(CloudEventsUtil::IsValid(cloud_event));
}

TEST(CloudEventsUtilTest, IsValid_NoSpecVersion) {
    CloudEvent cloud_event;
    cloud_event.set_id("1");
    cloud_event.set_source("/test");
    cloud_event.set_type("test");
    ASSERT_FALSE(CloudEventsUtil::IsValid(cloud_event));
}

TEST(CloudEventsUtilTest, IsValid_NoType) {
    CloudEvent cloud_event;
    cloud_event.set_id("1");
    cloud_event.set_source("/test");
    cloud_event.set_spec_version("1.0");
    ASSERT_FALSE(CloudEventsUtil::IsValid(cloud_event));
}

TEST(CloudEventsUtilTest, IsValid_HasRequired) {
    CloudEvent cloud_event;
    cloud_event.set_id("1");
    cloud_event.set_source("/test");
    cloud_event.set_spec_version("1.0");
    cloud_event.set_type("test");
    ASSERT_TRUE(CloudEventsUtil::IsValid(cloud_event));
}

} // util
} // cloudevents
