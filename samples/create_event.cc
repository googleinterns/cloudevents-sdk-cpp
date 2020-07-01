#include "proto/cloud_event.pb.h"
#include <iostream>
int main() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    google::protobuf::ShutdownProtobufLibrary();
    std::cout << "sup" << std::endl;
    return 0;
}