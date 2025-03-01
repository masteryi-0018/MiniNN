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


class convertor():
    def __init__(self):
        self.onnx_graph = None
        self.builder = flatbuffers.Builder(74741)
        self.tensor_idx = 0
        self.nodes_list = []
        self.tensors_list = []
        self.tensor_dict = {}
        self.input_list = []
        self.output_list = []


    def load_onnx_model(self):
        onnx_model = onnx.load("/home/gy/proj/MiniNN/mobilenetv2-10.onnx")
        onnx_model_shape = onnx.shape_inference.infer_shapes(onnx_model)
        onnx_graph = onnx_model_shape.graph
        print_graph_info(onnx_graph)
        self.onnx_graph = onnx_graph


    def build_mininn(self):
        self.build_node_and_tensor()
        graph = self.build_graph()
        
        self.builder.Finish(graph)
        with open("onnx_add_100_3_224_224.gynn", "wb") as f:
            f.write(self.builder.Output())
        print("add_model.onnx 模型已转换为 onnx_add_100_3_224_224.gynn \n")


    def build_node_and_tensor(self):
        for onnx_node in self.onnx_graph.node:
            # print_node_info(onnx_node)
            inputs = self.build_tensor(onnx_node.input)
            outputs = self.build_tensor(onnx_node.output)
            # print(f"node {onnx_node.name} inputs is: {onnx_node.input}")
            # print(f"node {onnx_node.name} outputs is: {onnx_node.output}")

            node_inputs = self.builder.CreateNumpyVector(np.array(inputs, dtype=np.int32))
            node_outputs = self.builder.CreateNumpyVector(np.array(outputs, dtype=np.int32))

            mininn_fbs.Node.NodeStart(self.builder)
            mininn_fbs.Node.NodeAddType(self.builder, mininn_fbs.Op.Op().ADD)
            mininn_fbs.Node.NodeAddInputs(self.builder, node_inputs)
            mininn_fbs.Node.NodeAddOutputs(self.builder, node_outputs)
            node = mininn_fbs.Node.NodeEnd(self.builder)
            self.nodes_list.append(node)


    def add_tensor(self, shape, data, idx_list, onnx_tensor):
        mininn_fbs.Tensor.TensorStart(self.builder)
        mininn_fbs.Tensor.TensorAddShape(self.builder, shape)
        mininn_fbs.Tensor.TensorAddData(self.builder, data)
        tensor = mininn_fbs.Tensor.TensorEnd(self.builder)
        self.tensors_list.append(tensor)
        idx_list.append(self.tensor_idx)
        self.tensor_dict[onnx_tensor] = self.tensor_idx
        self.tensor_idx += 1


    def build_tensor(self, tensors):
        idx_list = []
        for onnx_tensor in tensors:
            # print(onnx_tensor)
            if onnx_tensor in self.tensor_dict:
                idx_list.append(self.tensor_dict[onnx_tensor])
                continue
            
            for i in self.onnx_graph.initializer:
                if i.name == onnx_tensor:
                    # print_tensor_info(i)
                    # print("i from initializer")
                    shape_list = i.dims
                    shape = self.builder.CreateNumpyVector(np.array(shape_list, dtype=np.int32))
                    raw_data = i.raw_data
                    data = self.builder.CreateNumpyVector(np.frombuffer(raw_data, dtype=np.uint8))
                    self.add_tensor(shape, data, idx_list, onnx_tensor)

            for v in self.onnx_graph.value_info:
                if v.name == onnx_tensor:
                    # print_value_info(v)
                    # print("v from value_info")
                    shape_list = []
                    for dim in v.type.tensor_type.shape.dim:
                        if dim.dim_param == "batch_size":
                            shape_list.append(1)
                        else:
                            shape_list.append(dim.dim_value)
                    shape = self.builder.CreateNumpyVector(np.array(shape_list, dtype=np.int32))
                    data = 0
                    self.add_tensor(shape, data, idx_list, onnx_tensor)

            for i in self.onnx_graph.input:
                if i.name == onnx_tensor:
                    self.input_list.append(self.tensor_idx)
                    # print_value_info(i)
                    # print("i from input")
                    shape_list = []
                    for dim in i.type.tensor_type.shape.dim:
                        if dim.dim_param == "batch_size":
                            shape_list.append(1)
                        else:
                            shape_list.append(dim.dim_value)
                    shape = self.builder.CreateNumpyVector(np.array(shape_list, dtype=np.int32))
                    data = 0
                    self.add_tensor(shape, data, idx_list, onnx_tensor)
        
            for o in self.onnx_graph.output:
                if o.name == onnx_tensor:
                    self.output_list.append(self.tensor_idx)
                    # print_value_info(o)
                    # print("o from output")
                    shape_list = []
                    for dim in o.type.tensor_type.shape.dim:
                        if dim.dim_param == "batch_size":
                            shape_list.append(1)
                        else:
                            shape_list.append(dim.dim_value)
                    shape = self.builder.CreateNumpyVector(np.array(shape_list, dtype=np.int32))
                    data = 0
                    self.add_tensor(shape, data, idx_list, onnx_tensor)

        return idx_list


    def build_graph(self):
        graph_inputs = self.builder.CreateNumpyVector(np.array(self.input_list, dtype=np.int32))
        graph_outputs = self.builder.CreateNumpyVector(np.array(self.output_list, dtype=np.int32))
        
        mininn_fbs.Graph.GraphStartNodesVector(self.builder, len(self.nodes_list))
        for i in reversed(range(len(self.nodes_list))):
            self.builder.PrependUOffsetTRelative(self.nodes_list[i])
        nodes = self.builder.EndVector()

        mininn_fbs.Graph.GraphStartTensorsVector(self.builder, len(self.tensors_list))
        for i in reversed(range(len(self.tensors_list))):
            self.builder.PrependUOffsetTRelative(self.tensors_list[i])
        tensors = self.builder.EndVector()

        # 创建 Graph
        mininn_fbs.Graph.GraphStart(self.builder)
        mininn_fbs.Graph.GraphAddNodes(self.builder, nodes)
        mininn_fbs.Graph.GraphAddTensors(self.builder, tensors)
        mininn_fbs.Graph.GraphAddInputs(self.builder, graph_inputs)
        mininn_fbs.Graph.GraphAddOutputs(self.builder, graph_outputs)
        graph = mininn_fbs.Graph.GraphEnd(self.builder)
        
        return graph


def check_mininn():
    read("onnx_add_100_3_224_224.gynn")


def read(model_path):
    # read
    with open(model_path, "rb") as f:
        buf = f.read()
    
    graph = mininn_fbs.Graph.Graph.GetRootAsGraph(buf)

    # graph
    output = [212]
    for i in range(graph.OutputsLength()):
        assert graph.Outputs(i) == output[i]
    
    input = [0]
    for i in range(graph.InputsLength()):
        assert graph.Inputs(i) == input[i]
    
    assert graph.NodesLength() == 105
    # initializer num + value_info num + input num + output num
    assert graph.TensorsLength() == 107 + 104 + 1 + 1

    # node
    node = graph.Nodes(0)
    assert node.Type() == mininn_fbs.Op.Op.ADD

    output = [3]
    for i in range(node.OutputsLength()):
        assert node.Outputs(i) == output[i]
    
    input = [0, 1, 2]
    for i in range(node.InputsLength()):
        assert node.Inputs(i) == input[i]
    
    # tensor
    for i in range(graph.TensorsLength()):
        tensor = graph.Tensors(i)
        # use xxxAsNumpy, you should pip install numpy
        shape = tensor.ShapeAsNumpy()
        # print(shape)
        # assert (shape == [100, 3, 224, 224]).all()
        data_numpy = tensor.DataAsNumpy()
        # if isinstance(data_numpy, int) and data_numpy == 0:
        #     print("No data available.")
        # else:
        #     data_bytes = data_numpy.tobytes()
        #     data = np.frombuffer(data_bytes, dtype=np.float32)
        #     print(data[0])
        # assert data == 0

if __name__ == '__main__':
    my_convertor = convertor()
    my_convertor.load_onnx_model()
    my_convertor.build_mininn()
    check_mininn()
