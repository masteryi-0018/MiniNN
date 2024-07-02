#include "mininn/graph/tensor.h"

#include <gtest/gtest.h>

TEST(tensorTest, createTensor) {
    Tensor* tensor = new Tensor();
    ASSERT_TRUE(tensor != nullptr);
}