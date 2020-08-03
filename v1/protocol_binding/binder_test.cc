#include "binder.h"

#include <gtest/gtest.h>

namespace cloudevents {
namespace binding {
using ::google::pubsub::v1::PubsubMessage;
using ::cloudevents::format::StructuredCloudEvent;
using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::formatter_util::FormatterUtil;

TEST(General, GetFormat) {
    StructuredCloudEvent sce;
    Binder<StructuredCloudEvent> binder;

    ASSERT_TRUE(absl::IsInternal(binder.GetFormat(&sce).status()));
}

TEST(Pubsub, InStructuredContentMode_Structured) {
    PubsubMessage pm;
    (*pm.mutable_attributes())["content-type"] = "application/cloudevents+test";
    Binder<PubsubMessage> binder;
    absl::StatusOr<bool>  in_scm;

    in_scm = binder.InStructuredContentMode(&pm);

    ASSERT_TRUE(in_scm.ok());
    ASSERT_TRUE(*in_scm);
}

TEST(Pubsub, InStructuredContentMode_RandomContent) {
    PubsubMessage pm;
    (*pm.mutable_attributes())["content-type"] = "random-media";
    Binder<PubsubMessage> binder;
    absl::StatusOr<bool>  in_scm;

    in_scm = binder.InStructuredContentMode(&pm);

    ASSERT_TRUE(in_scm.ok());
    ASSERT_FALSE(*in_scm);
}

TEST(Pubsub, InStructuredContentMode_Malformed) {
    PubsubMessage pm;
    (*pm.mutable_attributes())["content-type"] = "wrong/application/cloudevents+test";
    Binder<PubsubMessage> binder;
    absl::StatusOr<bool>  in_scm;

    in_scm = binder.InStructuredContentMode(&pm);

    ASSERT_TRUE(in_scm.ok());
    ASSERT_FALSE(*in_scm);
}

TEST(Pubsub, InStructuredContentMode_NoContent) {
    PubsubMessage pm;
    Binder<PubsubMessage> binder;
    absl::StatusOr<bool>  in_scm;

    in_scm = binder.InStructuredContentMode(&pm);

    ASSERT_TRUE(in_scm.ok());
    ASSERT_FALSE(*in_scm);
}


// TEST(Pubsub, GetFormat) {
//     PubsubMessage pm;
//     Binder<PubsubMessage> binder;
//     ASSERT_EQ((*binder.GetFormat(&pm)), Format::kJson);
// }


} // binding
} // cloudevents
