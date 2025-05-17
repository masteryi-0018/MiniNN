#include "mininn/graph/tensor.h"

#include <gtest/gtest.h>

#include <numeric>
#include <functional>

TEST(Tensor, Tensor) {
    auto tensor = std::make_shared<Tensor>();
    EXPECT_TRUE(tensor != nullptr);
}

TEST(Tensor, set_shape) {
    auto tensor = std::make_shared<Tensor>();
    std::vector<int> shape = {1, 3, 224, 224};
    tensor->set_shape(shape);

    std::vector<int> shape_ = tensor->get_shape();
    EXPECT_EQ(shape_, shape);

    int size = std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>());
    int size_ = tensor->get_size();
    EXPECT_EQ(size_, size);

    void* buffer_ = tensor->get_buffer();
    EXPECT_TRUE(buffer_ != nullptr);
}