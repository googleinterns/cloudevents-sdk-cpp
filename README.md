[![Build Status](https://travis-ci.org/googleinterns/cloudevents-sdk-cpp.svg?branch=master)](https://travis-ci.org/googleinterns/cloudevents-sdk-cpp)

# C++ SDK for [CloudEvents](https://github.com/cloudevents/spec)

This SDK is still a work in progress.

The Protobuf format is based on this [open pull request](https://github.com/JemDay/spec/tree/jd-proto) from JemDay.

**This is not an officially supported Google product.**

# Samples
Run-able code samples are available in the `/samples` folder.

### Setup
Install Bazel. Instuctions found in [Bazel documentation](https://docs.bazel.build/versions/master/install-ubuntu.html).

### Run
1. Create executable <br/>
`bazel build //sample:create_event`
2. Run executable <br/>
`bazel-bin/sample/create_event`

# File Structure
All logic related to implementing version 1 of the CloudEvent spec can be found in `//v1`.
- All logic for [Protocol Binding](https://github.com/cloudevents/spec/blob/master/spec.md#protocol-binding)s can be found in the subfolder `//v1/protocol_binding`.
- All logic for [Event Format](https://github.com/cloudevents/spec/blob/master/spec.md#event-format)s can be found in the subfolder `//v1/event_format`.
