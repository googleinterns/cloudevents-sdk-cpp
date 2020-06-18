load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_proto//proto:defs.bzl", "proto_library")

# _____ build cpp from proto _____
cc_proto_library(
  name = "spec_cc_proto",
  visibility = ["//visibility:public"],
  deps = [":spec_proto"],
)


proto_library(
  name = "spec_proto",
  srcs = ["spec.proto"],
  deps = ["@com_google_protobuf//:any_proto",
  "@com_google_protobuf//:timestamp_proto"],
)
