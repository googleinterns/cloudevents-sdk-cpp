#ifndef CLOUDEVENTSCPPSDK_V1_BINDING_HTTPBINDER_H
#define CLOUDEVENTSCPPSDK_V1_BINDING_HTTPBINDER_H

#include <boost/beast/http.hpp>
#include "v1/protocol_binding/binder.h"

namespace cloudevents {
namespace binding {

// HTTP specialization
template <>
absl::StatusOr<bool> 
    Binder<boost::beast::http::request<boost::beast::http::string_body>>::InStructuredContentMode(
    boost::beast::http::request<boost::beast::http::string_body>& http_msg);

// template <>
// absl::StatusOr<cloudevents::format::Format> 
//     Binder<google::pubsub::v1::PubsubMessage>::GetFormat(
//     google::pubsub::v1::PubsubMessage& pubsub_msg);

// template <>
// absl::StatusOr<std::string> 
//     Binder<google::pubsub::v1::PubsubMessage>::GetPayload(
//     google::pubsub::v1::PubsubMessage& pubsub_msg);

// template <>
// absl::StatusOr<io::cloudevents::v1::CloudEvent> 
//     Binder<google::pubsub::v1::PubsubMessage>::UnbindBinary(
//     google::pubsub::v1::PubsubMessage& pubsub_msg);

// template <>
// absl::StatusOr<google::pubsub::v1::PubsubMessage> 
//     Binder<google::pubsub::v1::PubsubMessage>::BindBinary(
//     io::cloudevents::v1::CloudEvent& cloud_event);

// template <>
// absl::Status Binder<google::pubsub::v1::PubsubMessage>::SetContentType(
//     google::pubsub::v1::PubsubMessage& message, std::string contenttype);

// template <>
// absl::Status Binder<google::pubsub::v1::PubsubMessage>::SetPayload(
//     google::pubsub::v1::PubsubMessage& message, std::string payload);

} // binding
} // cloudevents

#endif // CLOUDEVENTSCPPSDK_V1_BINDING_HTTPBINDER_H
