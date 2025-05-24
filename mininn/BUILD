load("@rules_cuda//cuda:defs.bzl", "cuda_library")

cc_library(
    name = "runtime",
    srcs = glob([
        "runtime/*.cc",
    ]),
)

cc_library(
    name = "parser",
    srcs = glob([
        "parser/*.cc",
    ]),
    deps = [
        "@flatbuffers//:flatbuffers",
    ],
)

cc_library(
    name = "graph",
    srcs = glob([
        "graph/*.cc",
    ]),
)

cc_library(
    name = "operator",
    srcs = glob([
        "operator/*.cc",
    ]),
)

cc_library(
    name = "kernel",
    srcs = glob([
        "kernel/*.cc",
    ]),
    defines = [
        "WITH_MULTI_THREADS",
        "WITH_CUDA",
        "WITH_OPENCL",
        "WITH_AVX",
        "WITH_SSE",
        "WITH_MKL",
    ],
)

cc_library(
    name = "utils",
    srcs = glob([
        "utils/*.cc",
    ]),
)

# backend lib
cuda_library(
    name = "cuda",
    srcs = glob([
        "backend/cuda/*.cu",
    ]),
    # copts = [
    #     "-allow-unsupported-compiler",
    # ],
)

cc_library(
    name = "opencl",
    srcs = glob([
        "backend/opencl/*.cc",
    ]),
    # includes = [
    #     "C:/PROGRA~1/NVIDIA~2/CUDA/v12.2/include"
    # ],
    copts = [
        "/I\"C:/PROGRA~1/NVIDIA~2/CUDA/v12.2/include\"",
    ],
    linkopts = [
        "C:/PROGRA~1/NVIDIA~2/CUDA/v12.2/lib/x64/OpenCL.lib"
    ],
)

cc_library(
    name = "avx",
    srcs = glob([
        "backend/avx/*.cc",
    ]),
)

cc_library(
    name = "sse",
    srcs = glob([
        "backend/sse/*.cc",
    ]),
)

cc_library(
    name = "mkl",
    srcs = glob([
        "backend/mkl/*.cc",
    ]),
    includes = [
        "E:/oneAPI/mkl/2024.2/include"
    ],
    linkopts = [
        "E:/oneAPI/mkl/2024.2/lib/mkl_rt.lib"
    ]
)

cc_library(
    name = "backend",
    deps = [
        ":cuda",
        ":opencl",
        ":avx",
        ":sse",
        ":mkl",
    ],
)

cc_library(
    name = "mininn_lib",
    deps = [
        ":runtime",
        ":parser",
        ":graph",
        ":operator",
        ":kernel",
        ":utils",
        ":backend"
    ],
)

# test lib
cc_binary(
    name = "gtest-main",
    srcs = glob([
        "test/*.cc",
        "test/graph/*.cc",
        "test/operator/*.cc",
        "test/parser/*.cc",
        "test/runtime/*.cc",
    ]),
    deps = [
        ":mininn_lib",
        "@googletest//:gtest",
    ],
)