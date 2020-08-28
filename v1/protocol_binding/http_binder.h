#ifndef CLOUDEVENTSCPPSDK_V1_BINDING_HTTPBINDER_H
#define CLOUDEVENTSCPPSDK_V1_BINDING_HTTPBINDER_H

#include <boost/beast/http.hpp>

#include "proto/cloud_event.pb.h"
#include "third_party/statusor/statusor.h"
#include "v1/protocol_binding/binder.h"

namespace cloudevents {
namespace binding {

// Inheritence from template specialization to handle HttpMessages.
// For the full template code and comments,
// view `//v1/protocol_binding/binder.h`.
//
// Uses Boost beast's HTTP Request/ Response representation.
//
// User interacts with HttpReqBinder and HttpResBinder
// which are both child classes of HttpBinder.

template <bool IsReq>
class HttpBinder: public Binder<
  boost::beast::http::message<IsReq, boost::beast::http::string_body>> {
 private:
  absl::Status BindMetadata(const std::string& key,
    const io::cloudevents::v1::CloudEvent_CloudEventAttribute& val,
    boost::beast::http::message<
    IsReq, boost::beast::http::string_body>& http_msg) override;

  absl::Status BindDataBinary(const std::string& bin_data,
    boost::beast::http::message<
    IsReq, boost::beast::http::string_body>& http_msg) override;

  absl::Status BindDataText(const std::string& text_data,
    boost::beast::http::message<
    IsReq, boost::beast::http::string_body>& http_msg) override;

  absl::Status BindContentType(const std::string& contenttype,
    boost::beast::http::message<
    IsReq, boost::beast::http::string_body>& http_msg) override;

  absl::Status BindDataStructured(const std::string& payload,
    boost::beast::http::message<
    IsReq, boost::beast::http::string_body>& http_msg) override;

  absl::Status UnbindMetadata(
    const boost::beast::http::message<
    IsReq, boost::beast::http::string_body>& http_msg,
    io::cloudevents::v1::CloudEvent& cloud_event) override;

  absl::Status UnbindData(
    const boost::beast::http::message<
    IsReq, boost::beast::http::string_body>& http_msg,
    io::cloudevents::v1::CloudEvent& cloud_event) override;

  cloudevents_absl::StatusOr<std::string> GetContentType(
    const boost::beast::http::message<
    IsReq, boost::beast::http::string_body>& http_msg) override;

  cloudevents_absl::StatusOr<std::string> GetPayload(
    const boost::beast::http::message<
    IsReq, boost::beast::http::string_body>& http_msg) override;
};

// _____ Concrete Binders _____
using HttpReqBinder = HttpBinder<true>;
using HttpResBinder = HttpBinder<false>;


}  // namespace binding
}  // namespace cloudevents

#endif  // CLOUDEVENTSCPPSDK_V1_BINDING_HTTPBINDER_H
