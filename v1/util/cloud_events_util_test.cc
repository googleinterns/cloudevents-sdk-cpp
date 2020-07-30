#include "cloud_events_util.h"
#include <google/protobuf/timestamp.pb.h>
#include <google/protobuf/util/time_util.h>
#include <gtest/gtest.h>
#include <iostream>

namespace cloudevents {
namespace cloudevents_util {

using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;
using ::google::protobuf::Timestamp;
using ::google::protobuf::util::TimeUtil;

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

TEST(CloudEventsUtilTest, StringifyCeType_BoolFalse) {
    CloudEvent_CloudEventAttribute attr;
    attr.set_ce_boolean(false);
    absl::StatusOr<std::string> stringify_ce_type;

    stringify_ce_type = CloudEventsUtil::StringifyCeType(attr);

    ASSERT_TRUE(stringify_ce_type.ok());
    ASSERT_EQ((*stringify_ce_type), "false");
}

TEST(CloudEventsUtilTest, StringifyCeType_BoolTrue) {
    CloudEvent_CloudEventAttribute attr;
    attr.set_ce_boolean(true);
    absl::StatusOr<std::string> stringify_ce_type;

    stringify_ce_type = CloudEventsUtil::StringifyCeType(attr);

    ASSERT_TRUE(stringify_ce_type.ok());
    ASSERT_EQ((*stringify_ce_type), "true");
}

TEST(CloudEventsUtilTest, StringifyCeType_Integer) {
    CloudEvent_CloudEventAttribute attr;
    attr.set_ce_integer(88);
    absl::StatusOr<std::string> stringify_ce_type;

    stringify_ce_type = CloudEventsUtil::StringifyCeType(attr);

    ASSERT_TRUE(stringify_ce_type.ok());
    ASSERT_EQ((*stringify_ce_type), "88");
}


TEST(CloudEventsUtilTest, StringifyCeType_String) {
    CloudEvent_CloudEventAttribute attr;
    attr.set_ce_string("test");
    absl::StatusOr<std::string> stringify_ce_type;

    stringify_ce_type = CloudEventsUtil::StringifyCeType(attr);

    ASSERT_TRUE(stringify_ce_type.ok());
    ASSERT_EQ((*stringify_ce_type), "test");
}

TEST(CloudEventsUtilTest, StringifyCeType_URI) {
    CloudEvent_CloudEventAttribute attr;
    attr.set_ce_uri("https://google.com");
    absl::StatusOr<std::string> stringify_ce_type;

    stringify_ce_type = CloudEventsUtil::StringifyCeType(attr);

    ASSERT_TRUE(stringify_ce_type.ok());
    ASSERT_EQ((*stringify_ce_type), "https://google.com");
}

TEST(CloudEventsUtilTest, StringifyCeType_URIRef) {
    CloudEvent_CloudEventAttribute attr;
    attr.set_ce_uri_reference("https://www.google.com/#fragment");
    absl::StatusOr<std::string> stringify_ce_type;

    stringify_ce_type = CloudEventsUtil::StringifyCeType(attr);

    ASSERT_TRUE(stringify_ce_type.ok());
    ASSERT_EQ((*stringify_ce_type), "https://www.google.com/#fragment");
}

TEST(CloudEventsUtilTest, StringifyCeType_Timestamp) {
    Timestamp timestamp = TimeUtil::GetCurrentTime();
    std::string timestamp_str = TimeUtil::ToString(timestamp);
    CloudEvent_CloudEventAttribute attr;
    attr.mutable_ce_timestamp()-> MergeFrom(timestamp);
    absl::StatusOr<std::string> stringify_ce_type;
    
    stringify_ce_type = CloudEventsUtil::StringifyCeType(attr);

    ASSERT_EQ((*stringify_ce_type), timestamp_str);
}

TEST(CloudEventsUtilTest, StringifyCeType_NotSet) {
    CloudEvent_CloudEventAttribute attr;
    absl::StatusOr<std::string> stringify_ce_type;
    
    stringify_ce_type = CloudEventsUtil::StringifyCeType(attr);

    ASSERT_TRUE(absl::IsInvalidArgument(stringify_ce_type.status()));
}


} // util
} // cloudevents
