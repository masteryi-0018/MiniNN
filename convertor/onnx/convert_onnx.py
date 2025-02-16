import os
import sys

sys.path.append(os.path.join(os.path.dirname(__file__), '../third_party/flatbuffers/python'))
import flatbuffers

sys.path.append(os.path.join(os.path.dirname(__file__), '../'))
import mininn_fbs.Op
import mininn_fbs.Node
import mininn_fbs.Tensor
import mininn_fbs.Graph

import onnx
import numpy as np


def load_onnx_model():
    onnx_model = onnx.load("add_model.onnx")
    onnx_graph = onnx_model.graph

    print("模型输入:", [input.name for input in onnx_graph.input])
    print("模型输出:", [output.name for output in onnx_graph.output])
    print("node 数量:", len(onnx_graph.node))
    print("initializer 数量:", len(onnx_graph.initializer))
    print("value_info 数量:", len(onnx_graph.value_info))
    return onnx_graph


def build_mininn(onnx_graph):
    builder = flatbuffers.Builder(74741)

    nodes_list = build_node(builder, onnx_graph)
    tensors_list = build_tensor(builder, onnx_graph)
    graph = build_graph(builder, nodes_list, tensors_list)
    
    builder.Finish(graph)
    with open("onnx_add_100_3_224_224.gynn", "wb") as f:
        f.write(builder.Output())
    print("add_model.onnx 模型已转换为 onnx_add_100_3_224_224.gynn")


def build_node(builder, onnx_graph):
    tensor_idx = 0
    nodes_list = []
    for onnx_node in onnx_graph.node:
        inputs = [int(i.replace("input", "")) + tensor_idx for i in onnx_node.input]
        tensor_idx += len(inputs)
        outputs = [int(o.replace("output", "")) + tensor_idx for o in onnx_node.output]
        tensor_idx += len(outputs)

        node_inputs = builder.CreateNumpyVector(np.array(inputs, dtype=np.int32))
        node_outputs = builder.CreateNumpyVector(np.array(outputs, dtype=np.int32))

        mininn_fbs.Node.NodeStart(builder)
        mininn_fbs.Node.NodeAddType(builder, mininn_fbs.Op.Op().ADD)
        mininn_fbs.Node.NodeAddInputs(builder, node_inputs)
        mininn_fbs.Node.NodeAddOutputs(builder, node_outputs)
        node = mininn_fbs.Node.NodeEnd(builder)
        nodes_list.append(node)

    return nodes_list


def build_tensor(builder, onnx_graph):
    tensors_list = []
    for i in onnx_graph.input:
        shape_list = []
        for dim in i.type.tensor_type.shape.dim:
            shape_list.append(dim.dim_value)

        shape = builder.CreateNumpyVector(np.array(shape_list, dtype=np.int32))

        mininn_fbs.Tensor.TensorStart(builder)
        mininn_fbs.Tensor.TensorAddShape(builder, shape)
        mininn_fbs.Tensor.TensorAddData(builder, 0)
        tensor = mininn_fbs.Tensor.TensorEnd(builder)
        tensors_list.append(tensor)

    for o in onnx_graph.output:
        shape_list = []
        for dim in o.type.tensor_type.shape.dim:
            shape_list.append(dim.dim_value)

        shape = builder.CreateNumpyVector(np.array(shape_list, dtype=np.int32))

        mininn_fbs.Tensor.TensorStart(builder)
        mininn_fbs.Tensor.TensorAddShape(builder, shape)
        mininn_fbs.Tensor.TensorAddData(builder, 0)
        tensor = mininn_fbs.Tensor.TensorEnd(builder)
        tensors_list.append(tensor) 

    return tensors_list


def build_graph(builder, nodes_list, tensors_list):
    graph_inputs = builder.CreateNumpyVector(np.array([0, 1], dtype=np.int32))
    graph_outputs = builder.CreateNumpyVector(np.array([2], dtype=np.int32))
    
    mininn_fbs.Graph.GraphStartNodesVector(builder, len(nodes_list))
    for i in reversed(range(len(nodes_list))):
        builder.PrependUOffsetTRelative(nodes_list[i])
    nodes = builder.EndVector()

    mininn_fbs.Graph.GraphStartTensorsVector(builder, len(tensors_list))
    for i in reversed(range(len(tensors_list))):
        builder.PrependUOffsetTRelative(tensors_list[i])
    tensors = builder.EndVector()

    # 创建 Graph
    mininn_fbs.Graph.GraphStart(builder)
    mininn_fbs.Graph.GraphAddNodes(builder, nodes)
    mininn_fbs.Graph.GraphAddTensors(builder, tensors)
    mininn_fbs.Graph.GraphAddInputs(builder, graph_inputs)
    mininn_fbs.Graph.GraphAddOutputs(builder, graph_outputs)
    graph = mininn_fbs.Graph.GraphEnd(builder)
    
    return graph


def check_mininn():
    read("onnx_add_100_3_224_224.gynn")


def read(model_path):
    # read
    with open(model_path, "rb") as f:
        buf = f.read()
    
    graph = mininn_fbs.Graph.Graph.GetRootAsGraph(buf)

    # graph
    output = [2]
    for i in range(graph.OutputsLength()):
        assert graph.Outputs(i) == output[i]
    
    input = [0, 1]
    for i in range(graph.InputsLength()):
        assert graph.Inputs(i) == input[i]
    
    assert graph.NodesLength() == 1
    assert graph.TensorsLength() == 3

    # node
    node = graph.Nodes(0)
    assert node.Type() == mininn_fbs.Op.Op.ADD

    output = [2]
    for i in range(node.OutputsLength()):
        assert node.Outputs(i) == output[i]
    
    input = [0, 1]
    for i in range(node.InputsLength()):
        assert node.Inputs(i) == input[i]
    
    # tensor
    for i in range(graph.TensorsLength()):
        tensor = graph.Tensors(i)
        # use xxxAsNumpy, you should pip install numpy
        shape = tensor.ShapeAsNumpy()
        assert (shape == [100, 3, 224, 224]).all()
        data = tensor.DataAsNumpy()
        assert data == 0

if __name__ == '__main__':
    onnx_graph = load_onnx_model()
    build_mininn(onnx_graph)
    check_mininn()
