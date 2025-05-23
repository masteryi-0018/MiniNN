#include "mininn/utils/utils.h"

bool is_equal(float* tensor1, float* tensor2, int size) {
    for (int i = 0; i < size; ++i) {
        if (tensor1[i] != tensor2[i]) {
            return false;
        }
    }
    return true;
}