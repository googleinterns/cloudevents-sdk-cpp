#include "proto/spec.pb.h"
#include <fstream>
#include <iostream>

void GetUserInput(std::string prompt, std::string& res) {
    std::cout << prompt << ": ";
    getline(std::cin, res);
}

void OutputToInterface(std::string content, bool isError) {
    if ( isError ) {
        std::cout << content << std::endl;
    } else {
        std::cout << content << std::endl;
    }
}

int ValidateFile(std::string file_path){
    std::fstream input(file_path, std::ios::in | std::ios::binary);
    if ( input ) {
        std::string cont;
        GetUserInput("File already exists. Overwrite? (y/n)", cont);
        if ( cont != "y" ){
            OutputToInterface("Terminating.", 0);
            return -1;
        }
    }
    return 0;
}


int WriteToFile(std::string file_path, io::cloudevents::v1::CloudEvent* event){
    std::fstream output(file_path, std::ios::out | std::ios::trunc | std::ios::binary);
    if ( !event -> SerializeToOstream(&output) ) {
        OutputToInterface("Could not write to file given.", -1);
        return -1;
    }
    return 0;
}

int CreateEvent(io::cloudevents::v1::CloudEvent* event){
    // TODO (Michelle): Abstract this out to a Buider with validation.

    GetUserInput("Enter id", *event -> mutable_id());
    GetUserInput("Enter source", *event -> mutable_source());
    GetUserInput("Enter spec_version", *event -> mutable_spec_version());
    GetUserInput("Enter type", *event -> mutable_type());

    std::string has_data;
    GetUserInput("Would you like to enter data (y/n)", has_data);
    if ( has_data=="y" ) {
        std::string data_type;
        // TODO (Michelle): Support Any data
        GetUserInput("Enter data type (bytes/ string)", data_type);
        const static std::unordered_map<std::string,int> data_type_to_case{
            {"bytes",1},
            {"string",2},
            {"other", 3}
        };

        if ( data_type_to_case.find(data_type ) == data_type_to_case.end()) {
            OutputToInterface("Data type not recognized", -1);
            return -1;
        }
        
        switch(data_type_to_case.at(data_type)){
            case 1: GetUserInput("Enter data", *event -> mutable_binary_data()); break;
            case 2: GetUserInput("Enter data", *event -> mutable_text_data()); break;
            case 3: OutputToInterface("Other data not yet supported", -1); break;
            default: {
                OutputToInterface("Data type not handled", -1);
                return -1;
            }
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    int program_status = 0;
    
    // ensure that a write file is specified
    if ( argc != 2 ) {
        OutputToInterface("Incorrect Usage. Please specify a write file.", -1);
        return -1;
    }

    io::cloudevents::v1::CloudEvent event;

    program_status = ValidateFile(argv[1]);
    if ( program_status != 0 ) {
        return program_status;
    }
    
    // create an event
    // TODO (Michelle): handle optional and extension attrs
    program_status = CreateEvent(&event);
    if ( program_status != 0 ) {
        return program_status;
    }

    // write to file
    program_status = WriteToFile(argv[1], &event);
    if ( program_status != 0 ) {
        return program_status;
    }

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}