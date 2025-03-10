import os
import sys

# by use this, you won't use pip install to install flatbuffers locally
sys.path.append(os.path.join(os.path.dirname(__file__), '../third_party/flatbuffers/python'))
import flatbuffers

import mininn_fbs.Op
import mininn_fbs.Attribute
import mininn_fbs.Node
import mininn_fbs.Tensor
import mininn_fbs.Graph

import numpy as np

# Example of how to use FlatBuffers to create and read binary buffers.

def write():
    builder = flatbuffers.Builder(74741)

    # node
    nodes_list = []
    mininn_fbs.Node.NodeStartInputsVector(builder, 2)
    input_list = [0, 1]
    for i in reversed(range(len(input_list))):
        builder.PrependInt32(input_list[i])
    inputs = builder.EndVector()

    mininn_fbs.Node.NodeStartOutputsVector(builder, 1)
    output_list = [2]
    for i in reversed(range(len(output_list))):
        builder.PrependInt32(output_list[i])
    outputs = builder.EndVector()

    attributes_vector = []
    key = builder.CreateString("add_test")
    value_array = builder.CreateNumpyVector(np.array([0, 0], dtype=np.int32))

    mininn_fbs.Attribute.AttributeStart(builder)
    mininn_fbs.Attribute.AttributeAddKey(builder, key)
    mininn_fbs.Attribute.AttributeAddValue(builder, value_array)
    attr = mininn_fbs.Attribute.AttributeEnd(builder)
    attributes_vector.append(attr)
    
    mininn_fbs.Node.NodeStartAttributesVector(builder, len(attributes_vector))
    for attribute in reversed(attributes_vector):
        builder.PrependUOffsetTRelative(attribute)
    attributes = builder.EndVector()

    mininn_fbs.Node.NodeStart(builder)
    mininn_fbs.Node.NodeAddType(builder, mininn_fbs.Op.Op().ADD)
    mininn_fbs.Node.NodeAddAttributes(builder, attributes)
    mininn_fbs.Node.NodeAddInputs(builder, inputs)
    mininn_fbs.Node.NodeAddOutputs(builder, outputs)
    node = mininn_fbs.Node.NodeEnd(builder)
    nodes_list.append(node)

    # tensor
    mininn_fbs.Tensor.TensorStartShapeVector(builder, 4)
    shape_list = [1, 3, 224, 224]
    for i in reversed(range(len(shape_list))):
        builder.PrependInt32(shape_list[i])
    shape = builder.EndVector()

    tensors_list = []
    mininn_fbs.Tensor.TensorStart(builder)
    mininn_fbs.Tensor.TensorAddShape(builder, shape)
    mininn_fbs.Tensor.TensorAddData(builder, 0)
    tensor1 = mininn_fbs.Tensor.TensorEnd(builder)
    tensors_list.append(tensor1)

    mininn_fbs.Tensor.TensorStart(builder)
    mininn_fbs.Tensor.TensorAddShape(builder, shape)
    mininn_fbs.Tensor.TensorAddData(builder, 0)
    tensor2 = mininn_fbs.Tensor.TensorEnd(builder)
    tensors_list.append(tensor2)

    mininn_fbs.Tensor.TensorStart(builder)
    mininn_fbs.Tensor.TensorAddShape(builder, shape)
    mininn_fbs.Tensor.TensorAddData(builder, 0)
    tensor3 = mininn_fbs.Tensor.TensorEnd(builder)
    tensors_list.append(tensor3)

    # graph
    mininn_fbs.Graph.GraphStartInputsVector(builder, 2)
    input_list = [0, 1]
    for i in reversed(range(len(input_list))):
        builder.PrependInt32(input_list[i])
    graph_inputs = builder.EndVector()

    mininn_fbs.Graph.GraphStartOutputsVector(builder, 1)
    output_list = [2]
    for i in reversed(range(len(output_list))):
        builder.PrependInt32(output_list[i])
    graph_outputs = builder.EndVector()

    mininn_fbs.Graph.GraphStartNodesVector(builder, len(nodes_list))
    # todo: why reversed?
    # Note: Since we prepend the bytes, this loop iterates in reverse order.
    for i in reversed(range(len(nodes_list))):
        builder.PrependUOffsetTRelative(nodes_list[i])
    nodes = builder.EndVector()

    mininn_fbs.Graph.GraphStartTensorsVector(builder, len(tensors_list))
    for i in reversed(range(len(tensors_list))):
        builder.PrependUOffsetTRelative(tensors_list[i])
    tensors = builder.EndVector()

    mininn_fbs.Graph.GraphStart(builder)
    mininn_fbs.Graph.GraphAddNodes(builder, nodes)
    mininn_fbs.Graph.GraphAddTensors(builder, tensors)
    mininn_fbs.Graph.GraphAddInputs(builder, graph_inputs)
    mininn_fbs.Graph.GraphAddOutputs(builder, graph_outputs)
    graph = mininn_fbs.Graph.GraphEnd(builder)

    builder.Finish(graph)
    buf = builder.Output()
    with open("mininn_test.gynn", 'wb') as f:
        f.write(buf)

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
        assert (shape == [1, 3, 224, 224]).all()
        data = tensor.DataAsNumpy()
        assert data == 0


if __name__ == '__main__':
    write()
    read("mininn_test.gynn")
