#ifndef PUBSUB_MESSAGE_H
#define PUBSUB_MESSAGE_H

//#include "v1/message_interface.h"
#include <string>
#include "proto/spec.pb.h"

namespace pubsub {

// TODO (Michelle): create standard Message Interface
//class PubsubMessage: public MessageInterface {
class PubsubMessage {
    public:
        std::string format_;
        std::string Marshal(io::cloudevents::v1::CloudEvent* event);
        // TODO (Michelle): Implement Unmarshall
        // io::cloudevents::v1::CloudEvent Unmarshal(std::string* message);
};

} // namespace pubsub
#endif
