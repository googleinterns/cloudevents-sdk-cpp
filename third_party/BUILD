load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "statusor_lib",
    srcs = [
        "statusor.cc",
    ],
    hdrs = [
        "statusor.h",
        "statusor_internals.h",
    ],
    deps = ["@com_google_absl//absl/status"]
    #deps = ["//source/common/common:assert_lib"],
)

#envoy_cc_test(
#    name = "statusor_test",
#    srcs = ["statusor_test.cc"],
#    deps = [
#        ":statusor_lib",
#    ],
#)
