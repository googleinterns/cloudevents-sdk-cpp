#include "structured_cloud_event.h"
#include <iostream>

#include <gtest/gtest.h>
namespace cloud_events {
namespace format {

TEST(test, runs) {
    ASSERT_EQ(1,1);
}

TEST(StructuredCloudEvent, constructor) {
    StructuredCloudEvent sce1 = StructuredCloudEvent(CloudEventFormat::JSON, "test");
    StructuredCloudEvent sce2 = StructuredCloudEvent(CloudEventFormat::JSON, "");
    StructuredCloudEvent sce3 = StructuredCloudEvent(CloudEventFormat::JSON, "etrfyguhjnmhugyutxrxcjkuygilhojihguyrtesyrstduyfkgl");
}

TEST(StructuredCloudEvent, GetFormat) {
    StructuredCloudEvent sce1 = StructuredCloudEvent(CloudEventFormat::JSON, "test");
    ASSERT_EQ(sce1.GetCloudEventFormat(), CloudEventFormat::JSON);
    
    StructuredCloudEvent sce2 = StructuredCloudEvent(CloudEventFormat::JSON, "");
    ASSERT_EQ(sce2.GetCloudEventFormat(), CloudEventFormat::JSON);
    
    StructuredCloudEvent sce3 = StructuredCloudEvent(CloudEventFormat::JSON, "etrfyguhjnmhugyutxrxcjkuygilhojihguyrtesyrstduyfkgl");
    ASSERT_EQ(sce3.GetCloudEventFormat(), CloudEventFormat::JSON);
}

TEST(StructuredCloudEvent, GetSerialized) {
    StructuredCloudEvent sce1 = StructuredCloudEvent(CloudEventFormat::JSON, "test");
    ASSERT_EQ(sce1.GetSerializedCloudEvent(), "test");

    StructuredCloudEvent sce2 = StructuredCloudEvent(CloudEventFormat::JSON, "");
    ASSERT_EQ(sce2.GetSerializedCloudEvent(), "");

    StructuredCloudEvent sce3 = StructuredCloudEvent(CloudEventFormat::JSON, "etrfyguhjnmhugyutxrxcjkuygilhojihguyrtesyrstduyfkgl");
    ASSERT_EQ(sce3.GetSerializedCloudEvent(), "etrfyguhjnmhugyutxrxcjkuygilhojihguyrtesyrstduyfkgl");
}
} // format
} // cloud_events

