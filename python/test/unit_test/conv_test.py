from mininn.convertor import Convertor
from mininn.predictor import Predictor

import onnx
from onnx import helper, TensorProto
import onnxruntime as ort
import numpy as np


def make_model(input_shape=[1, 3, 224, 224],
               weight_shape=[64, 3, 3, 3],
               bias_shape=[64,],
               output_shape=[1, 64, 224, 224],
               dilations=[1, 1],
               group=1,
               kernel_shape=[3, 3],
               pads=[1, 1, 1, 1],
               strides=[1, 1]
               ):
    input0 = helper.make_tensor_value_info('input0', TensorProto.FLOAT, input_shape)

    weight_value = np.random.rand(*weight_shape).astype(np.float32)
    input1 = helper.make_tensor(
        name='input1',
        data_type=TensorProto.FLOAT,
        dims=weight_shape,
        vals=weight_value)

    bias_value = np.random.rand(*bias_shape).astype(np.float32)
    input2 = helper.make_tensor(
        name='input2',
        data_type=TensorProto.FLOAT,
        dims=bias_shape,
        vals=bias_value)

    output0 = helper.make_tensor_value_info('output0', TensorProto.FLOAT, output_shape)

    conv_node = helper.make_node(
        'Conv',
        inputs=['input0', 'input1', 'input2'],
        outputs=['output0'],
        dilations=dilations,
        group=group,
        kernel_shape=kernel_shape,
        pads=pads,
        strides=strides,
        name='conv_node'
    )

    graph = helper.make_graph(
        [conv_node],
        'conv_model',
        [input0],
        [output0],
        initializer=[input1, input2]
    )

    model = helper.make_model(
        graph,
        producer_name='onnx-mininn',
        opset_imports=[helper.make_opsetid("", 22)], # means the imports
        ir_version = 10 # means the format
    )

    onnx.checker.check_model(model)

    model_path = 'models/conv_model.onnx'
    onnx.save(model, model_path)
    print(f"模型已保存为 {model_path}")

    return model_path

def convert_model(model_path):
    new_model_path = model_path.replace(".onnx", ".gynn")
    my_convertor = Convertor()
    my_convertor.load_onnx_model(model_path)
    my_convertor.build_mininn(new_model_path)
    return new_model_path

def gen_all_golden(model_path, input):
    # add all outputs to the graph
    modified_model_path = model_path.replace(".onnx", "_all_outputs.onnx")

    model = onnx.load(model_path)
    for node in model.graph.node:
        for output in node.output:
            if not any(o.name == output for o in model.graph.output):
                model.graph.output.append(helper.make_tensor_value_info(output, TensorProto.FLOAT, None))

    onnx.save(model, modified_model_path)

    options = ort.SessionOptions()
    options.graph_optimization_level = ort.GraphOptimizationLevel.ORT_DISABLE_ALL

    session = ort.InferenceSession(model_path, options, providers=['CPUExecutionProvider'])
    input0_shape = session.get_inputs()[0].shape
    input = input.reshape(input0_shape)

    outputs = session.run(
        None,
        {'input0': input}
    )

    return outputs

def gen_golden(model_path, input):
    session = ort.InferenceSession(model_path)

    input0_shape = session.get_inputs()[0].shape
    input = input.reshape(input0_shape)

    outputs = session.run(
        None,
        {'input0': input}
    )
    return outputs

def l2_norm(a, b):
    arr_a = np.array(a, dtype=np.float32)
    arr_b = np.array(b, dtype=np.float32)
    arr_b = arr_b.reshape(arr_a.shape)
    return np.linalg.norm(arr_a - arr_b)

def test_model(new_model_path, model_path, debug=True):
    my_predictor= Predictor(new_model_path)

    inputs = my_predictor.get_input()
    input0_size = inputs[0].get_size()
    input = np.random.rand(input0_size).astype(np.float32)

    my_predictor.set_data([input])
    my_predictor.run()

    if debug:
        all_golden = gen_all_golden(model_path, input)
        all_outputs = my_predictor.dump_all_outputs()
        for golden, output in zip(all_golden, all_outputs):
            print(l2_norm(output.get_data(), golden))
    else:
        goldens = gen_golden(model_path, input)
        golden = goldens[0]
        outputs = my_predictor.get_output()
        output = outputs[0]
        # print(output.get_data()[0], output.get_shape())
        print(l2_norm(output.get_data(), golden))
    return


if __name__ == "__main__":

    batch_size = 1
    hw = 224
    ic = 3
    oc = 64
    dilations=[1, 1]
    group=1
    k_size = 3
    pads = [1, 1, 1, 1]
    strides = [1, 1]

    model_path = make_model(input_shape=(1, ic, hw, hw),
                            weight_shape=(oc, ic, k_size, k_size),
                            bias_shape=(oc,),
                            output_shape=(1, oc, hw, hw),
                            dilations=dilations,
                            group=group,
                            kernel_shape=(k_size, k_size),
                            pads=pads,
                            strides=strides
                            )

    new_model_path = convert_model(model_path)

    test_model(new_model_path, model_path, False)