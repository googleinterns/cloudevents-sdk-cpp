load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "structured_cloud_event_lib",
    srcs = ["structured_cloud_event.cc"],
    hdrs = ["structured_cloud_event.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//proto:cloud_event_cc_proto",
        "//third_party/base64:base64_lib",
        "//third_party/statusor:statusor_lib",
    ],
)

cc_test(
   name = "structured_cloud_event_test",
   srcs = ["structured_cloud_event_test.cc"],
   deps = [
       ":structured_cloud_event_lib",
       "@gtest//:gtest",
       "@gtest//:gtest_main"
   ],
)
