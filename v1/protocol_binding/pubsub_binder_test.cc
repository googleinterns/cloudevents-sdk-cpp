#include "pubsub_binder.h"

#include <gtest/gtest.h>

namespace cloudevents {
namespace binding {
using ::google::pubsub::v1::PubsubMessage;
using ::io::cloudevents::v1::CloudEvent;
using ::cloudevents::format::StructuredCloudEvent;
using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::formatter_util::FormatterUtil;

TEST(Bind, Invalid) {
    absl::StatusOr<PubsubMessage> bind;
    Binder<PubsubMessage> binder;
    CloudEvent ce;
    bind = binder.Bind(ce);

    ASSERT_FALSE(bind.ok());
    ASSERT_TRUE(absl::IsInvalidArgument(bind.status()));
}

TEST(Bind, Binary_Required) {
    absl::StatusOr<PubsubMessage> bind;
    Binder<PubsubMessage> binder;
    CloudEvent ce;
    ce.set_id("1");
    ce.set_source("2");
    ce.set_spec_version("3");
    ce.set_type("4");

    bind = binder.Bind(ce);

    ASSERT_TRUE(bind.ok());
    ASSERT_EQ((*bind).attributes().at("ce-id"), "1");
    ASSERT_EQ((*bind).attributes().at("ce-source"), "2");
    ASSERT_EQ((*bind).attributes().at("ce-spec_version"), "3");
    ASSERT_EQ((*bind).attributes().at("ce-type"), "4");
}

TEST(Bind, Structured_Required) {
    absl::StatusOr<PubsubMessage> bind;
    Binder<PubsubMessage> binder;
    CloudEvent ce;
    ce.set_id("1");
    ce.set_source("2");
    ce.set_spec_version("3");
    ce.set_type("4");

    bind = binder.Bind(ce,Format::kJson);

    ASSERT_TRUE(bind.ok());
    ASSERT_EQ((*bind).attributes().at("content-type"), "application/cloudevents+json");
    ASSERT_EQ((*bind).data(), "{\n\t\"id\" : \"1\",\n\t\"source\" : \"2\",\n\t\"spec_version\" : \"3\",\n\t\"type\" : \"4\"\n}");
}

TEST(Unbind, Binary_Required) {
    absl::StatusOr<CloudEvent> unbind;
    Binder<PubsubMessage> binder;
    PubsubMessage pmsg;
    (*pmsg.mutable_attributes())["ce-id"] = "1";
    (*pmsg.mutable_attributes())["ce-source"] = "2";
    (*pmsg.mutable_attributes())["ce-spec_version"] = "3";
    (*pmsg.mutable_attributes())["ce-type"] = "4";

    unbind = binder.Unbind(pmsg);

    ASSERT_TRUE(unbind.ok());
    ASSERT_EQ((*unbind).id(), "1");
    ASSERT_EQ((*unbind).source(), "2");
    ASSERT_EQ((*unbind).spec_version(), "3");
    ASSERT_EQ((*unbind).type(), "4");
}

// DEV NOTE: currently failing due to issues with the Base64 lib
TEST(Unbind, Structured_Required) {
    absl::StatusOr<CloudEvent> unbind;
    Binder<PubsubMessage> binder;
    PubsubMessage pmsg;
    (*pmsg.mutable_attributes())["content-type"] = "application/cloudevents+json";
    pmsg.set_data("e1xuXHRcImlkXCIgOiBcIjFcIixcblx0XCJzb3VyY2VcIiA6IFwiMlwiLFxuXHRcInNwZWNfdmVyc2lvblwiIDogXCIzXCIsXG5cdFwidHlwZVwiIDogXCI0XCJcbn0=");
    unbind = binder.Unbind(pmsg);
    ASSERT_TRUE(unbind.ok());
    ASSERT_EQ((*unbind).id(), "1");
    ASSERT_EQ((*unbind).source(), "2");
    ASSERT_EQ((*unbind).spec_version(), "3");
    ASSERT_EQ((*unbind).type(), "4");
}

} // binding
} // cloudevents