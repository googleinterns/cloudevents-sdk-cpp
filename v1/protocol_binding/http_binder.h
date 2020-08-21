#ifndef CLOUDEVENTSCPPSDK_V1_BINDING_HTTPBINDER_H
#define CLOUDEVENTSCPPSDK_V1_BINDING_HTTPBINDER_H

#include <boost/beast/http.hpp>
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

}  // namespace binding
}  // namespace cloudevents

#endif  // CLOUDEVENTSCPPSDK_V1_BINDING_HTTPBINDER_H
