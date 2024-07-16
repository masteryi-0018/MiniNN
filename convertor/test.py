import os
import sys

# by use this, you won't use pip install to install flatbuffers locally
sys.path.append(os.path.join(os.path.dirname(__file__), '../third_party/flatbuffers/python'))
import flatbuffers

import mininn.Op
import mininn.Node
import mininn.Tensor
import mininn.Graph
import mininn.Node

# Example of how to use FlatBuffers to create and read binary buffers.

def main():
    builder = flatbuffers.Builder(74741)

    # node
    nodes_list = []
    mininn.Node.NodeStartInputsVector(builder, 2)
    for i in range(2):
        builder.PrependInt32(i)
    inputs = builder.EndVector(2)

    mininn.Node.NodeStartOutputsVector(builder, 1)
    for i in range(1):
        builder.PrependInt32(2)
    outputs = builder.EndVector(1)

    mininn.Node.NodeStart(builder)
    mininn.Node.NodeAddType(builder, mininn.Op.Op().ADD)
    mininn.Node.NodeAddInputs(builder, inputs)
    mininn.Node.NodeAddOutputs(builder, outputs)
    node = mininn.Node.NodeEnd(builder)
    nodes_list.append(node)

    # tensor
    mininn.Tensor.TensorStartShapeVector(builder, 4)
    builder.PrependInt32(1)
    builder.PrependInt32(3)
    builder.PrependInt32(224)
    builder.PrependInt32(224)
    shape = builder.EndVector(4)

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
    for i in range(2):
        builder.PrependInt32(i)
    graph_inputs = builder.EndVector(2)

    mininn.Graph.GraphStartOutputsVector(builder, 1)
    for i in range(1):
        builder.PrependInt32(2)
    graph_outputs = builder.EndVector(1)

    mininn.Graph.GraphStartNodesVector(builder, len(nodes_list))
    # todo: why reversed?
    for i in reversed(range(len(nodes_list))):
        builder.PrependUOffsetTRelative(nodes_list[i])
    nodes = builder.EndVector(len(nodes_list))

    mininn.Graph.GraphStartTensorsVector(builder, len(tensors_list))
    # todo: why reversed?
    for i in reversed(range(len(tensors_list))):
        builder.PrependUOffsetTRelative(tensors_list[i])
    tensors = builder.EndVector(len(tensors_list))

    mininn.Graph.GraphStart(builder)
    mininn.Graph.GraphAddNodes(builder, nodes)
    mininn.Graph.GraphAddTensors(builder, tensors)
    mininn.Graph.GraphAddInputs(builder, graph_inputs)
    mininn.Graph.GraphAddOutputs(builder, graph_outputs)
    graph = mininn.Graph.GraphEnd(builder)

    builder.Finish(graph)
    buf = builder.Output()
    with open("mininn_test.gynn", 'wb') as of:
        of.write(buf)

if __name__ == '__main__':
    main()