cc_library(
    name = "format_lib",
    hdrs = [
        "format.h",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "formatter_lib",
    hdrs = [
        "formatter.h",
        "structured_cloud_event.h",
    ],
    deps = [
        "//proto:cloud_event_cc_proto",
        "//third_party/statusor:statusor_lib",
        ":format_lib",
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
