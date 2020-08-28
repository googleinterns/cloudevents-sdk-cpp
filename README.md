[![Build Status](https://travis-ci.org/googleinterns/cloudevents-sdk-cpp.svg?branch=master)](https://travis-ci.org/googleinterns/cloudevents-sdk-cpp)

# C++ SDK for [CloudEvents](https://github.com/cloudevents/spec)

This SDK is still a work in progress.

The CloudEvent class (generated from protobuf) is based on this [open PR to the CloudEvent spec](https://github.com/JemDay/spec/tree/jd-proto) from JemDay.
<br/>

The PubsubMessage class (generated from protobuf) is based on [Google API](https://github.com/googleapis/googleapis/blob/master/google/pubsub/v1/pubsub.proto#L188).

**This is not an officially supported Google product.**

# User Guide
More example use cases are available in header files as comments.
The only files that you will directly import and use are in `//v1/protocol_binding/`.

## Setup
### Install Bazel Build System
Instuctions found in [Bazel documentation](https://docs.bazel.build/versions/master/install-ubuntu.html).

### Import SDK as a Bazel package in WORKSPACE
In `//WORKSPACE`
```
# TODO (#63) : Update README to create Bazel target to master repo once merged

git_repository(
    name = "cloudevents_cpp_sdk",
    commit = "20c9855c11bc66efcc6e086571477bb69870be1d",
    remote = "https://github.com/michlee1337/cloudevents-sdk-cpp/tree/binder-all.v2",
)

```

### Set up the build dependencies
Specify any binders needed as a bazel target dependency.

To use PubsubBinder:
```
cc_binary(
    ...
    deps = [
        ...
        "@cloudevents_cpp_sdk/protocol_binding:pubsub_binder_lib",
    ],
)
```

To use HttpReqBinder or HttpResBinder:
```
cc_binary(
    ...
    deps = [
        ...
        "@cloudevents_cpp_sdk/protocol_binding:http_binder_lib",
    ],
)
```

## Binding a CloudEvent to a Protocol-Message in Binary-ContentMode
Import the header file and use XBinder
```
// import the header for the binder
#include "//v1/protocol_binding/pubsub_binder.h"

using ::google::pubsub::v1::PubsubMessage;
using ::cloudevents::binder::PubsubBinder;
using ::cloudevents_absl::StatusOr;

// create a CloudEvent
CloudEvent my_cloud_event;
my_cloud_event.set_id("my_id");
my_cloud_event.set_source("my_source");
my_cloud_event.set_spec_version("1.0");
my_cloud_event.set_type("my_type");
my_cloud_event.set_binary_data("1010");

// Initialize the binder
PubsubBinder pubsub_binder;

// Create the Message
StatusOr<PubsubMessage> bind = pubsub_binder.Bind(my_cloud_event);

// Check no errors
if (!bind.ok()) {
  std::cerr << bind.status();
}

// unwrap message
PubsubMessage usable_message = *bind;
```

## Binding a CloudEvent to a Protocol-Message in Structured-ContentMode
Import the header file and use Format and XBinder
```
// import the header for the binder
#include "//v1/protocol_binding/pubsub_binder.h"

using ::google::pubsub::v1::PubsubMessage;
using ::cloudevents::binder::PubsubBinder
using ::cloudevents::format::Format
using ::cloudevents_absl::StatusOr;

// create a CloudEvent
CloudEvent my_cloud_event;
my_cloud_event.set_id("my_id");
my_cloud_event.set_source("my_source");
my_cloud_event.set_spec_version("1.0");
my_cloud_event.set_type("my_type");
my_cloud_event.set_binary_data("1010");

// initialize the binder
PubsubBinder pubsub_binder;

// specify the EventFormat to be used and create the Message
StatusOr<PubsubMessage> bind = pubsub_binder.Bind(my_cloud_event, Format::kJson);

// check no errors
if (!bind.ok()) {
  std::cerr << bind.status();
}

// unwrap message
PubsubMessage usable_message = *bind;
```

## Unbind a CloudEvent from a Protocol-Message
```
// import the header for the binder of the protocol message you're working with
#include "//v1/protocol_binding/pubsub_binder.h"

using ::google::pubsub::v1::PubsubMessage;
using ::cloudevents::binder::Binder
using ::cloudevents_absl::StatusOr;

PubsubMessage my_pusub_msg = ...; // However you get this message

// initialize the binder
PubsubBinder pubsub_binder;

// create the CloudEvent
StatusOr<CloudEvent> unbind = pubsub_binder.Unbind(my_pusub_msg);

// check no errors
if (!unbind.ok()) {
  std::cerr << unbind.status();
}

// unwrap cloudevent
CloudEvent cloud_event = *unbind;

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
