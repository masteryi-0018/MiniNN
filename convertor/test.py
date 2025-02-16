import os
import sys

# by use this, you won't use pip install to install flatbuffers locally
sys.path.append(os.path.join(os.path.dirname(__file__), '../third_party/flatbuffers/python'))
import flatbuffers

import mininn.Op
import mininn.Node
import mininn.Tensor
import mininn.Graph

# Example of how to use FlatBuffers to create and read binary buffers.

def write():
    builder = flatbuffers.Builder(74741)

    # node
    nodes_list = []
    mininn.Node.NodeStartInputsVector(builder, 2)
    input_list = [0, 1]
    for i in reversed(range(len(input_list))):
        builder.PrependInt32(input_list[i])
    inputs = builder.EndVector()

    mininn.Node.NodeStartOutputsVector(builder, 1)
    output_list = [2]
    for i in reversed(range(len(output_list))):
        builder.PrependInt32(output_list[i])
    outputs = builder.EndVector()

    mininn.Node.NodeStart(builder)
    mininn.Node.NodeAddType(builder, mininn.Op.Op().ADD)
    mininn.Node.NodeAddInputs(builder, inputs)
    mininn.Node.NodeAddOutputs(builder, outputs)
    node = mininn.Node.NodeEnd(builder)
    nodes_list.append(node)

    # tensor
    mininn.Tensor.TensorStartShapeVector(builder, 4)
    shape_list = [1, 3, 224, 224]
    for i in reversed(range(len(shape_list))):
        builder.PrependInt32(shape_list[i])
    shape = builder.EndVector()

    tensors_list = []
    mininn.Tensor.TensorStart(builder)
    mininn.Tensor.TensorAddShape(builder, shape)
    mininn.Tensor.TensorAddData(builder, 0)
    tensor1 = mininn.Tensor.TensorEnd(builder)
    tensors_list.append(tensor1)

    mininn.Tensor.TensorStart(builder)
    mininn.Tensor.TensorAddShape(builder, shape)
    mininn.Tensor.TensorAddData(builder, 0)
    tensor2 = mininn.Tensor.TensorEnd(builder)
    tensors_list.append(tensor2)

    mininn.Tensor.TensorStart(builder)
    mininn.Tensor.TensorAddShape(builder, shape)
    mininn.Tensor.TensorAddData(builder, 0)
    tensor3 = mininn.Tensor.TensorEnd(builder)
    tensors_list.append(tensor3)

    # graph
    mininn.Graph.GraphStartInputsVector(builder, 2)
    input_list = [0, 1]
    for i in reversed(range(len(input_list))):
        builder.PrependInt32(input_list[i])
    graph_inputs = builder.EndVector()

    mininn.Graph.GraphStartOutputsVector(builder, 1)
    output_list = [2]
    for i in reversed(range(len(output_list))):
        builder.PrependInt32(output_list[i])
    graph_outputs = builder.EndVector()

    mininn.Graph.GraphStartNodesVector(builder, len(nodes_list))
    # todo: why reversed?
    # Note: Since we prepend the bytes, this loop iterates in reverse order.
    for i in reversed(range(len(nodes_list))):
        builder.PrependUOffsetTRelative(nodes_list[i])
    nodes = builder.EndVector()

    mininn.Graph.GraphStartTensorsVector(builder, len(tensors_list))
    for i in reversed(range(len(tensors_list))):
        builder.PrependUOffsetTRelative(tensors_list[i])
    tensors = builder.EndVector()

    mininn.Graph.GraphStart(builder)
    mininn.Graph.GraphAddNodes(builder, nodes)
    mininn.Graph.GraphAddTensors(builder, tensors)
    mininn.Graph.GraphAddInputs(builder, graph_inputs)
    mininn.Graph.GraphAddOutputs(builder, graph_outputs)
    graph = mininn.Graph.GraphEnd(builder)

    builder.Finish(graph)
    buf = builder.Output()
    with open("mininn_test.gynn", 'wb') as f:
        f.write(buf)

def read(model_path):
    # read
    with open(model_path, "rb") as f:
        buf = f.read()
    
    graph = mininn.Graph.Graph.GetRootAsGraph(buf)

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
    assert node.Type() == mininn.Op.Op.ADD

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
