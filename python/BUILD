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
    ] + select({
        "@platforms//os:windows": [":rename_pyd"],
        "@platforms//os:linux": [":rename_so"],
    }),
)

genrule(
    name = "rename_pyd",
    srcs = [":mininn_capi.pyd"],
    outs = ["mininn/mininn_capi.pyd"],
    cmd = "cp $< $@",
)

genrule(
    name = "rename_so",
    srcs = [":mininn_capi.so"],
    outs = ["mininn/mininn_capi.so"],
    cmd = "cp $< $@",
)

py_wheel(
    name = "mininn_wheel",
    distribution = "mininn",
    version = "1.0.0",
    deps  = [
        ":mininn_files",
    ],
    python_tag = "py3",
    strip_path_prefixes = [
        "python",
    ],
)