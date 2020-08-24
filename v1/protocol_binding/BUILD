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

cc_library(
    name = "pubsub_binder_lib",
    srcs = [
        "pubsub_binder.cc"
    ],
    hdrs = [
        "pubsub_binder.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//third_party/statusor:statusor_lib",
        "//third_party/base64:base64_lib",
        "//proto:cloud_event_cc_proto",
        "//proto:pubsub_cc_proto",
        "//v1/util:binder_util_lib",
        "//v1/util:cloud_events_util_lib",
        ":binder_lib",
    ],
)

cc_test(
   name = "pubsub_binder_test",
   srcs = ["pubsub_binder_test.cc"],
   deps = [
       ":pubsub_binder_lib",
       "@gtest//:gtest",
       "@gtest//:gtest_main"
   ],
)
