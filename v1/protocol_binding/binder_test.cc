#include "binder.h"

#include <gtest/gtest.h>

namespace cloudevents {
namespace binding {
using ::google::pubsub::v1::PubsubMessage;
using ::io::cloudevents::v1::CloudEvent;
using ::cloudevents::format::StructuredCloudEvent;
using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::formatter_util::FormatterUtil;

TEST(General, GetFormat) {
    StructuredCloudEvent sce;
    Binder<StructuredCloudEvent> binder;

    ASSERT_TRUE(absl::IsInternal(binder.GetFormat(&sce).status()));
}

TEST(PubsubBinder, InStructuredContentMode_Structured) {
    PubsubMessage pm;
    (*pm.mutable_attributes())["content-type"] = "application/cloudevents+test";
    Binder<PubsubMessage> binder;
    absl::StatusOr<bool>  in_scm;

    in_scm = binder.InStructuredContentMode(&pm);

    ASSERT_TRUE(in_scm.ok());
    ASSERT_TRUE(*in_scm);
}

TEST(PubsubBinder, InStructuredContentMode_RandomContent) {
    PubsubMessage pm;
    (*pm.mutable_attributes())["content-type"] = "random-media";
    Binder<PubsubMessage> binder;
    absl::StatusOr<bool>  in_scm;

    in_scm = binder.InStructuredContentMode(&pm);

    ASSERT_TRUE(in_scm.ok());
    ASSERT_FALSE(*in_scm);
}

TEST(PubsubBinder, InStructuredContentMode_Malformed) {
    PubsubMessage pm;
    (*pm.mutable_attributes())["content-type"] = "wrong/application/cloudevents+test";
    Binder<PubsubMessage> binder;
    absl::StatusOr<bool>  in_scm;

    in_scm = binder.InStructuredContentMode(&pm);

    ASSERT_TRUE(in_scm.ok());
    ASSERT_FALSE(*in_scm);
}

TEST(PubsubBinder, InStructuredContentMode_NoContent) {
    PubsubMessage pm;
    Binder<PubsubMessage> binder;
    absl::StatusOr<bool>  in_scm;

    in_scm = binder.InStructuredContentMode(&pm);

    ASSERT_TRUE(in_scm.ok());
    ASSERT_FALSE(*in_scm);
}


TEST(PubsubBinder, GetFormat_Json) {
    PubsubMessage pm;
    (*pm.mutable_attributes())["content-type"] = "application/cloudevents+json";
    Binder<PubsubMessage> binder;
    absl::StatusOr<Format> format;

    format = binder.GetFormat(&pm);

    ASSERT_TRUE(format.ok());
    ASSERT_EQ(*format, Format::kJson);
}

TEST(PubsubBinder, GetFormat_Malformed) {
    PubsubMessage pm;
    (*pm.mutable_attributes())["content-type"] = "application/cloudevents+test";
    Binder<PubsubMessage> binder;
    absl::StatusOr<Format> format;

    format = binder.GetFormat(&pm);

    ASSERT_FALSE(format.ok());
    ASSERT_TRUE(absl::IsInvalidArgument(format.status()));
}

TEST(PubsubBinder, GetPayload_Exists) {
    PubsubMessage pm;
    pm.set_data("dGVzdA==");
    Binder<PubsubMessage> binder;
    absl::StatusOr<std::string> payload;

    payload = binder.GetPayload(&pm);

    ASSERT_TRUE(payload.ok());
    ASSERT_EQ((*payload),"test");
}

TEST(PubsubBinder, GetPayload_Empty) {
    PubsubMessage pm;
    Binder<PubsubMessage> binder;
    absl::StatusOr<std::string> payload;

    payload = binder.GetPayload(&pm);

    ASSERT_TRUE(payload.ok());
    ASSERT_EQ((*payload),"");
}

TEST(PubsubBinder, UnbindBinary_Invalid) {
    PubsubMessage pm;
    Binder<PubsubMessage> binder;
    absl::StatusOr<CloudEvent> ce;

    ce = binder.UnbindBinary(&pm);

    ASSERT_FALSE(ce.ok());
    ASSERT_TRUE(absl::IsInvalidArgument(ce.status()));
}

TEST(PubsubBinder, UnbindBinary_Required) {
    PubsubMessage pm;
    (*pm.mutable_attributes())["ce-id"] = "1";
    (*pm.mutable_attributes())["ce-source"] = "2";
    (*pm.mutable_attributes())["ce-spec_version"] = "3";
    (*pm.mutable_attributes())["ce-type"] = "4";
    Binder<PubsubMessage> binder;
    absl::StatusOr<CloudEvent> ce;

    ce = binder.UnbindBinary(&pm);

    ASSERT_TRUE(ce.ok());
    ASSERT_EQ((*ce).id(), "1");
    ASSERT_EQ((*ce).source(), "2");
    ASSERT_EQ((*ce).spec_version(), "3");
    ASSERT_EQ((*ce).type(), "4");
}

TEST(PubsubBinder, UnbindBinary_Optional) {
    PubsubMessage pm;
    (*pm.mutable_attributes())["ce-id"] = "1";
    (*pm.mutable_attributes())["ce-source"] = "2";
    (*pm.mutable_attributes())["ce-spec_version"] = "3";
    (*pm.mutable_attributes())["ce-type"] = "4";
    (*pm.mutable_attributes())["ce-opt1"] = "5";
    (*pm.mutable_attributes())["ce-opt2"] = "6";
    Binder<PubsubMessage> binder;
    absl::StatusOr<CloudEvent> ce;
    ce = binder.UnbindBinary(&pm);

    ASSERT_TRUE(ce.ok());
    ASSERT_EQ((*ce).id(), "1");
    ASSERT_EQ((*ce).source(), "2");
    ASSERT_EQ((*ce).spec_version(), "3");
    ASSERT_EQ((*ce).type(), "4");
    ASSERT_EQ((*ce).attributes().at("opt1").ce_string(), "5");
    ASSERT_EQ((*ce).attributes().at("opt2").ce_string(), "6");

}

TEST(PubsubBinder, UnbindBinary_ContentType) {
    PubsubMessage pm;
    (*pm.mutable_attributes())["ce-id"] = "1";
    (*pm.mutable_attributes())["ce-source"] = "2";
    (*pm.mutable_attributes())["ce-spec_version"] = "3";
    (*pm.mutable_attributes())["ce-type"] = "4";
    (*pm.mutable_attributes())["content-type"] = "type";
    Binder<PubsubMessage> binder;
    absl::StatusOr<CloudEvent> ce;

    ce = binder.UnbindBinary(&pm);

    ASSERT_TRUE(ce.ok());
    ASSERT_EQ((*ce).id(), "1");
    ASSERT_EQ((*ce).source(), "2");
    ASSERT_EQ((*ce).spec_version(), "3");
    ASSERT_EQ((*ce).type(), "4");
    ASSERT_EQ((*ce).attributes().at("datacontenttype").ce_string(), "type");
}

TEST(PubsubBinder, UnbindBinary_Data) {
    PubsubMessage pm;
    (*pm.mutable_attributes())["ce-id"] = "1";
    (*pm.mutable_attributes())["ce-source"] = "2";
    (*pm.mutable_attributes())["ce-spec_version"] = "3";
    (*pm.mutable_attributes())["ce-type"] = "4";
    pm.set_data("dGVzdA==");
    Binder<PubsubMessage> binder;
    absl::StatusOr<CloudEvent> ce;

    ce = binder.UnbindBinary(&pm);

    ASSERT_TRUE(ce.ok());
    ASSERT_EQ((*ce).id(), "1");
    ASSERT_EQ((*ce).source(), "2");
    ASSERT_EQ((*ce).spec_version(), "3");
    ASSERT_EQ((*ce).type(), "4");
    ASSERT_EQ((*ce).binary_data(), "test");
}

TEST(PubsubBinder, BindStructured_Json) {
    StructuredCloudEvent sce;
    sce.format = Format::kJson;
    sce.serialization = "{test}";
    Binder<PubsubMessage> binder;

    absl::StatusOr<PubsubMessage> pubsub_msg; 
    pubsub_msg = binder.BindStructured(&sce);

    ASSERT_TRUE(pubsub_msg.ok());
    ASSERT_EQ((*pubsub_msg).attributes().at("content-type"), "application/cloudevents+json");
    ASSERT_EQ((*pubsub_msg).data(), "{test}");
}

} // binding
} // cloudevents
