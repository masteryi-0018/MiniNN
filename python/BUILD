load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")
# load("@rules_python//python:defs.bzl", "py_library")
# load("@rules_python//python:packaging.bzl", "py_wheel", "py_package")

pybind_extension(
    name = "mininn_capi",
    srcs = glob([
        "*.cc",
    ]),
    deps = [
        "//mininn:mininn_lib"
    ],
)

# todo: use bazel to generate python wheel, now copy mininn_capi manually
# py_library(
#     name = "mininn_py",
#     srcs = glob([
#         "mininn/*.py",
#     ]),
#     data = [
#         ":mininn_capi",
#     ]
# )

# py_package(
#     name = "mininn_pkg",
#     deps = [
#         ":mininn_py",
#         ":mininn_capi",
#     ],
#     package_name = "mininn_py_pkg",
# )

# py_wheel(
#     name = "mininn_wheel",
#     distribution = "mininn",
#     version = "1.0.0",
#     deps  = [
#         ":mininn_py",
#     ],
#     python_tag = "py3",
# )