import os
import sys

sys.path.append(os.path.join(os.path.dirname(__file__), '../third_party/flatbuffers/python'))
import flatbuffers

sys.path.append(os.path.join(os.path.dirname(__file__), '../'))
import mininn_fbs.Op
import mininn_fbs.Attribute
import mininn_fbs.Node
import mininn_fbs.Tensor
import mininn_fbs.Graph

import onnx
import numpy as np
from onnx import AttributeProto


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

OP_map = {
    "Add": mininn_fbs.Op.Op().ADD,
    "Conv": mininn_fbs.Op.Op().CONV,
    "Clip": mininn_fbs.Op.Op().CLIP,
    "Shape": mininn_fbs.Op.Op().SHAPE,
    "Gather": mininn_fbs.Op.Op().GATHER,
    "Unsqueeze": mininn_fbs.Op.Op().UNSQUEEZE,
    "Concat": mininn_fbs.Op.Op().CONCAT,
    "GlobalAveragePool": mininn_fbs.Op.Op().GLOBALAVERAGEPOOL,
    "Reshape": mininn_fbs.Op.Op().RESHAPE,
    "Gemm": mininn_fbs.Op.Op().GEMM,
    "Constant": mininn_fbs.Op.Op().CONSTANT,
}

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
        self.name = None


    def load_onnx_model(self):
        model_path = "/home/gy/proj/MiniNN/mobilenetv2-10.onnx"
        self.name = model_path.split("/")[-1].split(".")[0]
        onnx_model = onnx.load(model_path)
        onnx_model_shape = onnx.shape_inference.infer_shapes(onnx_model)
        onnx_graph = onnx_model_shape.graph
        print_graph_info(onnx_graph)
        self.onnx_graph = onnx_graph


    def build_mininn(self):
        self.build_node_and_tensor()
        graph = self.build_graph()
        
        self.builder.Finish(graph)
        new_name = self.name + ".gynn"
        with open(new_name, "wb") as f:
            f.write(self.builder.Output())
        print(f"{self.name} 已转换为 {new_name} \n")


    def build_node_and_tensor(self):
        for onnx_node in self.onnx_graph.node:
            # print_node_info(onnx_node)
            inputs = self.build_tensor(onnx_node.input)
            outputs = self.build_tensor(onnx_node.output)
            # print(f"node {onnx_node.name} inputs is: {onnx_node.input}")
            # print(f"node {onnx_node.name} outputs is: {onnx_node.output}")

            node_inputs = self.builder.CreateNumpyVector(np.array(inputs, dtype=np.int32))
            node_outputs = self.builder.CreateNumpyVector(np.array(outputs, dtype=np.int32))

            attributes_vector = []
            for attribute in onnx_node.attribute:
                key = self.builder.CreateString(attribute.name)
                if attribute.type == AttributeProto.INTS:
                    value = attribute.ints
                    value_array = self.builder.CreateNumpyVector(np.array(value, dtype=np.int32))
                elif attribute.type == AttributeProto.INT:
                    value = attribute.i
                    value_array = self.builder.CreateNumpyVector(np.array(value, dtype=np.int32))
                elif attribute.type == AttributeProto.FLOAT:
                    value = (int)(attribute.f) # todo
                    value_array = self.builder.CreateNumpyVector(np.array(value, dtype=np.int32))

                mininn_fbs.Attribute.AttributeStart(self.builder)
                mininn_fbs.Attribute.AttributeAddKey(self.builder, key)
                mininn_fbs.Attribute.AttributeAddValue(self.builder, value_array)
                attr = mininn_fbs.Attribute.AttributeEnd(self.builder)
                attributes_vector.append(attr)
            
            mininn_fbs.Node.NodeStartAttributesVector(self.builder, len(attributes_vector))
            for attribute in reversed(attributes_vector):
                self.builder.PrependUOffsetTRelative(attribute)
            attributes = self.builder.EndVector()

            mininn_fbs.Node.NodeStart(self.builder)
            mininn_fbs.Node.NodeAddType(self.builder, OP_map[onnx_node.op_type])
            mininn_fbs.Node.NodeAddAttributes(self.builder, attributes)
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
                    # if i.name in ['630']:
                    #     print_tensor_info(i)
                    # print("i from initializer")
                    if i.data_type == 7: # int64
                        shape_list = i.dims
                        shape = self.builder.CreateNumpyVector(np.array(shape_list, dtype=np.int32))
                        raw_data = i.raw_data
                        int64_value = np.frombuffer(raw_data, dtype=np.int64)
                        float32_value = np.float32(int64_value)
                        float32_bytes = float32_value.tobytes()
                        data = self.builder.CreateNumpyVector(np.frombuffer(float32_bytes, dtype=np.uint8))
                        self.add_tensor(shape, data, idx_list, onnx_tensor)
                    else: # float32
                        shape_list = i.dims
                        shape = self.builder.CreateNumpyVector(np.array(shape_list, dtype=np.int32))
                        raw_data = i.raw_data
                        data = self.builder.CreateNumpyVector(np.frombuffer(raw_data, dtype=np.uint8))
                        self.add_tensor(shape, data, idx_list, onnx_tensor)

            for v in self.onnx_graph.value_info:
                if v.name == onnx_tensor:
                    # if v.name in ['464', '471', '472']:
                    #     print_value_info(v)
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
    read("mobilenetv2-10.gynn")


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
    node = graph.Nodes(99)
    # assert node.Type() == mininn_fbs.Op.Op.CONV
    for i in range(node.AttributesLength()):
        print(node.Attributes(i).Key())
        print(node.Attributes(i).ValueAsNumpy())

    output = [3]
    for i in range(node.OutputsLength()):
        # assert node.Outputs(i) == output[i]
        print("Outputs:")
        print(node.Outputs(i))
    
    input = [0, 1, 2]
    for i in range(node.InputsLength()):
        # assert node.Inputs(i) == input[i]
        print("Inputs:")
        print(node.Inputs(i))
    
    print("\n")
    # debug
    for i in [99, 100, 101, 102, 103]:
        node = graph.Nodes(i)
        # for i in range(node.AttributesLength()):
        #     print(node.Attributes(i).Key())
        #     print(node.Attributes(i).ValueAsNumpy())
        print("node: ", i)
        for i in range(node.OutputsLength()):
            print("Outputs:")
            print(node.Outputs(i))
        
        for i in range(node.InputsLength()):
            print("Inputs:")
            print(node.Inputs(i))
        print("\n")

    # tensor
    tensor = graph.Tensors(204)
    # use xxxAsNumpy, you should pip install numpy
    shape = tensor.ShapeAsNumpy()
    # assert (shape == [1, 3, 224, 224]).all()
    print(shape)
    for i in range(graph.TensorsLength()):
        t = graph.Tensors(i)
        # 204, 205, 209
        if len(t.ShapeAsNumpy()) == 0:
            print(i)
    
    data_numpy = tensor.DataAsNumpy()
    if isinstance(data_numpy, int) and data_numpy == 0:
        print("No data available.")
        assert data_numpy == 0
    else:
        data_bytes = data_numpy.tobytes()
        data = np.frombuffer(data_bytes, dtype=np.float32)
        print(data[0])


if __name__ == '__main__':
    my_convertor = convertor()
    my_convertor.load_onnx_model()
    my_convertor.build_mininn()
    check_mininn()