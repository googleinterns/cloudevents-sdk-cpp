load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "binder_lib",
    srcs = ["binder.cc"],
    hdrs = ["binder.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//v1/format:structured_cloud_event_lib",
        "//v1/format:json_marshaller_lib",
        "//proto:cloud_event_cc_proto",
        "//proto:pubsub_message_cc_proto",
        "//third_party/statusor:statusor_lib",
        "//third_party/base64:base64_lib",
        "@com_github_open_source_parsers_jsoncpp//:jsoncpp",
    ],
)

cc_test(
   name = "binder_test",
   srcs = ["binder_test.cc"],
   deps = [
       ":binder_lib",
       "@gtest//:gtest",
       "@gtest//:gtest_main"
   ],
)

cc_library(
    name = "pubsub_binder_lib",
    srcs = ["pubsub_binder.cc"],
    hdrs = ["pubsub_binder.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//binder_lib",
        "//v1/format:structured_cloud_event_lib",
        "//v1/format:json_marshaller_lib",
        "//proto:cloud_event_cc_proto",
        "//proto:pubsub_message_cc_proto",
        "//third_party/statusor:statusor_lib",
        "//third_party/base64:base64_lib",
        "@com_github_open_source_parsers_jsoncpp//:jsoncpp",
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
