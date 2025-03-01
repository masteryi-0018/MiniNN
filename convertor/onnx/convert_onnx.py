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


def print_node_info(onnx_node):
    print(onnx_node.name)
    # print(onnx_node.input)
    # print(onnx_node.output)
    print(onnx_node.op_type)
    # print(onnx_node.domain)
    # print(onnx_node.attribute)
    # print(onnx_node.doc_string)
    # print(onnx_node.overload)
    # print(onnx_node.metadata_props)


def print_tensor_info(i):
    print(i.dims)
    print(i.data_type)
    # print(i.segment)
    # print(i.float_data)
    # print(i.int32_data)
    # print(i.string_data)
    # print(i.int64_data)
    print(i.name)
    # print(i.doc_string)
    # print(i.raw_data)
    # print(i.external_data)
    # print(i.data_location)
    # print(i.double_data)
    # print(i.uint64_data)
    # print(i.metadata_props)


def print_value_info(i):
    print(i.name)
    print(i.type)
    # print(i.doc_string)
    # print(i.metadata_props)
    # print(i.type.elem_type)
    # print(i.type.tensor_type.elem_type)
    # print(i.type.tensor_type.shape)
    # print(i.type.tensor_type.shape.dim)


def print_graph_info(onnx_graph):
    print("node 数量:", len(onnx_graph.node))
    # print("name:", onnx_graph.name)
    print("initializer 数量:", len(onnx_graph.initializer))
    # print("sparse_initializer 数量:", len(onnx_graph.sparse_initializer))
    # print("doc_string:", onnx_graph.doc_string)
    print("input:", [input.name for input in onnx_graph.input])
    print("output:", [output.name for output in onnx_graph.output])
    print("value_info 数量:", len(onnx_graph.value_info))
    # print("quantization_annotation 数量:", len(onnx_graph.quantization_annotation))
    # print("metadata_props:", onnx_graph.metadata_props)


def load_onnx_model():
    onnx_model = onnx.load("/home/gy/proj/MiniNN/mobilenetv2-10.onnx")
    onnx_model_shape = onnx.shape_inference.infer_shapes(onnx_model)
    onnx_graph = onnx_model_shape.graph
    print_graph_info(onnx_graph)

    return onnx_graph


def build_mininn(onnx_graph):
    builder = flatbuffers.Builder(74741)

    nodes_list = []
    tensors_list = []
    build_node(builder, onnx_graph, nodes_list, tensors_list)
    graph = build_graph(builder, nodes_list, tensors_list)
    
    builder.Finish(graph)
    with open("onnx_add_100_3_224_224.gynn", "wb") as f:
        f.write(builder.Output())
    print("add_model.onnx 模型已转换为 onnx_add_100_3_224_224.gynn")


def build_node(builder, onnx_graph, nodes_list, tensors_list):
    tensor_idx = 0
    for onnx_node in onnx_graph.node[:1]:
        # print_node_info(onnx_node)

        inputs = build_tensor(builder, onnx_node.input, tensors_list, tensor_idx)
        tensor_idx += len(inputs)
        print(inputs)
        outputs = build_tensor(builder, onnx_node.output, tensors_list, tensor_idx)
        tensor_idx += len(outputs)
        print(outputs)

        node_inputs = builder.CreateNumpyVector(np.array(inputs, dtype=np.int32))
        node_outputs = builder.CreateNumpyVector(np.array(outputs, dtype=np.int32))

        mininn_fbs.Node.NodeStart(builder)
        mininn_fbs.Node.NodeAddType(builder, mininn_fbs.Op.Op().ADD)
        mininn_fbs.Node.NodeAddInputs(builder, node_inputs)
        mininn_fbs.Node.NodeAddOutputs(builder, node_outputs)
        node = mininn_fbs.Node.NodeEnd(builder)
        nodes_list.append(node)


def build_tensor(builder, tensors, tensors_list, tensor_idx):
    idx_list = []
    for onnx_tensor in tensors:
        print(onnx_tensor)
        for i in onnx_graph.initializer:
            if i.name == onnx_tensor:
                # print_tensor_info(i)
                print("i from initializer")
                shape_list = i.dims
                shape = builder.CreateNumpyVector(np.array(shape_list, dtype=np.int32))
                raw_data = i.raw_data
                data = builder.CreateNumpyVector(np.frombuffer(raw_data, dtype=np.uint8))

                mininn_fbs.Tensor.TensorStart(builder)
                mininn_fbs.Tensor.TensorAddShape(builder, shape)
                mininn_fbs.Tensor.TensorAddData(builder, data)
                tensor = mininn_fbs.Tensor.TensorEnd(builder)
                tensors_list.append(tensor)
                idx_list.append(tensor_idx)
                tensor_idx += 1

        for v in onnx_graph.value_info:
            if v.name == onnx_tensor:
                # print_value_info(v)
                print("v from value_info")
                shape_list = []
                for dim in v.type.tensor_type.shape.dim:
                    if dim.dim_param == "batch_size":
                        shape_list.append(1)
                    else:
                        shape_list.append(dim.dim_value)
                shape = builder.CreateNumpyVector(np.array(shape_list, dtype=np.int32))

                mininn_fbs.Tensor.TensorStart(builder)
                mininn_fbs.Tensor.TensorAddShape(builder, shape)
                mininn_fbs.Tensor.TensorAddData(builder, 0)
                tensor = mininn_fbs.Tensor.TensorEnd(builder)
                tensors_list.append(tensor)
                idx_list.append(tensor_idx)
                tensor_idx += 1

        for i in onnx_graph.input:
            if i.name == onnx_tensor:
                # print_value_info(i)
                print("i from input")
                shape_list = []
                for dim in i.type.tensor_type.shape.dim:
                    if dim.dim_param == "batch_size":
                        shape_list.append(1)
                    else:
                        shape_list.append(dim.dim_value)
                shape = builder.CreateNumpyVector(np.array(shape_list, dtype=np.int32))

                mininn_fbs.Tensor.TensorStart(builder)
                mininn_fbs.Tensor.TensorAddShape(builder, shape)
                mininn_fbs.Tensor.TensorAddData(builder, 0)
                tensor = mininn_fbs.Tensor.TensorEnd(builder)
                tensors_list.append(tensor)
                idx_list.append(tensor_idx)
                tensor_idx += 1
    
        for o in onnx_graph.output:
            if o.name == onnx_tensor:
                # print_value_info(o)
                print("o from output")
                shape_list = []
                for dim in o.type.tensor_type.shape.dim:
                    if dim.dim_param == "batch_size":
                        shape_list.append(1)
                    else:
                        shape_list.append(dim.dim_value)
                shape = builder.CreateNumpyVector(np.array(shape_list, dtype=np.int32))

                mininn_fbs.Tensor.TensorStart(builder)
                mininn_fbs.Tensor.TensorAddShape(builder, shape)
                mininn_fbs.Tensor.TensorAddData(builder, 0)
                tensor = mininn_fbs.Tensor.TensorEnd(builder)
                tensors_list.append(tensor) 
                idx_list.append(tensor_idx)
                tensor_idx += 1

    return idx_list


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
    # output = [2]
    # for i in range(graph.OutputsLength()):
    #     assert graph.Outputs(i) == output[i]
    
    # input = [0, 1]
    # for i in range(graph.InputsLength()):
    #     assert graph.Inputs(i) == input[i]
    
    # assert graph.NodesLength() == 1
    # assert graph.TensorsLength() == 3

    # # node
    # node = graph.Nodes(0)
    # assert node.Type() == mininn_fbs.Op.Op.ADD

    # output = [2]
    # for i in range(node.OutputsLength()):
    #     assert node.Outputs(i) == output[i]
    
    # input = [0, 1]
    # for i in range(node.InputsLength()):
    #     assert node.Inputs(i) == input[i]
    
    # tensor
    for i in range(graph.TensorsLength()):
        tensor = graph.Tensors(i)
        # use xxxAsNumpy, you should pip install numpy
        shape = tensor.ShapeAsNumpy()
        print(shape)
        # assert (shape == [100, 3, 224, 224]).all()
        data_numpy = tensor.DataAsNumpy()
        if isinstance(data_numpy, int) and data_numpy == 0:
            print("No data available.")
            print(data_numpy)
        else:
            data_bytes = data_numpy.tobytes()
            data = np.frombuffer(data_bytes, dtype=np.float32)
            print(data[0])
        # assert data == 0

if __name__ == '__main__':
    onnx_graph = load_onnx_model()
    build_mininn(onnx_graph)
    check_mininn()
