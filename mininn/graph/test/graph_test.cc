#include "use_op.h"

#include "graph.h"
#include "node.h"
#include "register.h"
#include "type.h"

#include <gtest/gtest.h>

TEST(graphTest, add_node) {
    Graph* graph = new Graph();

    // 通过type创建具体的node，也就是op
    std::shared_ptr<Node> node = OpFactory::Global().Create(ADD);

    graph->add_node(node, {0, 1}, {2});
    int size = graph->get_nodes().size();
    EXPECT_EQ(size, 1);
}