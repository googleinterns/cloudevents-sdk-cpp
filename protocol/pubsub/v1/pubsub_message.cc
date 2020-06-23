#include "protocol/pubsub/v1/pubsub_message.h"

namespace pubsub {

std::string PubsubMessage::Marshal(io::cloudevents::v1::CloudEvent* event) {
    return "hw";
}

// io::cloudevents::v1::CloudEvent PubsubMessage::Unmarshal(std::string* message){
// }

} // namespace pubsub