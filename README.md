[![Build Status](https://travis-ci.org/googleinterns/cloudevents-sdk-cpp.svg?branch=master)](https://travis-ci.org/googleinterns/cloudevents-sdk-cpp)

# C++ SDK for [CloudEvents](https://github.com/cloudevents/spec)

This SDK is still a work in progress.

The CloudEvent class (generated from protobuf) is based on this [open PR to the CloudEvent spec](https://github.com/JemDay/spec/tree/jd-proto) from JemDay.
<br/>

The PubsubMessage class (generated from protobuf) is based on [Google API](https://github.com/googleapis/googleapis/blob/master/google/pubsub/v1/pubsub.proto#L188).

**This is not an officially supported Google product.**

# User Guide
## Setup
Install Bazel. Instuctions found in [Bazel documentation](https://docs.bazel.build/versions/master/install-ubuntu.html).

## Binding a CloudEvent to a Protocol
### BinaryContentMode
_Coming soon_

### StructuredContentMode
_Coming soon_

## Event-formats
To specify an event-format you want to use:
1. Import the header file `//v1/event_format/format`.
2. Pass in the format parameter as `cloudevents::format::_A_FORMAT`. For a specific sample check the documentation on StructuredContentMode.

# Samples
Run-able code samples are available in the `/samples` folder.

### Run
1. Create executable <br/>
`bazel build //sample:create_event`
2. Run executable <br/>
`bazel-bin/sample/create_event`

# File Structure
All logic related to implementing version 1 of the CloudEvent spec can be found in `//v1`.
- All logic for [Protocol Binding](https://github.com/cloudevents/spec/blob/master/spec.md#protocol-binding)s can be found in the subfolder `//v1/protocol_binding`.
- All logic for [Event Format](https://github.com/cloudevents/spec/blob/master/spec.md#event-format)s can be found in the subfolder `//v1/event_format`.

# Dependencies
## Json
Using [Nuxi NL's bazel setup] (https://github.com/NuxiNL/bazel-third-party) to setup a bazel target for jsoncpp

## base64
Using René Nyffenegger's [base64 encoding/ decoding lib](https://github.com/ReneNyffenegger/cpp-base64).

## Abseil StatusOr
Copying [Envoy's Abseil StatusOr setup](https://github.com/envoyproxy/envoy/tree/44eedc792ab64bba2358e0294b53294c6bc30526/third_party/statusor) temporarily until an official abseil release exists.

