#include "http_binder.h"

#include <gtest/gtest.h>

namespace cloudevents {
namespace binding {
using ::boost::beast::http::request;
using ::boost::beast::http::string_body;
using ::io::cloudevents::v1::CloudEvent;
using ::cloudevents::format::StructuredCloudEvent;
using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::formatter_util::FormatterUtil;

// TEST(Dummy, Debug) {
//     request<string_body> req;
//     std::cerr << "EMPTY HEADERS" << req.base() << std::endl;
//     req.base().set("test","1");
//     std::cerr << "SET" << req.base() << std::endl;
//     req.body() = "test body";
//     std::cerr << "SET B: " << req.body() << std::endl;
// }

} // binding
} // cloudevents
