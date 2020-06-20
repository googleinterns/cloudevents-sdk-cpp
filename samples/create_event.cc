#include "proto/spec.pb.h"
#include <fstream>
#include <iostream>

void CreateEvent(io::cloudevents::v1::CloudEvent* event){
    // TODO (Michelle): Abstract this out to a Buider with validation.

    
    // std::map<std::string, std::string* (*)()> KEYTOFUNC;
    // KEYTOFUNC["id"] = &event.mutable_id;
    // KEYTOFUNC["source"] = &event.mutable_source;
    // KEYTOFUNC["spec_verion"] = &event.mutable_spec_verion;
    // KEYTOFUNC["type"] = &event.mutable_type;

    // for (const std::string& key : {"id", "source", "spec_version", "type"}){
    //     std::string res;
    //     std::cout << "Enter " << key << ": ";
    //     getline(std::cin, KEYTOFUNC[key]);
    // }

    std::cout << "Enter " << "id" << ": ";
    getline(std::cin, *event -> mutable_id());

    std::cout << "Enter " << "source" << ": ";
    getline(std::cin, *event -> mutable_source());

    std::cout << "Enter " << "spec_version" << ": ";
    getline(std::cin, *event -> mutable_spec_version());

    std::cout << "Enter " << "type" << ": ";
    getline(std::cin, *event -> mutable_type());

    std::string data_present;
    std::cout << "Would you like to enter data?: " << "(y/n)";
    getline(std::cin, data_present);
    if (data_present=="y") {
        std::string datatype;
        std::cout << "Enter " << "datatype" << ": (bytes/ string/ other)";
        getline(std::cin, datatype);
        std::cout << "Enter " << "data" << ": ";
        if (datatype == "bytes") {
            // TODO (Michelle): Fix reading binary from cin
            //getline(std::cin, *event -> mutable_binary_data());
            std::cerr << "Not yet supported." << std::endl;
        } else if (datatype=="string") {
            getline(std::cin, *event -> mutable_text_data());
        } else {
            std::cerr << "Not yet supported." << std::endl;
            // TODO (Michelle): Fix any
            //getline(std::cin, event.add_proto_data() -> PackFrom());
        }
    }
}

int main(int argc, char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    // ensure that a write file is specified
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " FILE" << std::endl;
        return -1;
    }

    // ensure streams are ok
    io::cloudevents::v1::CloudEvent event;

    std::fstream input(argv[1], std::ios::in | std::ios::binary);
    if (input) {
        std::cout << "File already exists. Overwrite? (y/n)" << std::endl;
        std::string cont;
        getline(std::cin,cont);
        if (cont != "y"){
            std::cout << "Terminating." << std::endl;
            return -1;
        }
    }

    // create an event
    // TODO (Michelle): handle optional and extension attrs
    CreateEvent(&event);

    // write to file
    std::fstream output(argv[1], std::ios::out | std::ios::trunc | std::ios::binary);
    if (!event.SerializeToOstream(&output)) {
        std::cerr << "Could not write to file given." << std::endl;
        return -1;
    }

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}