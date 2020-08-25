load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "binder_lib",
    hdrs = [
        "binder.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//proto:cloud_event_cc_proto",
        "//third_party/statusor:statusor_lib",
        "//v1/util:binder_util_lib",
        "//v1/util:cloud_events_util_lib",
        "//v1/util:formatter_util_lib",

    ],
)
