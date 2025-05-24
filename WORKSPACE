# external flatbuffers lib
local_repository(
    name = "flatbuffers",
    path = "third_party/flatbuffers",
)

# external googletest libs
local_repository(
    name = "googletest",
    path = "third_party/googletest",
)

# external rules_cuda libs
local_repository(
    name = "rules_cuda",
    path = "third_party/rules_cuda",
)

load("@rules_cuda//cuda:repositories.bzl", "rules_cuda_dependencies", "rules_cuda_toolchains")
rules_cuda_dependencies()
rules_cuda_toolchains(register_toolchains = True)

# Uncomment the following lines to use a specific version of rules_cuda
# load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
# http_archive(
#     name = "rules_cuda",
#     sha256 = "{sha256_to_replace}",
#     strip_prefix = "rules_cuda-{git_commit_hash}",
#     urls = ["https://github.com/bazel-contrib/rules_cuda/archive/{git_commit_hash}.tar.gz"],
# )