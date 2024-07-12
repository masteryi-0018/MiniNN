cc_library(
    name = "utils",
    srcs = glob([
        "utils/*.cc",
    ]),
    hdrs = glob([
        "utils/*.h",
    ]),
)

cc_library(
    name = "graph",
    srcs = glob([
        "graph/*.cc",
    ]),
    hdrs = glob([
        "graph/*.h",
    ]),
    deps = [
        ":utils",
    ],
)

cc_library(
    name = "operator",
    srcs = glob([
        "operator/*.cc",
    ]),
    hdrs = glob([
        "operator/*.h",
    ]),
    deps = [
        ":graph",
        ":utils",
    ],
)

cc_library(
    name = "kernel",
    srcs = glob([
        "kernel/*.cc",
    ]),
    hdrs = glob([
        "kernel/*.h",
    ]),
    deps = [
        ":graph",
        ":utils",
    ],
)

cc_library(
    name = "runtime",
    srcs = glob([
        "runtime/*.cc",
    ]),
    hdrs = glob([
        "runtime/*.h",
    ]),
    deps = [
        ":graph",
    ],
)

cc_binary(
    name = "test-main",
    srcs = [
        "test.cc",
    ],
    deps = [
        ":utils",
    ],
)

cc_binary(
    name = "gtest-ut",
    srcs = glob(
        include = [
            "test/*.cc",
        ],
        exclude = [
            "test/predictor_test.cc",
        ],
    ),
    deps = [
        ":runtime",
        ":operator",
        ":kernel",
        "//third_party/googletest:gtest",
    ],
)

cc_binary(
    name = "gtest-predictor",
    srcs = [
        "test/predictor_test.cc",
        "test/gtest_main.cc",
    ],
    deps = [
        ":runtime",
        ":operator",
        ":kernel",
        "//third_party/googletest:gtest",
    ],
)