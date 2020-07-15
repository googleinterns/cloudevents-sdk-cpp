load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_proto//proto:defs.bzl", "proto_library")

# _____ build cpp from proto _____
cc_proto_library(
  name = "cloud_event_cc_proto",
  visibility = ["//visibility:public"],
  deps = [":cloud_event_proto"],
)


proto_library(
  name = "cloud_event_proto",
  srcs = ["cloud_event.proto"],
  deps = ["@com_google_protobuf//:any_proto",
  "@com_google_protobuf//:timestamp_proto"],
)

cc_proto_library(
  name = "pubsub_message_cc_proto",
  visibility = ["//visibility:public"],
  deps = [":pubsub_message_proto"],
)


proto_library(
  name = "pubsub_message_proto",
  srcs = ["pubsub_message.proto"],
  deps = ["@com_google_protobuf//:any_proto",
  "@com_google_protobuf//:timestamp_proto"],
)
