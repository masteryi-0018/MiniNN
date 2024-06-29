cc_library(
    name = "utils",
    srcs = glob([
        "utils/*.cc",
    ]),
    hdrs = glob([
        "utils/*.h",
    ]),
)

cc_binary(
    name = "test-main",
    srcs = [
        "test.cc",
    ],
    includes = ["utils"],
    deps = [
        ":utils",
    ],
)