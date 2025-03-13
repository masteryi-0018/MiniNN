#ifndef UTILS_H
#define UTILS_H

#include "mininn/graph/graph.h"
#include "mininn/parser/parser.h"

bool is_equal(float* tensor1, float* tensor2, int size);

std::shared_ptr<Graph> load_graph(std::string filename);

#endif // UTILS_H