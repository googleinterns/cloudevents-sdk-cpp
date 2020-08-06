load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "binder_lib",
    hdrs = [
        "binder.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//third_party/base64:base64_lib",
        "//third_party/statusor:statusor_lib",
        "//proto:cloud_event_cc_proto",
        "//proto:pubsub_cc_proto",
        "//v1/event_format:json_formatter_lib",
        "//v1/util:formatter_util_lib",
    ],
)
