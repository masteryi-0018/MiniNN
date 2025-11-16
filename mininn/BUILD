load("@rules_cuda//cuda:defs.bzl", "cuda_library")

cc_library(
    name = "runtime",
    srcs = glob([
        "runtime/*.cc",
    ]),
    hdrs = glob([
        "runtime/*.h",
        "graph/*.h",
    ]),
)

cc_library(
    name = "parser",
    srcs = glob([
        "parser/*.cc",
    ]),
    hdrs = glob([
        "parser/*.h",
        "graph/*.h",
        "utils/*.h",
    ]),
    # includes = [
    #     "../third_party/flatbuffers/include"
    # ],
    # linux need this
    deps = [
        "@flatbuffers//:flatbuffers",
    ],
)

cc_library(
    name = "graph",
    srcs = glob([
        "graph/*.cc",
    ]),
    hdrs = glob([
        "graph/*.h",
        "utils/*.h",
    ]),
)

cc_library(
    name = "operator",
    srcs = glob([
        "operator/*.cc",
    ]),
    hdrs = glob([
        "operator/*.h",
        "graph/*.h",
        "utils/*.h",
    ]),
)

cc_library(
    name = "kernel",
    srcs = glob([
        "kernel/*.cc",
    ]),
    hdrs = glob([
        "kernel/*.h",
        "graph/*.h",
        "utils/*.h",
        "backend/cuda/*.cuh",
        "backend/opencl/*.h",
        "backend/avx/*.h",
        "backend/sse/*.h",
        "backend/mkl/*.h",
    ]),
    defines = select({
        ":with_wheel": [],
        "@platforms//os:windows": [
            "WITH_MULTI_THREADS",
            "WITH_CUDA",
            "WITH_OPENCL",
            "WITH_AVX",
            "WITH_SSE",
            "WITH_MKL",
        ],
        "@platforms//os:linux": [
            "WITH_MULTI_THREADS",
            "WITH_CUDA",
            "WITH_OPENCL",
            "WITH_AVX",
            "WITH_SSE",
        ],
    }),
)

cc_library(
    name = "utils",
    srcs = glob([
        "utils/*.cc",
    ]),
    hdrs = glob([
        "utils/*.h",
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
    #     "C:/PROGRA~1/NVIDIA~2/CUDA/v12.9/include"
    # ],
    copts = select({
        "@platforms//os:windows": ["/I\"C:/PROGRA~1/NVIDIA~2/CUDA/v12.9/include\""],
        "@platforms//os:linux": ["/usr/local/cuda/include/"],
    }),
    linkopts = select({
        "@platforms//os:windows": ["C:/PROGRA~1/NVIDIA~2/CUDA/v12.9/lib/x64/OpenCL.lib"],
        "@platforms//os:linux": ["/usr/local/cuda/lib64/libOpenCL.so"],
    }),
)

cc_library(
    name = "avx",
    srcs = glob([
        "backend/avx/*.cc",
    ]),
    copts = [
        "-mavx512vnni",
    ],
)

cc_library(
    name = "sse",
    srcs = glob([
        "backend/sse/*.cc",
    ]),
    copts = [
        "-msse4.2",
    ],
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
        "E:/oneAPI/mkl/2024.2/lib/mkl_intel_lp64.lib",
        "E:/oneAPI/mkl/2024.2/lib/mkl_core.lib",
        "E:/oneAPI/mkl/2024.2/lib/mkl_sequential.lib"
    ],
    # linux will get include path '/opt/intel/oneapi/mkl/latest/include'
    # references a path outside of the execution root.
    # flatbuffers use third_party to fix, but mkl not
    target_compatible_with = ["@platforms//os:windows"],
)

config_setting(
    name = "with_wheel",
    values = {"define": "with_wheel=1"},
)

cc_library(
    name = "backend",
    deps = select({
        ":with_wheel": [],
        "@platforms//os:windows": [
            ":cuda",
            ":opencl",
            ":avx",
            ":sse",
            ":mkl",
        ],
        "@platforms//os:linux": [
            ":cuda",
            ":opencl",
            ":avx",
            ":sse",
        ],
    }),
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
    visibility = ["//visibility:public"],
)

# test lib
cc_binary(
    name = "gtest-main",
    srcs = glob([
        "test/*.cc",
        "test/graph/*.cc",
        "test/model/*.cc",
        "test/operator/*.cc",
        "test/parser/*.cc",
        "test/runtime/*.cc",
    ]),
    deps = [
        ":mininn_lib",
        "@googletest//:gtest",
    ],
)