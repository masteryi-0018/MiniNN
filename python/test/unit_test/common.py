from mininn.convertor import Convertor
from mininn.predictor import Predictor

import onnx
from onnx import helper, TensorProto
import onnxruntime as ort
import numpy as np


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

    session = ort.InferenceSession(modified_model_path, options, providers=['CPUExecutionProvider'])
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
    if model_path == "models/clip_model.onnx":
        input = np.random.uniform(low=-10, high=10, size=input0_size).astype(np.float32)

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