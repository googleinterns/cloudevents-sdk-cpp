load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "format_lib",
    srcs = ["format.cc"],
    hdrs = ["format.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//proto:cloud_event_cc_proto",
        "//third_party/statusor:statusor_lib",
        "@com_github_open_source_parsers_jsoncpp//:jsoncpp",
    ],
)

cc_test(
   name = "format_test",
   srcs = ["format_test.cc"],
   deps = [
       ":format_lib",
       "@gtest//:gtest",
       "@gtest//:gtest_main"
   ],
)
