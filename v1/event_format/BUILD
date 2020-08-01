load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "format_lib",
    hdrs = [
        "format.h",
    ],
    visibility = ["//visibility:public"],
    deps = [],
)

cc_library(
    name = "structured_cloud_event_lib",
    hdrs = [
        "structured_cloud_event.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":format_lib"
    ],
)

cc_library(
    name = "formatter_lib",
    hdrs = [
        "formatter.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":structured_cloud_event_lib"
    ],
)

cc_library(
    name = "json_formatter_lib",
    srcs = ["json_formatter.cc"],
    hdrs = [
        "json_formatter.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@com_github_open_source_parsers_jsoncpp//:jsoncpp",
        "//proto:cloud_event_cc_proto",
        "//third_party/statusor:statusor_lib",
        "//v1/util:cloud_events_util_lib",
        ":formatter_lib",
    ],
)

cc_test(
   name = "json_formatter_test",
   srcs = ["json_formatter_test.cc"],
   deps = [
       "@gtest//:gtest",
       "@gtest//:gtest_main",
       ":json_formatter_lib",
   ],
)
