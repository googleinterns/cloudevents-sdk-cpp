#include "format.h"
#include <iostream>

#include <gtest/gtest.h>

TEST(test, runs) {
    ASSERT_EQ(1,1);
}

TEST(marshaller, sce) {
    io::cloudevents::v1::CloudEvent ce;
    ce.set_id("9");
    ce.set_source("/wer/9");
    ce.set_spec_version("1");
    ce.set_type("test");

    structured_cloud_events::Marshaller m;

    structured_cloud_events::StructuredCloudEvent my_sce = mCreateStructuredCloudEvent(&ce, structured_cloud_events::CloudEventFormat::Json, "test");
    ASSERT_EQ(my_sce.GetCloudEvent(),ce);
    ASSERT_EQ(my_sce.GetSerializedCloudEvent(),"test");
}