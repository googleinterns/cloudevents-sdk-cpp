#include "proto/spec.pb.h"
#include "third_party/statusor/statusor.h"
#include <fstream>
#include <iostream>

void GetUserInput(std::string prompt, std::string& res) {
    std::cout << prompt << ": ";
    getline(std::cin, res);
}

int OutputToInterface(std::string content, int type) {
    switch(type) {
        case 0: std::cout << content << std::endl; break;
        case -1: std::cerr << content << std::endl; break;
        default: {
            return -1;
        }
    }
    return 0;
}

absl::Status ValidateFile(std::string file_path){
    std::fstream input(file_path, std::ios::in | std::ios::binary);
    if (input) {
        std::string cont;
        GetUserInput("File already exists. Overwrite? (y/n)", cont);
        if (cont != "y"){
            return absl::CancelledError();
        }
    }
    return absl::OkStatus();
}


absl::Status WriteToFile(std::string file_path, io::cloudevents::v1::CloudEvent* event){
    std::fstream output(file_path, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!event -> SerializeToOstream(&output)) {
        return absl::InvalidArgumentError("Could not write to file given.");
    }
    return absl::OkStatus();
}

absl::Status PopulateEvent(io::cloudevents::v1::CloudEvent* event){
    // TODO (Michelle): Abstract this out to a Buider with validation.

    GetUserInput("Enter id", *event -> mutable_id());
    GetUserInput("Enter source", *event -> mutable_source());
    GetUserInput("Enter spec_version", *event -> mutable_spec_version());
    GetUserInput("Enter type", *event -> mutable_type());

    std::string has_data;
    GetUserInput("Would you like to enter data (y/n)", has_data);
    if (has_data=="y") {
        std::string data_type;
        // TODO (Michelle): Support Any data
        GetUserInput("Enter data type (bytes/ string)", data_type);
        const static std::unordered_map<std::string,int> data_type_to_case{
            {"bytes",1},
            {"string",2},
            {"other", 3}
        };

        if (data_type_to_case.find(data_type) == data_type_to_case.end()) {
            return absl::InvalidArgumentError("Data type not recognized.");
        }
        
        switch(data_type_to_case.at(data_type)){
            case 1: GetUserInput("Enter data", *event -> mutable_binary_data()); break;
            case 2: GetUserInput("Enter data", *event -> mutable_text_data()); break;
            case 3: return absl::UnimplementedError("Other data type not supported yet.");
            default: {
                return absl::InternalError("Data type mapping resulted in unknown case.");
            }
        }
    }
    return absl::OkStatus();
}

int main(int argc, char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    absl::Status program_status;
    
    // ensure that a write file is specified
    if (argc != 2) {
        OutputToInterface("Incorrect Usage. Please specify a write file.", -1);
        return -1;
    }

    io::cloudevents::v1::CloudEvent event;

    program_status = ValidateFile(argv[1]);
    if (!program_status.ok()) {
        OutputToInterface(program_status.ToString(), -1);
        return -1;
    }
    
    // create an event
    // TODO (Michelle): handle optional and extension attrs
    program_status = PopulateEvent(&event);
    if (!program_status.ok()) {
        OutputToInterface(program_status.ToString(), -1);
        return -1;
    }

    // write to file
    program_status = WriteToFile(argv[1], &event);
    if (!program_status.ok()) {
        OutputToInterface(program_status.ToString(), -1);
        return -1;
    }

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}