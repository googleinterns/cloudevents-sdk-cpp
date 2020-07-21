[![Build Status](https://travis-ci.org/googleinterns/cloudevents-sdk-cpp.svg?branch=master)](https://travis-ci.org/googleinterns/cloudevents-sdk-cpp)

# C++ SDK for [CloudEvents](https://github.com/cloudevents/spec)

This SDK is still a work in progress.

The CloudEvent class (generated from protobuf) is based on this [open PR to the CloudEvent spec](https://github.com/JemDay/spec/tree/jd-proto) from JemDay.
<br/>

The PubsubMessage class (generated from protobuf) is based on [Google API](https://github.com/googleapis/googleapis/blob/master/google/pubsub/v1/pubsub.proto#L188).


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
