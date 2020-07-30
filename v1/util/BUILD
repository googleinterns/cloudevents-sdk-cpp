load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "formatter_util_lib",
    srcs = ["formatter_util.cc"],
    hdrs = [
        "//v1/event_format/format.h"
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//third_party/statusor:statusor_lib",
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
