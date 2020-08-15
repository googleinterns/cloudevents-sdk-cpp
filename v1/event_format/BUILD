cc_library(
    name = "json_formatter_lib",
    srcs = ["json_formatter.cc"],
    hdrs = [
        "json_formatter.h",
        "formatter.h",
        "structured_cloud_event.h",
        "format.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//proto:cloud_event_cc_proto",
        "//third_party/statusor:statusor_lib",
        "@com_github_open_source_parsers_jsoncpp//:jsoncpp",
        "//v1/util:cloud_events_util_lib",
    ],
)

cc_test(
   name = "json_formatter_test",
   srcs = ["json_formatter_test.cc"],
   deps = [
       ":json_formatter_lib",
       "@gtest//:gtest",
       "@gtest//:gtest_main"
   ],
)
