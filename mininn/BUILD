load("@rules_cuda//cuda:defs.bzl", "cuda_library")

config_setting(
    name = "with_multi_threads",
    define_values = {"WITH_MULTI_THREADS": "ON"},
)

config_setting(
    name = "with_cuda",
    define_values = {"WITH_CUDA": "ON"},
)

config_setting(
    name = "with_opencl",
    define_values = {"WITH_OPENCL": "ON"},
)

config_setting(
    name = "with_avx",
    define_values = {"WITH_AVX": "ON"},
)

config_setting(
    name = "with_sse",
    define_values = {"WITH_SSE": "ON"},
)

config_setting(
    name = "with_mkl",
    define_values = {"WITH_MKL": "ON"},
)

config_setting(
    name = "with_neon",
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
        "backend/neon/*.h",
    ]),
    copts = select({
        ":with_multi_threads": [ "-DWITH_MULTI_THREADS" ],
        "//conditions:default": [],
    }) + select({
        ":with_cuda": [ "-DWITH_CUDA" ],
        "//conditions:default": [],
    }) + select({
        ":with_opencl": [ "-DWITH_OPENCL" ],
        "//conditions:default": [],
    }) + select({
        ":with_avx": [ "-DWITH_AVX" ],
        "//conditions:default": [],
    }) + select({
        ":with_sse": [ "-DWITH_SSE" ],
        "//conditions:default": [],
    }) + select({
        ":with_mkl": [ "-DWITH_MKL" ],
        "//conditions:default": [],
    }) + select({
        ":with_neon": [ "-DWITH_NEON" ],
        "//conditions:default": [],
    })
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
# TODO: make cuda optional
cuda_library(
    name = "cuda",
    # can not pass empty srcs to cuda_library
    srcs = glob([
        "backend/cuda/*.cu",
    ]),
)

cc_library(
    name = "opencl",
    srcs = select({
        ":with_opencl": glob([
            "backend/opencl/*.cc",
        ]),
        "//conditions:default": [],
    }),
    copts = select({
        "@platforms//os:windows": ["/I\"C:/PROGRA~1/NVIDIA~2/CUDA/v12.9/include\""],
        # linux will get:
        # The include path 'xxx' references a path outside of the execution root.
        "@platforms//os:linux": [],
    }),
    linkopts = select({
        # windows search lib derictly, linux need "-l"
        "@platforms//os:windows": ["C:/PROGRA~1/NVIDIA~2/CUDA/v12.9/lib/x64/OpenCL.lib"],
        "@platforms//os:linux": ["-l/usr/local/cuda/lib64/libOpenCL.so"],
    }),
)

cc_library(
    name = "avx",
    srcs = select({
        ":with_avx": glob([
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
        ":with_sse": glob([
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
        ":with_mkl": glob([
            "backend/mkl/*.cc",
        ]),
        "//conditions:default": [],
    }),
    copts = select({
        "@platforms//os:windows": ["/I\"E:/oneAPI/mkl/2024.2/include\""],
        "@platforms//os:linux": [],
    }),
    linkopts = select({
        "@platforms//os:windows": [
            "E:/oneAPI/mkl/2024.2/lib/mkl_intel_lp64.lib",
            "E:/oneAPI/mkl/2024.2/lib/mkl_core.lib",
            "E:/oneAPI/mkl/2024.2/lib/mkl_sequential.lib"
        ],
        "@platforms//os:linux": [
            "-l/opt/intel/oneapi/mkl/latest/lib/libmkl_intel_lp64.so",
            "-l/opt/intel/oneapi/mkl/latest/lib/libmkl_core.so",
            "-l/opt/intel/oneapi/mkl/latest/lib/libmkl_sequential.so",
        ],
    }),
)

cc_library(
    name = "neon",
    srcs = select({
        ":with_neon": glob([
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
            "with_cuda": [":cuda"],
            "//conditions:default": [],
        }) + select({
            "with_opencl": [":opencl"],
            "//conditions:default": [],
        }) + select({
            "with_avx": [":avx"],
            "//conditions:default": [],
        }) + select({
            "with_sse": [":sse"],
            "//conditions:default": [],
        }) + select({
            "with_mkl": [":mkl"],
            "//conditions:default": [],
        }) + select({
            "with_neon": [":neon"],
            "//conditions:default": [],
        }),
)

cc_library(
    name = "mininn_lib",
    # order matters for linking
    deps = [
        ":runtime",
        ":parser",
        ":graph",
        ":operator",
        ":kernel",
        ":utils",
        ":backend"
    ],
    linkopts = select({
        "@platforms//os:windows": [],
        "@platforms//os:linux": ["-lpthread"],
    }),
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