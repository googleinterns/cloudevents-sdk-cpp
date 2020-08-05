workspace(name = "cloudevent")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# _____ BUILD RULES _____

http_archive(
    name = "rules_cc",
    sha256 = "35f2fb4ea0b3e61ad64a369de284e4fbbdcdba71836a5555abb5e194cf119509",
    strip_prefix = "rules_cc-624b5d59dfb45672d4239422fa1e3de1822ee110",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_cc/archive/624b5d59dfb45672d4239422fa1e3de1822ee110.tar.gz",
        "https://github.com/bazelbuild/rules_cc/archive/624b5d59dfb45672d4239422fa1e3de1822ee110.tar.gz",
    ],
)
load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies")
rules_cc_dependencies()

http_archive(
    name = "rules_proto",
    sha256 = "602e7161d9195e50246177e7c55b2f39950a9cf7366f74ed5f22fd45750cd208",
    strip_prefix = "rules_proto-97d8af4dc474595af3900dd85cb3a29ad28cc313",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_proto/archive/97d8af4dc474595af3900dd85cb3a29ad28cc313.tar.gz",
        "https://github.com/bazelbuild/rules_proto/archive/97d8af4dc474595af3900dd85cb3a29ad28cc313.tar.gz",
    ],
)

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()

# _____ ABSEIL _____

http_archive(
    name = "com_google_absl",
    # Commit from 2020-03-03
    url = "https://github.com/abseil/abseil-cpp/archive/b19ba96766db08b1f32605cb4424a0e7ea0c7584.tar.gz",
    sha256 = "c7ff8decfbda0add222d44bdc27b47527ca4e76929291311474efe7354f663d3",
    strip_prefix = "abseil-cpp-b19ba96766db08b1f32605cb4424a0e7ea0c7584",
)

# _____ GTEST _____

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
git_repository(
    name = "gtest",
    remote = "https://github.com/google/googletest",
    commit = "703bd9caab50b139428cea1aaff9974ebee5742e",
    shallow_since = "1570114335 -0400",
)

# _____ GAPIS _____
# Used to get PubSub message proto

http_archive(
    name = "googleapis",
    urls = ["https://github.com/googleapis/googleapis/archive/master.zip"],
    # commented out SHA as it is flaky
    #sha256 = "58d0c3e2bf38d25c4a0be7939166ca263bfc4fe58d3cfc83d05576cb04b97cc7",
    strip_prefix = "googleapis-master",
)

load("@googleapis//:repository_rules.bzl", "switched_rules_by_language")

switched_rules_by_language(
    name = "com_google_googleapis_imports",
    cc = True,
)

http_archive(
    name = "rules_proto_grpc",
    urls = ["https://github.com/rules-proto-grpc/rules_proto_grpc/archive/1.0.2.tar.gz"],
    sha256 = "5f0f2fc0199810c65a2de148a52ba0aff14d631d4e8202f41aff6a9d590a471b",
    strip_prefix = "rules_proto_grpc-1.0.2",
)

git_repository(
    name = "org_cloudabi_bazel_third_party",
    commit = "91ca2167219c612a89334fa09ddf15fbdc5d0592",
    remote = "https://github.com/NuxiNL/bazel-third-party.git",
    shallow_since = "1547652202 +0100",
)
load("@org_cloudabi_bazel_third_party//:third_party.bzl", "third_party_repositories")
third_party_repositories()
