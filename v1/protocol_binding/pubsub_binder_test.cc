#include "pubsub_binder.h"

#include <gtest/gtest.h>

namespace cloudevents {
namespace binding {
using ::cloudevents_absl::StatusOr;
using ::cloudevents_base64::base64_encode;
using ::google::pubsub::v1::PubsubMessage;
using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;
using ::cloudevents::format::StructuredCloudEvent;
using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::formatter_util::FormatterUtil;

// Setup a valid CloudEvent
class BindTest : public ::testing::Test {
 protected:
  void SetUp() override {
    ce.set_id("1");
    ce.set_source("2");
    ce.set_spec_version("3");
    ce.set_type("4");
  }
  PubsubBinder binder;
  CloudEvent ce;
};

// Setup a Binary PubsubMessage containing valid CloudEvent
class UnbindBinaryTest : public ::testing::Test {
 protected:
  void SetUp() override {
    (*pubsub_msg.mutable_attributes())["ce-id"] = "1";
    (*pubsub_msg.mutable_attributes())["ce-source"] = "2";
    (*pubsub_msg.mutable_attributes())["ce-specversion"] = "3";
    (*pubsub_msg.mutable_attributes())["ce-type"] = "4";
  }
  PubsubBinder binder;
  PubsubMessage pubsub_msg;
};

// Setup a Structured PubsubMessage containing valid CloudEvent
class UnbindStructuredTest : public ::testing::Test {
 protected:
  void SetUp() override {
    (*pubsub_msg.mutable_attributes())["content-type"] = "application/cloudevents+json";
  }
  std::string valid_payload = "{\n\t\"id\" : \"1\",\n\t\"source\" : \"/test\",\n\t\"specversion\" : \"1.0\",\n\t\"type\" : \"test\"\n}";
  PubsubBinder binder;
  PubsubMessage pubsub_msg;
};

TEST(Bind, Invalid) {
  PubsubBinder binder;
  CloudEvent ce;

  StatusOr<PubsubMessage> bind = binder.Bind(ce);

  ASSERT_FALSE(bind.ok());
  ASSERT_TRUE(absl::IsInvalidArgument(bind.status()));
}

TEST_F(BindTest, Binary_Required) {
  StatusOr<PubsubMessage> bind = binder.Bind(ce);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).attributes().at("ce-id"), "1");
  ASSERT_EQ((*bind).attributes().at("ce-source"), "2");
  ASSERT_EQ((*bind).attributes().at("ce-specversion"), "3");
  ASSERT_EQ((*bind).attributes().at("ce-type"), "4");
}

TEST_F(BindTest, Binary_Optional) {
  CloudEvent_CloudEventAttribute opt_val;
  opt_val.set_ce_string("5");
  (*ce.mutable_attributes())["opt"] = opt_val;

  StatusOr<PubsubMessage> bind = binder.Bind(ce);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).attributes().at("ce-id"), "1");
  ASSERT_EQ((*bind).attributes().at("ce-source"), "2");
  ASSERT_EQ((*bind).attributes().at("ce-specversion"), "3");
  ASSERT_EQ((*bind).attributes().at("ce-type"), "4");
  ASSERT_EQ((*bind).attributes().at("ce-opt"), "5");
}

TEST_F(BindTest, Binary_BinData) {
  ce.set_binary_data("1010");

  StatusOr<PubsubMessage> bind = binder.Bind(ce);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).attributes().at("ce-id"), "1");
  ASSERT_EQ((*bind).attributes().at("ce-source"), "2");
  ASSERT_EQ((*bind).attributes().at("ce-specversion"), "3");
  ASSERT_EQ((*bind).attributes().at("ce-type"), "4");
  ASSERT_EQ((*bind).data(), "1010");
}

TEST_F(BindTest, Binary_TextData) {
  ce.set_text_data("hello");

  StatusOr<PubsubMessage> bind = binder.Bind(ce);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).attributes().at("ce-id"), "1");
  ASSERT_EQ((*bind).attributes().at("ce-source"), "2");
  ASSERT_EQ((*bind).attributes().at("ce-specversion"), "3");
  ASSERT_EQ((*bind).attributes().at("ce-type"), "4");
  ASSERT_EQ((*bind).data(), "aGVsbG8=");
}

TEST_F(BindTest, Structured_Required) {
  StatusOr<PubsubMessage> bind = binder.Bind(ce, Format::kJson);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).attributes().at("content-type"), "application/cloudevents+json");
  ASSERT_EQ((*bind).data(), "{\n\t\"id\" : \"1\",\n\t\"source\" : \"2\",\n\t\"specversion\" : \"3\",\n\t\"type\" : \"4\"\n}");
}

TEST_F(BindTest, Structured_Optional) {
  CloudEvent_CloudEventAttribute opt_val;
  opt_val.set_ce_string("5");
  (*ce.mutable_attributes())["opt"] = opt_val;

  StatusOr<PubsubMessage> bind = binder.Bind(ce, Format::kJson);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).attributes().at("content-type"), "application/cloudevents+json");
  ASSERT_EQ((*bind).data(), "{\n\t\"id\" : \"1\",\n\t\"opt\" : \"5\",\n\t\"source\" : \"2\",\n\t\"specversion\" : \"3\",\n\t\"type\" : \"4\"\n}");
}

TEST_F(BindTest, Structured_BinData) {
  ce.set_binary_data("1010");

  StatusOr<PubsubMessage> bind = binder.Bind(ce, Format::kJson);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).attributes().at("content-type"), "application/cloudevents+json");
  ASSERT_EQ((*bind).data(), "{\n\t\"data_base64\" : \"1010\",\n\t\"id\" : \"1\",\n\t\"source\" : \"2\",\n\t\"specversion\" : \"3\",\n\t\"type\" : \"4\"\n}");
}

TEST_F(BindTest, Structured_TextData) {
  ce.set_text_data("hello");

  StatusOr<PubsubMessage> bind = binder.Bind(ce, Format::kJson);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).attributes().at("content-type"), "application/cloudevents+json");
  ASSERT_EQ((*bind).data(), "{\n\t\"data\" : \"hello\",\n\t\"id\" : \"1\",\n\t\"source\" : \"2\",\n\t\"specversion\" : \"3\",\n\t\"type\" : \"4\"\n}");
}

TEST_F(UnbindBinaryTest, Invalid) {
  PubsubBinder binder;
  PubsubMessage pubsub_msg;
  StatusOr<CloudEvent> unbind = binder.Unbind(pubsub_msg);

  ASSERT_FALSE(unbind.ok());
  ASSERT_TRUE(absl::IsInvalidArgument(unbind.status()));
}

TEST_F(UnbindBinaryTest, Required) {
  StatusOr<CloudEvent> unbind = binder.Unbind(pubsub_msg);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "2");
  ASSERT_EQ((*unbind).spec_version(), "3");
  ASSERT_EQ((*unbind).type(), "4");
}

TEST_F(UnbindBinaryTest, Optional) {
  (*pubsub_msg.mutable_attributes())["ce-opt"] = "5";

  StatusOr<CloudEvent> unbind = binder.Unbind(pubsub_msg);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "2");
  ASSERT_EQ((*unbind).spec_version(), "3");
  ASSERT_EQ((*unbind).type(), "4");
  ASSERT_EQ((*unbind).attributes().at("opt").ce_string(), "5");
}

TEST_F(UnbindBinaryTest, Data) {
  (*pubsub_msg.mutable_attributes())["ce-opt"] = "5";
  pubsub_msg.set_data("1010");

  StatusOr<CloudEvent> unbind = binder.Unbind(pubsub_msg);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "2");
  ASSERT_EQ((*unbind).spec_version(), "3");
  ASSERT_EQ((*unbind).type(), "4");
  ASSERT_EQ((*unbind).binary_data(), "1010");
}

TEST(Unbind, Structured_Invalid) {
  PubsubBinder binder;
  PubsubMessage pubsub_msg;
  (*pubsub_msg.mutable_attributes())["content-type"] = "application/cloudevents+json";

  StatusOr<CloudEvent> unbind = binder.Unbind(pubsub_msg);

  ASSERT_FALSE(unbind.ok());
  ASSERT_TRUE(absl::IsInvalidArgument(unbind.status()));
}

TEST_F(UnbindStructuredTest, Required) {
  pubsub_msg.set_data(base64_encode(valid_payload));

  StatusOr<CloudEvent> unbind = binder.Unbind(pubsub_msg);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "/test");
  ASSERT_EQ((*unbind).spec_version(), "1.0");
  ASSERT_EQ((*unbind).type(), "test");
}

TEST_F(UnbindStructuredTest, Optional) {
  valid_payload.insert(1, "\t\"opt\" : \"5\",\n");
  pubsub_msg.set_data(base64_encode(valid_payload));

  StatusOr<CloudEvent> unbind = binder.Unbind(pubsub_msg);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "/test");
  ASSERT_EQ((*unbind).spec_version(), "1.0");
  ASSERT_EQ((*unbind).type(), "test");
  ASSERT_EQ((*unbind).attributes().at("opt").ce_string(), "5");
}

TEST_F(UnbindStructuredTest, BinData) {
  valid_payload.insert(1, "\t\"data_base64\" : \"1010\",\n");
  pubsub_msg.set_data(base64_encode(valid_payload));

  StatusOr<CloudEvent> unbind = binder.Unbind(pubsub_msg);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "/test");
  ASSERT_EQ((*unbind).spec_version(), "1.0");
  ASSERT_EQ((*unbind).type(), "test");
  ASSERT_EQ((*unbind).binary_data(), "1010");
}

TEST_F(UnbindStructuredTest, TextData) {
  valid_payload.insert(1, "\t\"data\" : \"hello\",\n");
  pubsub_msg.set_data(base64_encode(valid_payload));

  StatusOr<CloudEvent> unbind = binder.Unbind(pubsub_msg);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "/test");
  ASSERT_EQ((*unbind).spec_version(), "1.0");
  ASSERT_EQ((*unbind).type(), "test");
  ASSERT_EQ((*unbind).text_data(), "hello");
}


}  // namespace binding
}  // namespace cloudevents
