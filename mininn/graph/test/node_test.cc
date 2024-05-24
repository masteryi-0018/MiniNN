#include "node.h"

#include <gtest/gtest.h>

TEST(nodeTest, createNode) {
    Node* node = new Node();
    ASSERT_TRUE(node != nullptr);
}

TEST(nodeTest, get_inputs) {
    Node* node = new Node();
    std::vector<int> indices = {0, 1};
    node->set_inputs(indices);
    EXPECT_EQ(node->get_inputs(), indices);
}

TEST(nodeTest, set_inputs) {
    Node* node = new Node();
    std::vector<int> indices = {0, 1};
    node->set_inputs(indices);
    EXPECT_EQ(node->get_inputs(), indices);
}

TEST(nodeTest, get_outputs) {
    Node* node = new Node();
    std::vector<int> indices = {2};
    node->set_outputs(indices);
    EXPECT_EQ(node->get_outputs(), indices);
}

TEST(nodeTest, set_outputs) {
    Node* node = new Node();
    std::vector<int> indices = {2};
    node->set_outputs(indices);
    EXPECT_EQ(node->get_outputs(), indices);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}