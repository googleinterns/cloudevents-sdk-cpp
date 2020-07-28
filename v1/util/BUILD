load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "cloud_events_util_lib",
    srcs = ["cloud_events_util.cc"],
    hdrs = [
        "cloud_events_util.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//third_party/statusor:statusor_lib",
        "//third_party/base64:base64_lib",
        "//proto:cloud_event_cc_proto",
    ],
)
