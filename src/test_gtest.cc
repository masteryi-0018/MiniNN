#include <gtest/gtest.h>

// 测试用例1：测试两个整数相加
TEST(AdditionTest, PositiveNumbers) {
  int a = 2;
  int b = 3;
  int result = a + b;
  EXPECT_EQ(result, 5);
}

// 测试用例2：测试两个负数相加
TEST(AdditionTest, NegativeNumbers) {
  int a = -2;
  int b = -3;
  int result = a + b;
  EXPECT_EQ(result, -5);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}