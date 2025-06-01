load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")
load("@rules_python//python:defs.bzl", "py_library")
load("@rules_python//python:packaging.bzl", "py_wheel")

pybind_extension(
    name = "mininn_capi",
    srcs = glob([
        "*.cc",
    ]),
    deps = [
        "//mininn:mininn_lib"
    ],
)

py_library(
    name = "mininn_py",
    srcs = glob([
        "mininn/*.py",
    ]),
)

# learn from: https://github.com/bazel-contrib/rules_python/issues/2918
filegroup(
    name = "mininn_files",
    srcs = [
        ":mininn_py",
        ":mininn_capi.pyd",
        ":mininn_capi.so",
    ],
)


py_wheel(
    name = "mininn_wheel",
    distribution = "mininn",
    version = "0.9.0",
    deps  = [
        ":mininn_files",
    ],
    python_tag = "py3",
)