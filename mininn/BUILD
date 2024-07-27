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

# todo: use bazel to build flatbuffers
# cc_library(
#     name = "parser",
#     srcs = glob([
#         "parser/*.cc",
#     ]),
#     hdrs = glob([
#         "parser/*.h",
#     ]),
#     includes = [
#         "//third_party/flatbuffers/include",
#     ],
#     deps = [
#         ":graph",
#         "//third_party/flatbuffers:flatbuffers",
#     ],
# )

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
    name = "gtest-main",
    srcs = glob(
        include = [
            "test/*.cc",
        ],
        exclude = [
            "test/parser_test.cc",
        ],
    ),
    deps = [
        ":runtime",
        ":operator",
        ":kernel",
        "//third_party/googletest:gtest",
    ],
)