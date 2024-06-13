#include "tensor.h"

#include <gtest/gtest.h>

TEST(tensorTest, createTensor) {
    std::vector<int> shape = {1, 3, 224, 224};
    Tensor* tensor = new Tensor(shape);
    ASSERT_TRUE(tensor != nullptr);
}

TEST(tensorTest, createTensorFromData) {
    std::vector<int> shape = {1, 3, 224, 224};
    int size = std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>());
    std::vector<float> data;
    data.resize(size);
    for (int i = 0; i < size; ++i) {
        data[i] = 1.0f;
    }
    Tensor* tensor = new Tensor(shape, data);
    ASSERT_TRUE(tensor != nullptr);
}

TEST(tensorTest, plusTensor) {
    std::vector<int> shape1 = {2, 3, 4};
    int size1 = std::accumulate(shape1.begin(), shape1.end(), 1, std::multiplies<int>());
    std::vector<float> data1(size1, 0);
    for (int i = 0; i < size1; ++i) {
        data1[i] = 1.0f;
    }
    Tensor* tensor1 = new Tensor(shape1, data1);

    std::vector<int> shape2 = {2, 3, 4};
    int size2 = std::accumulate(shape2.begin(), shape2.end(), 1, std::multiplies<int>());
    std::vector<float> data2(size2, 0);
    for (int i = 0; i < size2; ++i) {
        data2[i] = 2.0f;
    }
    Tensor* tensor2 = new Tensor(shape2, data2);

    std::vector<float> result(size1, 3.0f);
    Tensor* tensor3 = new Tensor(shape1);
    *tensor3 = *tensor1 + *tensor2;
    EXPECT_EQ(tensor3->data(), result);
}

TEST(tensorTest, minusTensor) {
    std::vector<int> shape1 = {2, 3, 4};
    int size1 = std::accumulate(shape1.begin(), shape1.end(), 1, std::multiplies<int>());
    std::vector<float> data1(size1, 0);
    for (int i = 0; i < size1; ++i) {
        data1[i] = 3.0f;
    }
    Tensor* tensor1 = new Tensor(shape1, data1);

    std::vector<int> shape2 = {2, 3, 4};
    int size2 = std::accumulate(shape2.begin(), shape2.end(), 1, std::multiplies<int>());
    std::vector<float> data2(size2, 0);
    for (int i = 0; i < size2; ++i) {
        data2[i] = 2.0f;
    }
    Tensor* tensor2 = new Tensor(shape2, data2);

    std::vector<float> result(size1, 1.0f);
    Tensor* tensor3 = new Tensor(shape1);
    *tensor3 = *tensor1 - *tensor2;
    EXPECT_EQ(tensor3->data(), result);
}

TEST(tensorTest, dotTensor) {
    std::vector<int> shape1 = {2, 3, 4};
    int size1 = std::accumulate(shape1.begin(), shape1.end(), 1, std::multiplies<int>());
    std::vector<float> data1(size1, 0);
    for (int i = 0; i < size1; ++i) {
        data1[i] = 2.0f;
    }
    Tensor* tensor1 = new Tensor(shape1, data1);

    std::vector<int> shape2 = {2, 3, 4};
    int size2 = std::accumulate(shape2.begin(), shape2.end(), 1, std::multiplies<int>());
    std::vector<float> data2(size2, 0);
    for (int i = 0; i < size2; ++i) {
        data2[i] = 3.0f;
    }
    Tensor* tensor2 = new Tensor(shape2, data2);

    std::vector<float> result(size1, 6.0f);
    Tensor* tensor3 = new Tensor(shape1);
    *tensor3 = *tensor1 * *tensor2;
    EXPECT_EQ(tensor3->data(), result);
}

TEST(tensorTest, divTensor) {
    std::vector<int> shape1 = {2, 3, 4};
    int size1 = std::accumulate(shape1.begin(), shape1.end(), 1, std::multiplies<int>());
    std::vector<float> data1(size1, 0);
    for (int i = 0; i < size1; ++i) {
        data1[i] = 6.0f;
    }
    Tensor* tensor1 = new Tensor(shape1, data1);

    std::vector<int> shape2 = {2, 3, 4};
    int size2 = std::accumulate(shape2.begin(), shape2.end(), 1, std::multiplies<int>());
    std::vector<float> data2(size2, 0);
    for (int i = 0; i < size2; ++i) {
        data2[i] = 2.0f;
    }
    Tensor* tensor2 = new Tensor(shape2, data2);

    std::vector<float> result(size1, 3.0f);
    Tensor* tensor3 = new Tensor(shape1);
    *tensor3 = *tensor1 / *tensor2;
    EXPECT_EQ(tensor3->data(), result);
}

TEST(tensorTest, shapeTensor) {
    std::vector<int> shape = {2, 3, 4};
    Tensor* tensor = new Tensor(shape);
    std::vector<int> result = {2, 3, 4};
    EXPECT_EQ(tensor->shape(), result);
}

TEST(tensorTest, sizeTensor) {
    std::vector<int> shape = {2, 3, 4};
    Tensor* tensor = new Tensor(shape);
    int result = 24;
    EXPECT_EQ(tensor->size(), result);
}

TEST(tensorTest, dataTensor) {
    std::vector<int> shape = {2, 3, 4};
    int size = std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>());
    std::vector<float> data;
    data.resize(size);
    for (int i = 0; i < size; ++i) {
        data[i] = 1.0f;
    }
    Tensor* tensor = new Tensor(shape, data);

    std::vector<float> result(size, 1.0f);
    EXPECT_EQ(tensor->data(), result);
}