#include "http_binder.h"

#include <gtest/gtest.h>

namespace cloudevents {
namespace binding {
using ::io::cloudevents::v1::CloudEvent;
using ::cloudevents::format::StructuredCloudEvent;
using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::formatter_util::FormatterUtil;

typedef boost::beast::http::request<boost::beast::http::string_body> HttpRequest;

// TEST(Dummy, Debug) {
//     request<string_body> req;
//     std::cerr << "EMPTY HEADERS" << req.base() << std::endl;
//     req.base().set("test","1");
//     std::cerr << "SET" << req.base() << std::endl;
//     req.body() = "test body";
//     std::cerr << "SET B: " << req.body() << std::endl;
// }

TEST(InScm, Empty) {
    HttpRequest req;
    Binder<HttpRequest> binder;
    
    absl::StatusOr<bool> inscm = binder.InStructuredContentMode(req);

    ASSERT_TRUE(inscm.ok());
    ASSERT_FALSE((*inscm));
}

TEST(InScm, Diff) {
    HttpRequest req;
    req.base().set("Content-Type", "rand");
    Binder<HttpRequest> binder;
    
    absl::StatusOr<bool> inscm = binder.InStructuredContentMode(req);

    ASSERT_TRUE(inscm.ok());
    ASSERT_FALSE((*inscm));
}

TEST(InScm, Valid) {
    HttpRequest req;
    req.base().set("Content-Type", "application/cloudevents+test");
    Binder<HttpRequest> binder;
    
    absl::StatusOr<bool> inscm = binder.InStructuredContentMode(req);

    ASSERT_TRUE(inscm.ok());
    ASSERT_TRUE((*inscm));
}

} // binding
} // cloudevents
