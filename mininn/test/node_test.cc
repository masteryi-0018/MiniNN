#include "mininn/graph/node.h"

#include <gtest/gtest.h>

TEST(nodeTest, createNode) {
    Node* node = new Node(ADD);
    ASSERT_TRUE(node != nullptr);
}

TEST(nodeTest, set_inputs) {
    Node* node = new Node(ADD);
    std::vector<int> indices = {0, 1};
    node->set_inputs(indices);
    EXPECT_EQ(node->get_inputs(), indices);
}

TEST(nodeTest, set_outputs) {
    Node* node = new Node(ADD);
    std::vector<int> indices = {2};
    node->set_outputs(indices);
    EXPECT_EQ(node->get_outputs(), indices);
}