# C++ SDK for [CloudEvents](https://github.com/cloudevents/spec)

This SDK is still a work in progress. In particular, the protos it is built on is in Alpha status.

The CloudEvent proto is based on this [open pull request](https://github.com/JemDay/spec/tree/jd-proto) from JemDay.
<br/>

The PubsubMessage proto is based on this [pubsub proto file from Google API service platform](https://github.com/googleapis/googleapis/blob/master/google/pubsub/v1/pubsub.proto#L188).

**This is not an officially supported Google product.**

# Samples
Run-able code samples are available in the `/samples` folder.

### Setup
Install Bazel. Instuctions found in (Bazel documentation)(https://docs.bazel.build/versions/master/install-ubuntu.html).

### Run
1. Create executable <br/>
`bazel build //sample:create_event`
2. Run executable <br/>
`bazel-bin/sample/create_event`