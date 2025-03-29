#ifndef UTILS_H
#define UTILS_H

#include "mininn/graph/graph.h"
#include "mininn/parser/parser.h"

#include <iostream>

bool is_equal(float* tensor1, float* tensor2, int size);

std::shared_ptr<Graph> load_graph(std::string filename);

template <typename T>
void print_vector(const std::vector<T>& vec) {
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i != vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

#endif // UTILS_H