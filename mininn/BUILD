load("@rules_cuda//cuda:defs.bzl", "cuda_library")

config_setting(
    name = "WITH_MULTI_THREADS",
    define_values = {"WITH_MULTI_THREADS": "ON"},
)

config_setting(
    name = "WITH_CUDA",
    define_values = {"WITH_CUDA": "ON"},
)

config_setting(
    name = "WITH_OPENCL",
    define_values = {"WITH_OPENCL": "ON"},
)

config_setting(
    name = "WITH_AVX",
    define_values = {"WITH_AVX": "ON"},
)

config_setting(
    name = "WITH_SSE",
    define_values = {"WITH_SSE": "ON"},
)

config_setting(
    name = "WITH_MKL",
    define_values = {"WITH_MKL": "ON"},
)

config_setting(
    name = "WITH_NEON",
    define_values = {"WITH_NEON": "ON"},
)

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

# TODO: make these optional
# backend lib
cuda_library(
    name = "cuda",
    # can not pass empty srcs to cuda_library
    srcs = glob([
        "backend/cuda/*.cu",
    ]),
    # copts = [
    #     "-allow-unsupported-compiler",
    # ],
)

cc_library(
    name = "opencl",
    srcs = select({
        ":WITH_OPENCL": glob([
            "backend/opencl/*.cc",
        ]),
        "//conditions:default": [],
    }),
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
    srcs = select({
        ":WITH_AVX": glob([
            "backend/avx/*.cc",
        ]),
        "//conditions:default": [],
    }),
    copts = select({
        "@platforms//os:windows": [
            "/arch:AVX512",
        ],
        "@platforms//os:linux": [
            "-mavx512vnni",
        ],
    }),
)

cc_library(
    name = "sse",
    srcs = select({
        ":WITH_SSE": glob([
            "backend/sse/*.cc",
        ]),
        "//conditions:default": [],
    }),
    copts = select({
        "@platforms//os:windows": [
            "/arch:SSE4.2",
        ],
        "@platforms//os:linux": [
            "-mavx512vnni",
        ],
    }),
)

cc_library(
    name = "mkl",
    srcs = select({
        ":WITH_MKL": glob([
            "backend/mkl/*.cc",
        ]),
        "//conditions:default": [],
    }),
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

cc_library(
    name = "neon",
    srcs = select({
        ":WITH_NEON": glob([
            "backend/neon/*.cc",
        ]),
        "//conditions:default": [],
    }),
)

cc_library(
    name = "backend",
    srcs = glob([
        "backend/*.cc",
    ]),
    deps = select({
            "WITH_CUDA": [":cuda"],
            "//conditions:default": [],
        }) + select({
            "WITH_OPENCL": [":opencl"],
            "//conditions:default": [],
        }) + select({
            "WITH_AVX": [":avx"],
            "//conditions:default": [],
        }) + select({
            "WITH_SSE": [":sse"],
            "//conditions:default": [],
        }) + select({
            "WITH_MKL": [":mkl"],
            "//conditions:default": [],
        }) + select({
            "WITH_NEON": [":neon"],
            "//conditions:default": [],
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