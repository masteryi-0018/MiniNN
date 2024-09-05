bazel clean --expunge

bazel build //mininn:utils
bazel build //mininn:graph
bazel build //mininn:operator
bazel build //mininn:kernel
bazel build //mininn:runtime
bazel build //mininn:parser

bazel build //mininn:test-main
bazel build //mininn:gtest-main