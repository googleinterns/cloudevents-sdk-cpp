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

cc_library(
    name = "formatter_util_lib",
    srcs = ["formatter_util.cc"],
    hdrs = [
        "formatter_util.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//third_party/statusor:statusor_lib",
        "//proto:cloud_event_cc_proto",
        "//v1/event_format:json_formatter_lib",
    ],
)

cc_test(
   name = "formatter_util_test",
   srcs = ["formatter_util_test.cc"],
   deps = [
       ":formatter_util_lib",
       "@gtest//:gtest",
       "@gtest//:gtest_main"
   ],
)

cc_library(
    name = "binder_util_lib",
    srcs = ["binder_util.cc"],
    hdrs = [
        "binder_util.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//third_party/statusor:statusor_lib",
    ],
)

cc_test(
   name = "binder_util_test",
   srcs = ["binder_util_test.cc"],
   deps = [
       ":binder_util_lib",
       "@gtest//:gtest",
       "@gtest//:gtest_main"
   ],
)
