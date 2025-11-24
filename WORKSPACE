# external flatbuffers lib
local_repository(
    name = "flatbuffers",
    path = "third_party/flatbuffers",
)

# external googletest lib
local_repository(
    name = "googletest",
    path = "third_party/googletest",
)

# external rules_cuda lib
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

# external pybind11_bazel lib
local_repository(
    name = "pybind11_bazel",
    path = "third_party/pybind11_bazel",
)

# external pybind11 lib
local_repository(
    name = "pybind11",
    path = "third_party/pybind11",
)

# We still require the pybind library from http_archive because pybind11 repo do not have a WORKSPACE file.
# load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
# http_archive(
#   name = "pybind11",
#   build_file = "@pybind11_bazel//:pybind11-BUILD.bazel",
#   strip_prefix = "pybind11-2.13.6",
#   urls = ["https://github.com/pybind/pybind11/archive/v2.13.6.zip"],
# )

# external rules_python lib
# https://rules-python.readthedocs.io/en/latest/getting-started.html#using-a-workspace-file
local_repository(
    name = "rules_python",
    path = "third_party/rules_python",
)

load("@rules_python//python:repositories.bzl", "py_repositories")
py_repositories()
load("@rules_python//python:repositories.bzl", "python_register_toolchains")
python_register_toolchains(
    name = "python_3_13",
    # Available versions are listed in @rules_python//python:versions.bzl.
    # We recommend using the same version your team is already standardized on.
    python_version = "3.13",
)

# load("@rules_python//python:pip.bzl", "pip_parse")
# pip_parse(
#     name = "pypi",
#     python_interpreter_target = "@python_3_13_host//:python",
#     requirements_lock = "//:requirements.txt",
# )
