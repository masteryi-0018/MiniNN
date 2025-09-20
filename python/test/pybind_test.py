import os
import sys

sys.path.append(os.path.join(os.path.dirname(__file__), "../../build/python"))
sys.path.append(os.path.join(os.path.dirname(__file__), "../../bazel-bin/python"))

import mininn_capi
import numpy as np


def main():
    # Tensor
    tensor = mininn_capi.Tensor()
    tensor.set_shape([1, 3, 224, 224])
    print(tensor.get_shape())
    print(tensor.get_size())
    print(tensor.get_buffer())

    # parser
    filename = "../../models/mininn_test.gynn"
    graph = mininn_capi.Graph()
    mininn_capi.load_model(filename, graph)

    # graph
    node_num = len(graph.get_nodes())
    print(f"graph has {node_num} nodes")  # py3.6+
    tensor_num = len(graph.get_tensors())
    print(f"graph has {tensor_num} tensors")
    tensors = graph.get_tensors()
    for i in range(tensor_num):
        print(
            "tensor {} shape is {}".format(i, tensors[i].get_shape())
        )  # py2.7+ / py3.x
    print(graph.get_inputs())
    print(graph.get_outputs())

    # predictor
    predictor = mininn_capi.Predictor(graph)
    input = predictor.get_input_tensors()
    output = predictor.get_output_tensors()
    for i in range(len(input)):
        print(f"input tensor {i} shape is {input[i].get_shape()}")
    for i in range(len(output)):
        print(f"output tensor {i} shape is {output[i].get_shape()}")

    input1_size = input[0].get_size()
    input2_size = input[1].get_size()
    output_size = output[0].get_size()
    a = np.full(input1_size, 1.0, dtype=np.float32)
    b = np.full(input2_size, 2.0, dtype=np.float32)
    input[0].set_data(a)
    input[1].set_data(b)
    predictor.prepare()
    predictor.run()
    c = output[0].get_data()
    print(c[0], output[0].get_shape())

    golden = np.full(output_size, 3.0, dtype=np.float32)
    assert c == golden
    return


if __name__ == "__main__":
    main()
