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

## Binding a CloudEvent to a Protocol-Message
### BinaryContentMode
```
// import the header for the binder of the protocol message you're working with
#include "//v1/protocol_binding/pubsub_binder.h"
 
using ::google::pubsub::v1::PubsubMessage;
using ::cloudevents::binder::Binder

// create a CloudEvent
CloudEvent my_cloud_event;
my_cloud_event.set_id("my_id");
my_cloud_event.set_source("my_source");
my_cloud_event.set_spec_version("1.0");
my_cloud_event.set_type("my_type");
my_cloud_event.set_binary_data("1010");

// Initialize the binder
Binder<PubsubMessage> pubsub_binder;

// Create the Message
PubsubMessage usable_message = pubsub_binder.bind(my_cloud_event);
```

### StructuredContentMode
```
// import the header for formats
#include "//v1/event_format/format.h"

// import the header for the binder of the protocol message you're working with
#include "//v1/protocol_binding/pubsub_binder.h"

using ::google::pubsub::v1::PubsubMessage;
using ::cloudevents::binder::Binder
using ::cloudevents::format::Format

// create a CloudEvent
CloudEvent my_cloud_event;
my_cloud_event.set_id("my_id");
my_cloud_event.set_source("my_source");
my_cloud_event.set_spec_version("1.0");
my_cloud_event.set_type("my_type");
my_cloud_event.set_binary_data("1010");

// Initialize the binder
Binder<PubsubMessage> pubsub_binder;

// Specify the EventFormat to be used and create the Message
PubsubMessage usable_message = pubsub_binder.Bind(my_cloud_event, Format::kJson);
```

## Unbind a CloudEvent from a Protocol-Message
```
// import the header for the binder of the protocol message you're working with
#include "//v1/protocol_binding/pubsub_binder.h"

using ::google::pubsub::v1::PubsubMessage;
using ::cloudevents::binder::Binder
using ::cloudevents::format::Format

PubsubMessage my_pusub_msg; // how you get this message is out of scope for this SDK

// Initialize the binder
Binder<PubsubMessage> pubsub_binder;

// Create the CloudEvent
CloudEvent my_cloud_event = pubsub_binder.Unbind(my_pusub_msg);
```

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
Jouni Malinen's [base64 encoding/ decoding lib](http://web.mit.edu/freebsd/head/contrib/wpa/src/utils/base64.c) and René Nyffenegger's [base64 testing code](https://github.com/ReneNyffenegger/cpp-base64).

## Abseil StatusOr
Copying [Envoy's Abseil StatusOr setup](https://github.com/envoyproxy/envoy/tree/44eedc792ab64bba2358e0294b53294c6bc30526/third_party/statusor) temporarily until an official abseil release exists.

