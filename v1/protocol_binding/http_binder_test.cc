#include "http_binder.h"

#include <gtest/gtest.h>

namespace cloudevents {
namespace binding {

using ::io::cloudevents::v1::CloudEvent;
using ::cloudevents::format::Format;
using ::boost::beast::http::message;
using ::boost::beast::http::string_body;

typedef io::cloudevents::v1::CloudEvent_CloudEventAttribute CeAttr;
typedef boost::beast::http::request<string_body> HttpRequest;
typedef boost::beast::http::response<string_body> HttpResponse;

// Setup a valid CloudEvent
class BindTest : public ::testing::Test {
 protected:
  void SetUp() override {
    ce.set_id("1");
    ce.set_source("2");
    ce.set_spec_version("3");
    ce.set_type("4");
  }
  CloudEvent ce;
};

// Setup a Binary Http Req containing valid CloudEvent
class UnbindBinaryReqTest : public ::testing::Test {
 protected:
  void SetUp() override {
    http_req.base().set("ce-id", "1");
    http_req.base().set("ce-source", "2");
    http_req.base().set("ce-specversion", "3");
    http_req.base().set("ce-type", "4");
  }
  HttpReqBinder binder;
  HttpRequest http_req;
};

// Setup a Structured Http Req containing valid CloudEvent
class UnbindStructuredReqTest : public ::testing::Test {
 protected:
  void SetUp() override {
    http_req.base().set("content-type", "application/cloudevents+json");
  }
  std::string valid_payload = "{\n\t\"id\" : \"1\",\n\t\"source\" : \"/test\",\n\t\"specversion\" : \"1.0\",\n\t\"type\" : \"test\"\n}";
  HttpReqBinder binder;
  HttpRequest http_req;
};

// Setup a Binary Http Res containing valid CloudEvent
class UnbindBinaryResTest : public ::testing::Test {
 protected:
  void SetUp() override {
    http_res.base().set("ce-id", "1");
    http_res.base().set("ce-source", "2");
    http_res.base().set("ce-specversion", "3");
    http_res.base().set("ce-type", "4");
  }
  HttpResBinder binder;
  HttpResponse http_res;
};

// Setup a Structured Http Res containing valid CloudEvent
class UnbindStructuredResTest : public ::testing::Test {
 protected:
  void SetUp() override {
    http_res.base().set("content-type", "application/cloudevents+json");
  }
  std::string valid_payload = "{\n\t\"id\" : \"1\",\n\t\"source\" : \"/test\",\n\t\"specversion\" : \"1.0\",\n\t\"type\" : \"test\"\n}";
  HttpResBinder binder;
  HttpResponse http_res;
};


// Tests not using the Fixture are named differently
// as TEST and TEST_F cannot share the same test name.
TEST(BindReqTest, Invalid) {
  HttpReqBinder binder;
  CloudEvent ce;
  cloudevents_absl::StatusOr<HttpRequest> bind = binder.Bind(ce);

  ASSERT_FALSE(bind.ok());
  ASSERT_TRUE(absl::IsInvalidArgument(bind.status()));
}

TEST_F(BindTest, BinaryReq_Required) {
  HttpReqBinder binder;
  cloudevents_absl::StatusOr<HttpRequest> bind = binder.Bind(ce);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["ce-id"], "1");
  ASSERT_EQ((*bind).base()["ce-source"], "2");
  ASSERT_EQ((*bind).base()["ce-specversion"], "3");
  ASSERT_EQ((*bind).base()["ce-type"], "4");
}

TEST_F(BindTest, BinaryReq_Optional) {
  CeAttr attr_val;
  attr_val.set_ce_string("5");
  (*ce.mutable_attributes())["opt"] = attr_val;
  HttpReqBinder binder;
  cloudevents_absl::StatusOr<HttpRequest> bind = binder.Bind(ce);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["ce-id"], "1");
  ASSERT_EQ((*bind).base()["ce-source"], "2");
  ASSERT_EQ((*bind).base()["ce-specversion"], "3");
  ASSERT_EQ((*bind).base()["ce-type"], "4");
}

TEST_F(BindTest, BinaryReq_BinData) {
  HttpReqBinder binder;
  ce.set_binary_data("1010");

  cloudevents_absl::StatusOr<HttpRequest> bind = binder.Bind(ce);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["ce-id"], "1");
  ASSERT_EQ((*bind).base()["ce-source"], "2");
  ASSERT_EQ((*bind).base()["ce-specversion"], "3");
  ASSERT_EQ((*bind).base()["ce-type"], "4");
  ASSERT_EQ((*bind).body(), "1010");
}

TEST_F(BindTest, BinaryReq_TextData) {
  HttpReqBinder binder;
  ce.set_text_data("hello");

  cloudevents_absl::StatusOr<HttpRequest> bind = binder.Bind(ce);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["ce-id"], "1");
  ASSERT_EQ((*bind).base()["ce-source"], "2");
  ASSERT_EQ((*bind).base()["ce-specversion"], "3");
  ASSERT_EQ((*bind).base()["ce-type"], "4");
  ASSERT_EQ((*bind).body(), "hello");
}

TEST_F(BindTest, StructuredReq_Required) {
  HttpReqBinder binder;

  cloudevents_absl::StatusOr<HttpRequest> bind = binder.Bind(ce, Format::kJson);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["content-type"], "application/cloudevents+json");
  ASSERT_EQ((*bind).body(), "{\n\t\"id\" : \"1\",\n\t\"source\" : \"2\",\n\t\"specversion\" : \"3\",\n\t\"type\" : \"4\"\n}");
}

TEST_F(BindTest, StructuredReq_Optional) {
  HttpReqBinder binder;
  CeAttr attr_val;
  attr_val.set_ce_string("5");
  (*ce.mutable_attributes())["opt"] = attr_val;

  cloudevents_absl::StatusOr<HttpRequest> bind = binder.Bind(ce,
    Format::kJson);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["content-type"], "application/cloudevents+json");
  ASSERT_EQ((*bind).body(), "{\n\t\"id\" : \"1\",\n\t\"opt\" : \"5\",\n\t\"source\" : \"2\",\n\t\"specversion\" : \"3\",\n\t\"type\" : \"4\"\n}");
}

TEST_F(BindTest, StructuredReq_BinData) {
  HttpReqBinder binder;
  ce.set_binary_data("1010");

  cloudevents_absl::StatusOr<HttpRequest> bind = binder.Bind(ce,
    Format::kJson);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["content-type"], "application/cloudevents+json");
  ASSERT_EQ((*bind).body(), "{\n\t\"data_base64\" : \"1010\",\n\t\"id\" : \"1\",\n\t\"source\" : \"2\",\n\t\"specversion\" : \"3\",\n\t\"type\" : \"4\"\n}");
}

TEST_F(BindTest, StructuredReq_TextData) {
  HttpReqBinder binder;
  ce.set_text_data("hello");

  cloudevents_absl::StatusOr<HttpRequest> bind = binder.Bind(ce,
    Format::kJson);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["content-type"], "application/cloudevents+json");
  ASSERT_EQ((*bind).body(), "{\n\t\"data\" : \"hello\",\n\t\"id\" : \"1\",\n\t\"source\" : \"2\",\n\t\"specversion\" : \"3\",\n\t\"type\" : \"4\"\n}");
}

TEST_F(UnbindBinaryReqTest, Required) {
  cloudevents_absl::StatusOr<CloudEvent> unbind = binder.Unbind(http_req);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "2");
  ASSERT_EQ((*unbind).spec_version(), "3");
  ASSERT_EQ((*unbind).type(), "4");
}

TEST_F(UnbindBinaryReqTest, Optional) {
  http_req.base().set("ce-opt", "5");

  cloudevents_absl::StatusOr<CloudEvent> unbind = binder.Unbind(http_req);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "2");
  ASSERT_EQ((*unbind).spec_version(), "3");
  ASSERT_EQ((*unbind).type(), "4");
  ASSERT_EQ((*unbind).attributes().at("opt").ce_string(), "5");
}

TEST_F(UnbindBinaryReqTest, Data) {
  http_req.body() = "1010";

  cloudevents_absl::StatusOr<CloudEvent> unbind = binder.Unbind(http_req);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "2");
  ASSERT_EQ((*unbind).spec_version(), "3");
  ASSERT_EQ((*unbind).type(), "4");
  ASSERT_EQ((*unbind).binary_data(), "1010");
}

TEST_F(UnbindStructuredReqTest, Required) {
  http_req.body() = valid_payload;

  cloudevents_absl::StatusOr<CloudEvent> unbind = binder.Unbind(http_req);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "/test");
  ASSERT_EQ((*unbind).spec_version(), "1.0");
  ASSERT_EQ((*unbind).type(), "test");
}

TEST_F(UnbindStructuredReqTest, Optional) {
  std::string payload = valid_payload;
  payload.insert(1, "\t\"opt\" : \"5\",\n");
  http_req.body() = payload;

  cloudevents_absl::StatusOr<CloudEvent> unbind = binder.Unbind(http_req);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "/test");
  ASSERT_EQ((*unbind).spec_version(), "1.0");
  ASSERT_EQ((*unbind).type(), "test");
  ASSERT_EQ((*unbind).attributes().at("opt").ce_string(), "5");
}

TEST_F(UnbindStructuredReqTest, BinData) {
  std::string payload = valid_payload;
  payload.insert(1, "\t\"data_base64\" : \"1010\",\n");
  http_req.body() = payload;

  cloudevents_absl::StatusOr<CloudEvent> unbind = binder.Unbind(http_req);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "/test");
  ASSERT_EQ((*unbind).spec_version(), "1.0");
  ASSERT_EQ((*unbind).type(), "test");
  ASSERT_EQ((*unbind).binary_data(), "1010");
}

TEST_F(UnbindStructuredReqTest, TextData) {
  std::string payload = valid_payload;
  payload.insert(1, "\t\"data\" : \"hello\",\n");
  http_req.body() = payload;

  cloudevents_absl::StatusOr<CloudEvent> unbind = binder.Unbind(http_req);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "/test");
  ASSERT_EQ((*unbind).spec_version(), "1.0");
  ASSERT_EQ((*unbind).type(), "test");
  ASSERT_EQ((*unbind).text_data(), "hello");
}

TEST(BindResTest, Invalid) {
  HttpResBinder binder;
  CloudEvent ce;
  cloudevents_absl::StatusOr<HttpResponse> bind = binder.Bind(ce);

  ASSERT_FALSE(bind.ok());
  ASSERT_TRUE(absl::IsInvalidArgument(bind.status()));
}

TEST_F(BindTest, BinaryRes_Required) {
  HttpResBinder binder;

  cloudevents_absl::StatusOr<HttpResponse> bind = binder.Bind(ce);

  std::cerr << bind.status();
  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["ce-id"], "1");
  ASSERT_EQ((*bind).base()["ce-source"], "2");
  ASSERT_EQ((*bind).base()["ce-specversion"], "3");
  ASSERT_EQ((*bind).base()["ce-type"], "4");
}

TEST_F(BindTest, BinaryRes_Optional) {
  HttpResBinder binder;
  CeAttr attr_val;
  attr_val.set_ce_string("5");
  (*ce.mutable_attributes())["opt"] = attr_val;

  cloudevents_absl::StatusOr<HttpResponse> bind = binder.Bind(ce);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["ce-id"], "1");
  ASSERT_EQ((*bind).base()["ce-source"], "2");
  ASSERT_EQ((*bind).base()["ce-specversion"], "3");
  ASSERT_EQ((*bind).base()["ce-type"], "4");
  ASSERT_EQ((*bind).base()["ce-opt"], "5");
}

TEST_F(BindTest, BinaryRes_BinData) {
  HttpResBinder binder;
  ce.set_binary_data("1010");

  cloudevents_absl::StatusOr<HttpResponse> bind = binder.Bind(ce);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["ce-id"], "1");
  ASSERT_EQ((*bind).base()["ce-source"], "2");
  ASSERT_EQ((*bind).base()["ce-specversion"], "3");
  ASSERT_EQ((*bind).base()["ce-type"], "4");
  ASSERT_EQ((*bind).body(), "1010");
}

TEST_F(BindTest, BinaryRes_TextData) {
  HttpResBinder binder;
  ce.set_text_data("hello");

  cloudevents_absl::StatusOr<HttpResponse> bind = binder.Bind(ce);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["ce-id"], "1");
  ASSERT_EQ((*bind).base()["ce-source"], "2");
  ASSERT_EQ((*bind).base()["ce-specversion"], "3");
  ASSERT_EQ((*bind).base()["ce-type"], "4");
  ASSERT_EQ((*bind).body(), "hello");
}

TEST_F(BindTest, StructuredRes_Required) {
  HttpResBinder binder;

  cloudevents_absl::StatusOr<HttpResponse> bind = binder.Bind(ce,
    Format::kJson);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["content-type"], "application/cloudevents+json");
  ASSERT_EQ((*bind).body(), "{\n\t\"id\" : \"1\",\n\t\"source\" : \"2\",\n\t\"specversion\" : \"3\",\n\t\"type\" : \"4\"\n}");
}

TEST_F(BindTest, StructuredRes_Optional) {
  HttpResBinder binder;
  CeAttr attr_val;
  attr_val.set_ce_string("5");
  (*ce.mutable_attributes())["opt"] = attr_val;

  cloudevents_absl::StatusOr<HttpResponse> bind = binder.Bind(ce,
    Format::kJson);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["content-type"], "application/cloudevents+json");
  ASSERT_EQ((*bind).body(), "{\n\t\"id\" : \"1\",\n\t\"opt\" : \"5\",\n\t\"source\" : \"2\",\n\t\"specversion\" : \"3\",\n\t\"type\" : \"4\"\n}");
}

TEST_F(BindTest, StructuredRes_BinData) {
  HttpResBinder binder;
  ce.set_binary_data("1010");

  cloudevents_absl::StatusOr<HttpResponse> bind = binder.Bind(ce,
    Format::kJson);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["content-type"], "application/cloudevents+json");
  ASSERT_EQ((*bind).body(), "{\n\t\"data_base64\" : \"1010\",\n\t\"id\" : \"1\",\n\t\"source\" : \"2\",\n\t\"specversion\" : \"3\",\n\t\"type\" : \"4\"\n}");
}

TEST_F(BindTest, StructuredRes_TextData) {
  HttpResBinder binder;
  ce.set_text_data("hello");

  cloudevents_absl::StatusOr<HttpResponse> bind = binder.Bind(ce,
    Format::kJson);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["content-type"], "application/cloudevents+json");
  ASSERT_EQ((*bind).body(), "{\n\t\"data\" : \"hello\",\n\t\"id\" : \"1\",\n\t\"source\" : \"2\",\n\t\"specversion\" : \"3\",\n\t\"type\" : \"4\"\n}");
}

TEST_F(UnbindBinaryResTest, Required) {
  cloudevents_absl::StatusOr<CloudEvent> unbind = binder.Unbind(http_res);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "2");
  ASSERT_EQ((*unbind).spec_version(), "3");
  ASSERT_EQ((*unbind).type(), "4");
}

TEST_F(UnbindBinaryResTest, Optional) {
  http_res.base().set("ce-opt", "5");

  cloudevents_absl::StatusOr<CloudEvent> unbind = binder.Unbind(http_res);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "2");
  ASSERT_EQ((*unbind).spec_version(), "3");
  ASSERT_EQ((*unbind).type(), "4");
  ASSERT_EQ((*unbind).attributes().at("opt").ce_string(), "5");
}

TEST_F(UnbindBinaryResTest, Data) {
  http_res.body() = "1010";

  cloudevents_absl::StatusOr<CloudEvent> unbind = binder.Unbind(http_res);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "2");
  ASSERT_EQ((*unbind).spec_version(), "3");
  ASSERT_EQ((*unbind).type(), "4");
  ASSERT_EQ((*unbind).binary_data(), "1010");
}

TEST_F(UnbindStructuredResTest, Required) {
  http_res.body() = valid_payload;

  cloudevents_absl::StatusOr<CloudEvent> unbind = binder.Unbind(http_res);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "/test");
  ASSERT_EQ((*unbind).spec_version(), "1.0");
  ASSERT_EQ((*unbind).type(), "test");
}

TEST_F(UnbindStructuredResTest, Optional) {
  std::string payload = valid_payload;
  payload.insert(1, "\t\"opt\" : \"5\",\n");
  http_res.body() = payload;

  cloudevents_absl::StatusOr<CloudEvent> unbind = binder.Unbind(http_res);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "/test");
  ASSERT_EQ((*unbind).spec_version(), "1.0");
  ASSERT_EQ((*unbind).type(), "test");
  ASSERT_EQ((*unbind).attributes().at("opt").ce_string(), "5");
}

TEST_F(UnbindStructuredResTest, BinData) {
  std::string payload = valid_payload;
  payload.insert(1, "\t\"data_base64\" : \"1010\",\n");
  http_res.body() = payload;

  cloudevents_absl::StatusOr<CloudEvent> unbind = binder.Unbind(http_res);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "/test");
  ASSERT_EQ((*unbind).spec_version(), "1.0");
  ASSERT_EQ((*unbind).type(), "test");
  ASSERT_EQ((*unbind).binary_data(), "1010");
}

TEST_F(UnbindStructuredResTest, TetData) {
  std::string payload = valid_payload;
  payload.insert(1, "\t\"data\" : \"hello\",\n");
  http_res.body() = payload;

  cloudevents_absl::StatusOr<CloudEvent> unbind = binder.Unbind(http_res);

  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "/test");
  ASSERT_EQ((*unbind).spec_version(), "1.0");
  ASSERT_EQ((*unbind).type(), "test");
  ASSERT_EQ((*unbind).text_data(), "hello");
}

}  // namespace binding
}  // namespace cloudevents
