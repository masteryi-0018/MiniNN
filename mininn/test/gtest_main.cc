#include "mininn/graph/use_op.h"
#include "mininn/graph/use_kernel.h"

#include <gtest/gtest.h>
// #include <glog/logging.h>

int main(int argc, char** argv) {
    // FLAGS_logtostderr  = 1;
    // google::InitGoogleLogging(argv[0]);
    // LOG(INFO) << "Found " << 1 << " cookies";
    // LOG(WARNING) << "Found " << 2 << " cookies";
    // LOG(ERROR) << "Found " << 3 << " cookies";
    // LOG(FATAL) << "Found " << 4 << " cookies";
    // return 0;

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}