load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "cloud_events_util_lib",
    srcs = ["cloud_events_util.cc"],
    hdrs = [
        "cloud_events_util.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@com_google_absl//absl/container:flat_hash_map",
        "//third_party/statusor:statusor_lib",
        "//third_party/base64:base64_lib",
        "//proto:cloud_event_cc_proto",
    ],
)

cc_test(
   name = "cloud_events_util_test",
   srcs = ["cloud_events_util_test.cc"],
   deps = [
       ":cloud_events_util_lib",
       "@gtest//:gtest",
       "@gtest//:gtest_main"
   ],
)
