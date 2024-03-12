#include <gtest/gtest.h>

#include "tensor.h"

TEST(Tensor, createTensor) {
    std::vector<int> shape1 = {1, 3, 224, 224};
    Tensor* tensor1 = new Tensor(shape1);
    ASSERT_TRUE(tensor1 != nullptr);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}