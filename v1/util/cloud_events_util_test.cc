#include "cloud_events_util.h"

#include <google/protobuf/timestamp.pb.h>
#include <google/protobuf/util/time_util.h>
#include <gtest/gtest.h>

namespace cloudevents {
namespace cloudevents_util {

using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;
using ::google::protobuf::Timestamp;
using ::google::protobuf::util::TimeUtil;

typedef absl::flat_hash_map<std::string, CloudEvent_CloudEventAttribute>
  CeAttrMap;

TEST(CloudEventsUtilTest, IsValid_NoSource) {
  CloudEvent cloud_event;
  cloud_event.set_id("1");
  cloud_event.set_spec_version("1.0");
  cloud_event.set_type("test");

  absl::Status is_valid = CloudEventsUtil::IsValid(cloud_event);

  ASSERT_FALSE(is_valid.ok());
  ASSERT_TRUE(absl::IsInvalidArgument(is_valid));
}

TEST(CloudEventsUtilTest, IsValid_NoSpecVersion) {
  CloudEvent cloud_event;
  cloud_event.set_id("1");
  cloud_event.set_source("/test");
  cloud_event.set_type("test");

  absl::Status is_valid = CloudEventsUtil::IsValid(cloud_event);

  ASSERT_FALSE(is_valid.ok());
  ASSERT_TRUE(absl::IsInvalidArgument(is_valid));
}

TEST(CloudEventsUtilTest, IsValid_NoType) {
  CloudEvent cloud_event;
  cloud_event.set_id("1");
  cloud_event.set_source("/test");
  cloud_event.set_spec_version("1.0");

  absl::Status is_valid = CloudEventsUtil::IsValid(cloud_event);

  ASSERT_FALSE(is_valid.ok());
  ASSERT_TRUE(absl::IsInvalidArgument(is_valid));
}

TEST(CloudEventsUtilTest, IsValid_HasRequired) {
  CloudEvent cloud_event;
  cloud_event.set_id("1");
  cloud_event.set_source("/test");
  cloud_event.set_spec_version("1.0");
  cloud_event.set_type("test");

  absl::Status is_valid = CloudEventsUtil::IsValid(cloud_event);

  ASSERT_TRUE(is_valid.ok());
}

TEST(CloudEventsUtilTest, GetMetadata_InvalidCloudEvent) {
  CloudEvent cloud_event;
  cloud_event.set_id("1");
  cloud_event.set_source("/test");
  cloud_event.set_spec_version("1.0");

  absl::Status get_metadata_status = CloudEventsUtil::GetMetadata(
    cloud_event).status();

  ASSERT_TRUE(absl::IsInvalidArgument(get_metadata_status));
}

TEST(CloudEventsUtilTest, GetMetadata_NoOptional) {
  // setup input to function
  CloudEvent cloud_event;
  cloud_event.set_id("1");
  cloud_event.set_source("/test");
  cloud_event.set_spec_version("1.0");
  cloud_event.set_type("test");

  cloudevents_absl::StatusOr<CeAttrMap> get_metadata =
    CloudEventsUtil::GetMetadata(cloud_event);

  ASSERT_TRUE(get_metadata.ok());
  ASSERT_EQ((*get_metadata)["id"].ce_string(), "1");
  ASSERT_EQ((*get_metadata)["source"].ce_string(), "/test");
  ASSERT_EQ((*get_metadata)["spec_version"].ce_string(), "1.0");
  ASSERT_EQ((*get_metadata)["type"].ce_string(), "test");
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

  cloudevents_absl::StatusOr<CeAttrMap> get_metadata =
    CloudEventsUtil::GetMetadata(cloud_event);

  ASSERT_TRUE(get_metadata.ok());
  ASSERT_EQ((*get_metadata)["id"].ce_string(), "1");
  ASSERT_EQ((*get_metadata)["source"].ce_string(), "/test");
  ASSERT_EQ((*get_metadata)["spec_version"].ce_string(), "1.0");
  ASSERT_EQ((*get_metadata)["type"].ce_string(), "test");
  ASSERT_EQ((*get_metadata)["test_key"].ce_string(), "test_val");
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

  cloudevents_absl::StatusOr<CeAttrMap> get_metadata =
    CloudEventsUtil::GetMetadata(cloud_event);

  ASSERT_TRUE(get_metadata.ok());
  ASSERT_EQ((*get_metadata)["id"].ce_string(), "1");
  ASSERT_EQ((*get_metadata)["source"].ce_string(), "/test");
  ASSERT_EQ((*get_metadata)["spec_version"].ce_string(), "1.0");
  ASSERT_EQ((*get_metadata)["type"].ce_string(), "test");
  ASSERT_EQ((*get_metadata)["test_key1"].ce_string(), "test_val1");
  ASSERT_EQ((*get_metadata)["test_key2"].ce_string(), "test_val2");
}

TEST(CloudEventsUtilTest, SetMetadata_Id) {
  CloudEvent cloud_event;

  absl::Status set_meta = CloudEventsUtil::SetMetadata("id", "1",
    cloud_event);

  ASSERT_TRUE(set_meta.ok());
  ASSERT_EQ(cloud_event.id(), "1");
}

TEST(CloudEventsUtilTest, SetMetadata_Source) {
  CloudEvent cloud_event;

  absl::Status set_meta = CloudEventsUtil::SetMetadata("source", "/a_source",
  cloud_event);

  ASSERT_TRUE(set_meta.ok());
  ASSERT_EQ(cloud_event.source(), "/a_source");
}

TEST(CloudEventsUtilTest, SetMetadata_SpecVersion) {
  CloudEvent cloud_event;

  absl::Status set_meta = CloudEventsUtil::SetMetadata("spec_version", "1.xx",
    cloud_event);

  ASSERT_TRUE(set_meta.ok());
  ASSERT_EQ(cloud_event.spec_version(), "1.xx");
}

TEST(CloudEventsUtilTest, SetMetadata_Type) {
  CloudEvent cloud_event;

  absl::Status set_meta = CloudEventsUtil::SetMetadata("type", "test",
    cloud_event);

  ASSERT_TRUE(set_meta.ok());
  ASSERT_EQ(cloud_event.type(), "test");
}

TEST(CloudEventsUtilTest, SetMetadata_Optional) {
  CloudEvent cloud_event;

  absl::Status set_meta = CloudEventsUtil::SetMetadata("opt", "arbitrary",
    cloud_event);

  CloudEvent_CloudEventAttribute attr = cloud_event.attributes().at("opt");

  ASSERT_TRUE(set_meta.ok());
  ASSERT_EQ(attr.ce_string(), "arbitrary");
}

TEST(CloudEventsUtilTest, SetMetadata_Time) {
  CloudEvent cloud_event;
  Timestamp timestamp = TimeUtil::GetCurrentTime();
  std::string timestamp_str = TimeUtil::ToString(timestamp);

  absl::Status set_meta = CloudEventsUtil::SetMetadata("time", timestamp_str,
    cloud_event);
  CloudEvent_CloudEventAttribute attr = cloud_event.attributes().at("time");

  ASSERT_TRUE(set_meta.ok());
  ASSERT_EQ(TimeUtil::ToString(attr.ce_timestamp()), timestamp_str);
}

TEST(CloudEventsUtilTest, ToString_BoolFalse) {
  CloudEvent_CloudEventAttribute attr;
  attr.set_ce_boolean(false);

  cloudevents_absl::StatusOr<std::string> stringify_ce_type =
    CloudEventsUtil::ToString(attr);

  ASSERT_TRUE(stringify_ce_type.ok());
  ASSERT_EQ(*stringify_ce_type, "false");
}

TEST(CloudEventsUtilTest, ToString_BoolTrue) {
  CloudEvent_CloudEventAttribute attr;
  attr.set_ce_boolean(true);

  cloudevents_absl::StatusOr<std::string> stringify_ce_type =
    CloudEventsUtil::ToString(attr);

  ASSERT_TRUE(stringify_ce_type.ok());
  ASSERT_EQ(*stringify_ce_type, "true");
}

TEST(CloudEventsUtilTest, ToString_Integer) {
  CloudEvent_CloudEventAttribute attr;
  attr.set_ce_integer(88);

  cloudevents_absl::StatusOr<std::string> stringify_ce_type =
    CloudEventsUtil::ToString(attr);

  ASSERT_TRUE(stringify_ce_type.ok());
  ASSERT_EQ(*stringify_ce_type, "88");
}

TEST(CloudEventsUtilTest, ToString_String) {
  CloudEvent_CloudEventAttribute attr;
  attr.set_ce_string("test");

  cloudevents_absl::StatusOr<std::string> stringify_ce_type =
    CloudEventsUtil::ToString(attr);

  ASSERT_TRUE(stringify_ce_type.ok());
  ASSERT_EQ(*stringify_ce_type, "test");
}

TEST(CloudEventsUtilTest, ToString_URI) {
  CloudEvent_CloudEventAttribute attr;
  attr.set_ce_uri("https://google.com");

  cloudevents_absl::StatusOr<std::string> stringify_ce_type =
    CloudEventsUtil::ToString(attr);

  ASSERT_TRUE(stringify_ce_type.ok());
  ASSERT_EQ(*stringify_ce_type, "https://google.com");
}

TEST(CloudEventsUtilTest, ToString_URIRef) {
  CloudEvent_CloudEventAttribute attr;
  attr.set_ce_uri_reference("https://www.google.com/#fragment");

  cloudevents_absl::StatusOr<std::string> stringify_ce_type =
    CloudEventsUtil::ToString(attr);

  ASSERT_TRUE(stringify_ce_type.ok());
  ASSERT_EQ(*stringify_ce_type, "https://www.google.com/#fragment");
}

TEST(CloudEventsUtilTest, ToString_Timestamp) {
  Timestamp timestamp = TimeUtil::GetCurrentTime();
  std::string timestamp_str = TimeUtil::ToString(timestamp);
  CloudEvent_CloudEventAttribute attr;
  attr.mutable_ce_timestamp()-> MergeFrom(timestamp);

  cloudevents_absl::StatusOr<std::string> stringify_ce_type =
    CloudEventsUtil::ToString(attr);

  ASSERT_EQ(*stringify_ce_type, timestamp_str);
}

TEST(CloudEventsUtilTest, ToString_NotSet) {
  CloudEvent_CloudEventAttribute attr;

  cloudevents_absl::StatusOr<std::string> stringify_ce_type =
    CloudEventsUtil::ToString(attr);

  ASSERT_TRUE(absl::IsInvalidArgument(stringify_ce_type.status()));
}


}  // namespace cloudevents_util
}  // namespace cloudevents
