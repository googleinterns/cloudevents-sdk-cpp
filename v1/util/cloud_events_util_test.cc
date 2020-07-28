#include "cloud_events_util.h"
#include <gtest/gtest.h>

#include <iostream> 

namespace cloudevents {
namespace util {

using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;

typedef std::map<std::string, CloudEvent_CloudEventAttribute> CeAttrMap;

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

TEST(CloudEventsUtilTest, GetMetadata_InvalidCloudEvent) {
    CloudEvent cloud_event;
    cloud_event.set_id("1");
    cloud_event.set_source("/test");
    cloud_event.set_spec_version("1.0");
    absl::Status get_metadata_status;

    get_metadata_status = CloudEventsUtil::GetMetadata(cloud_event).status();
    
    ASSERT_TRUE(absl::IsInvalidArgument(get_metadata_status));
}

TEST(CloudEventsUtilTest, GetMetadata_NoOptional) {
    // setup input to function
    CloudEvent cloud_event;
    cloud_event.set_id("1");
    cloud_event.set_source("/test");
    cloud_event.set_spec_version("1.0");
    cloud_event.set_type("test");

    absl::StatusOr<CeAttrMap> get_metadata = CloudEventsUtil::GetMetadata(cloud_event);

    ASSERT_TRUE(get_metadata.ok());
    ASSERT_TRUE((*get_metadata)["id"].ce_string() == "1");
    ASSERT_TRUE((*get_metadata)["source"].ce_string() == "/test");
    ASSERT_TRUE((*get_metadata)["spec_version"].ce_string() == "1.0");
    ASSERT_TRUE((*get_metadata)["type"].ce_string() == "test");
}


TEST(CloudEventsUtilTest, GetMetadata_OneOptional) {
    CloudEvent cloud_event;
    cloud_event.set_id("1");
    cloud_event.set_source("/test");
    cloud_event.set_spec_version("1.0");
    cloud_event.set_type("test");
    CloudEvent_CloudEventAttribute attr;
    attr.set_ce_string("test_val");
    (*cloud_event.mutable_attributes())["test_key"] = attr;

    absl::StatusOr<CeAttrMap> get_metadata = CloudEventsUtil::GetMetadata(cloud_event);
    ASSERT_TRUE(get_metadata.ok());
    ASSERT_TRUE((*get_metadata)["id"].ce_string() == "1");
    ASSERT_TRUE((*get_metadata)["source"].ce_string() == "/test");
    ASSERT_TRUE((*get_metadata)["spec_version"].ce_string() == "1.0");
    ASSERT_TRUE((*get_metadata)["type"].ce_string() == "test");
    ASSERT_TRUE((*get_metadata)["test_key"].ce_string() == "test_val");
}

TEST(CloudEventsUtilTest, GetMetadata_TwoOptional) {
    CloudEvent cloud_event;
    cloud_event.set_id("1");
    cloud_event.set_source("/test");
    cloud_event.set_spec_version("1.0");
    cloud_event.set_type("test");
    CloudEvent_CloudEventAttribute attr;
    attr.set_ce_string("test_val1");
    (*cloud_event.mutable_attributes())["test_key1"] = attr;
    attr.set_ce_string("test_val2");
    (*cloud_event.mutable_attributes())["test_key2"] = attr;

    absl::StatusOr<CeAttrMap> get_metadata = CloudEventsUtil::GetMetadata(cloud_event);
    ASSERT_TRUE(get_metadata.ok());
    ASSERT_TRUE((*get_metadata)["id"].ce_string() == "1");
    ASSERT_TRUE((*get_metadata)["source"].ce_string() == "/test");
    ASSERT_TRUE((*get_metadata)["spec_version"].ce_string() == "1.0");
    ASSERT_TRUE((*get_metadata)["type"].ce_string() == "test");
    ASSERT_TRUE((*get_metadata)["test_key1"].ce_string() == "test_val1");
    ASSERT_TRUE((*get_metadata)["test_key2"].ce_string() == "test_val2");
}

} // util
} // cloudevents
