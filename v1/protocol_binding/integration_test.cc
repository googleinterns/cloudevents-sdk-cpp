#include "pubsub_binder.h"
#include "http_binder.h"

#include <google/protobuf/util/message_differencer.h>
#include <gtest/gtest.h>

namespace cloudevents {
namespace binding {

using ::google::protobuf::util::MessageDifferencer;
using ::google::pubsub::v1::PubsubMessage;
using ::boost::beast::http::message;
using ::boost::beast::http::string_body;
using ::io::cloudevents::v1::CloudEvent;
using ::cloudevents::format::Format;

typedef io::cloudevents::v1::CloudEvent_CloudEventAttribute CeAttr;
typedef boost::beast::http::request<string_body> HttpRequest;
typedef boost::beast::http::response<string_body> HttpResponse;

// Setup a valid CloudEvent
class BindUnbindTest : public ::testing::Test {
 protected:
  void SetUp() override {
    ce.set_id("1");
    ce.set_source("2");
    ce.set_spec_version("3");
    ce.set_type("4");
    ce.set_binary_data("1010");
  }
  CloudEvent ce;
};

TEST_F(BindUnbindTest, Pubsub_Binary) {
  PubsubBinder pubsub_binder;
  cloudevents_absl::StatusOr<PubsubMessage> bind = pubsub_binder.Bind(ce);
  cloudevents_absl::StatusOr<CloudEvent> unbind = pubsub_binder.Unbind(*bind);

  ASSERT_TRUE(unbind.ok());
  ASSERT_TRUE(MessageDifferencer::Equals(ce, *unbind));
}

TEST_F(BindUnbindTest, Pubsub_Structured) {
  PubsubBinder pubsub_binder;
  cloudevents_absl::StatusOr<PubsubMessage> bind = pubsub_binder.Bind(ce, Format::kJson);
  cloudevents_absl::StatusOr<CloudEvent> unbind = pubsub_binder.Unbind(*bind);

  ASSERT_TRUE(unbind.ok());
  ASSERT_TRUE(MessageDifferencer::Equals(ce, *unbind));
}

TEST_F(BindUnbindTest, HttpReq_Binary) {
  HttpReqBinder http_req_binder;
  cloudevents_absl::StatusOr<HttpRequest> bind = http_req_binder.Bind(ce);
  cloudevents_absl::StatusOr<CloudEvent> unbind = http_req_binder.Unbind(*bind);

  ASSERT_TRUE(unbind.ok());
  ASSERT_TRUE(MessageDifferencer::Equals(ce, *unbind));
}

TEST_F(BindUnbindTest, HttpReq_Structured) {
  HttpReqBinder http_req_binder;
  cloudevents_absl::StatusOr<HttpRequest> bind = http_req_binder.Bind(ce, Format::kJson);
  cloudevents_absl::StatusOr<CloudEvent> unbind = http_req_binder.Unbind(*bind);

  ASSERT_TRUE(unbind.ok());
  ASSERT_TRUE(MessageDifferencer::Equals(ce, *unbind));
}

TEST_F(BindUnbindTest, HttpRes_Binary) {
  HttpResBinder http_res_binder;
  cloudevents_absl::StatusOr<HttpResponse> bind = http_res_binder.Bind(ce, Format::kJson);
  cloudevents_absl::StatusOr<CloudEvent> unbind = http_res_binder.Unbind(*bind);

  ASSERT_TRUE(unbind.ok());
  ASSERT_TRUE(MessageDifferencer::Equals(ce, *unbind));
}

TEST_F(BindUnbindTest, HttpRes_Structured) {
  HttpResBinder http_res_binder;
  cloudevents_absl::StatusOr<HttpResponse> bind = http_res_binder.Bind(ce);
  cloudevents_absl::StatusOr<CloudEvent> unbind = http_res_binder.Unbind(*bind);

  ASSERT_TRUE(unbind.ok());
  ASSERT_TRUE(MessageDifferencer::Equals(ce, *unbind));
}

TEST(UnbindBindTest, Pubsub_Binary) {
  PubsubBinder binder;
  PubsubMessage pubsub_msg;
  (*pubsub_msg.mutable_attributes())["ce-id"] = "1";
  (*pubsub_msg.mutable_attributes())["ce-source"] = "2";
  (*pubsub_msg.mutable_attributes())["ce-specversion"] = "3";
  (*pubsub_msg.mutable_attributes())["ce-type"] = "4";
  pubsub_msg.set_data("1010");
  cloudevents_absl::StatusOr<CloudEvent> unbind = binder.Unbind(pubsub_msg);
  cloudevents_absl::StatusOr<PubsubMessage> bind = binder.Bind(*unbind);

  ASSERT_TRUE(bind.ok());
  ASSERT_TRUE(MessageDifferencer::Equals(pubsub_msg, *bind));
}

TEST(UnbindBindTest, Pubsub_Structured) {
  PubsubBinder binder;
  PubsubMessage pubsub_msg;
  (*pubsub_msg.mutable_attributes())["content-type"] = "application/cloudevents+json";
  pubsub_msg.set_data("{\n\t\"data_base64\" : \"1010\",\n\t\"id\" : \"1\",\n\t\"source\" : \"/test\",\n\t\"specversion\" : \"1.0\",\n\t\"type\" : \"test\"\n}");

  cloudevents_absl::StatusOr<CloudEvent> unbind = binder.Unbind(pubsub_msg);
  cloudevents_absl::StatusOr<PubsubMessage> bind = binder.Bind(*unbind, Format::kJson);

  ASSERT_TRUE(bind.ok());
  ASSERT_TRUE(MessageDifferencer::Equals(pubsub_msg, *bind));
}

TEST(UnbindBindTest, HttpReq_Binary) {
  HttpReqBinder http_req_binder;
  HttpRequest http_req;
  http_req.base().set("ce-id", "1");
  http_req.base().set("ce-source", "2");
  http_req.base().set("ce-specversion", "3");
  http_req.base().set("ce-type", "4");
  http_req.body() = "1010";

  cloudevents_absl::StatusOr<CloudEvent> unbind = http_req_binder.Unbind(http_req);
  cloudevents_absl::StatusOr<HttpRequest> bind = http_req_binder.Bind(*unbind);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ(http_req.base()["ce-id"], (*bind).base()["ce-id"]);
  ASSERT_EQ(http_req.base()["ce-source"], (*bind).base()["ce-source"]);
  ASSERT_EQ(http_req.base()["ce-specversion"], (*bind).base()["ce-specversion"]);
  ASSERT_EQ(http_req.base()["ce-type"], (*bind).base()["ce-type"]);
  ASSERT_EQ(http_req.body(), (*bind).body());
}

TEST(UnbindBindTest, HttpReq_Structured) {
  HttpReqBinder http_req_binder;
  HttpRequest http_req;
  http_req.base().set("content-type", "application/cloudevents+json");
  http_req.body() = "{\n\t\"id\" : \"1\",\n\t\"source\" : \"/test\",\n\t\"specversion\" : \"1.0\",\n\t\"type\" : \"test\"\n}";

  cloudevents_absl::StatusOr<CloudEvent> unbind = http_req_binder.Unbind(http_req);
  cloudevents_absl::StatusOr<HttpRequest> bind = http_req_binder.Bind(*unbind, Format::kJson);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ(http_req.base()["content-type"], (*bind).base()["content-type"]);
  ASSERT_EQ(http_req.body(), (*bind).body());
}

TEST(UnbindBindTest, HttpRes_Binary) {
  HttpResBinder http_res_binder;
  HttpResponse http_res;
  http_res.base().set("ce-id", "1");
  http_res.base().set("ce-source", "2");
  http_res.base().set("ce-specversion", "3");
  http_res.base().set("ce-type", "4");
  cloudevents_absl::StatusOr<CloudEvent> unbind = http_res_binder.Unbind(http_res);
  cloudevents_absl::StatusOr<HttpResponse> bind = http_res_binder.Bind(*unbind);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ(http_res.base()["ce-id"], (*bind).base()["ce-id"]);
  ASSERT_EQ(http_res.base()["ce-source"], (*bind).base()["ce-source"]);
  ASSERT_EQ(http_res.base()["ce-specversion"], (*bind).base()["ce-specversion"]);
  ASSERT_EQ(http_res.base()["ce-type"], (*bind).base()["ce-type"]);
}

TEST(UnbindBindTest, HttpRes_Structured) {
  HttpResBinder http_res_binder;
  HttpResponse http_res;
  http_res.base().set("content-type", "application/cloudevents+json");
  http_res.body() = "{\n\t\"id\" : \"1\",\n\t\"source\" : \"/test\",\n\t\"specversion\" : \"1.0\",\n\t\"type\" : \"test\"\n}";

  cloudevents_absl::StatusOr<CloudEvent> unbind = http_res_binder.Unbind(http_res);
  cloudevents_absl::StatusOr<HttpResponse> bind = http_res_binder.Bind(*unbind, Format::kJson);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ(http_res.base()["content-type"], (*bind).base()["content-type"]);
  ASSERT_EQ(http_res.body(), (*bind).body());
}

}  // namespace binding
}  // namespace cloudevents
