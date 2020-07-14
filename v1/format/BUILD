load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "structured_cloud_event_lib",
    srcs = ["structured_cloud_event.cc"],
    hdrs = ["structured_cloud_event.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//proto:cloud_event_cc_proto",
        "//third_party/statusor:statusor_lib",
        "@com_github_open_source_parsers_jsoncpp//:jsoncpp",
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

cc_library(
    name = "json_marshaller_lib",
    srcs = ["json_marshaller.cc"],
    hdrs = [
        "json_marshaller.h",
        "marshaller.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//v1/format:structured_cloud_event_lib",
        "//proto:cloud_event_cc_proto",
        "//third_party/statusor:statusor_lib",
        "//third_party/base64:base64_lib",
        "@com_github_open_source_parsers_jsoncpp//:jsoncpp",
    ],
)

cc_test(
   name = "json_marshaller_test",
   srcs = ["json_marshaller_test.cc"],
   deps = [
       ":json_marshaller_lib",
       "@gtest//:gtest",
       "@gtest//:gtest_main"
   ],
)
