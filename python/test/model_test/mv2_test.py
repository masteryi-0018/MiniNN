from mininn.convertor import Convertor
from mininn.predictor import Predictor

import onnx
from onnx import helper, TensorProto
import onnxruntime as ort
import numpy as np


def get_dtype(tensor_name, model):
    for value_info in model.graph.value_info:
        if value_info.name == tensor_name:
            return value_info.type.tensor_type.elem_type


def gen_all_golden(model_path, input):
    # add all outputs to the graph
    modified_model_path = model_path.replace(".onnx", "_all_outputs.onnx")

    model = onnx.load(model_path)
    # only shape infer and get value_info
    onnx_model_shape = onnx.shape_inference.infer_shapes(model)

    for node in onnx_model_shape.graph.node:
        for output in node.output:
            if not any(o.name == output for o in model.graph.output):
                if get_dtype(output, onnx_model_shape) == TensorProto.FLOAT:
                    onnx_model_shape.graph.output.append(
                        helper.make_tensor_value_info(output, TensorProto.FLOAT, None)
                    )
                elif get_dtype(output, onnx_model_shape) == TensorProto.INT64:
                    onnx_model_shape.graph.output.append(
                        helper.make_tensor_value_info(output, TensorProto.INT64, None)
                    )

    onnx.save(onnx_model_shape, modified_model_path)

    options = ort.SessionOptions()
    options.graph_optimization_level = ort.GraphOptimizationLevel.ORT_DISABLE_ALL

    session = ort.InferenceSession(
        modified_model_path, options, providers=["CPUExecutionProvider"]
    )
    input0_shape = session.get_inputs()[0].shape
    input0_shape[0] = 1  # batch size
    input = input.reshape(input0_shape)

    outputs = session.run(None, {"input": input})
    return outputs


def gen_golden(model_path, input):
    session = ort.InferenceSession(model_path)

    input0_shape = session.get_inputs()[0].shape
    input0_shape[0] = 1  # batch size
    input = input.reshape(input0_shape)

    outputs = session.run(None, {"input": input})
    return outputs


def l2_norm(a, b):
    arr_a = np.array(a, dtype=np.float32)
    arr_b = np.array(b, dtype=np.float32)
    arr_b = arr_b.reshape(arr_a.shape)
    return np.linalg.norm(arr_a - arr_b)


def test_model(new_model_path, model_path, debug=True):
    my_predictor = Predictor(new_model_path)

    inputs = my_predictor.get_input()
    input0_size = inputs[0].get_size()
    input = np.random.rand(input0_size).astype(np.float32)

    my_predictor.set_data([input])
    my_predictor.run()

    if debug:
        all_golden = gen_all_golden(model_path, input)
        first_node = all_golden.pop(0)
        all_golden.append(first_node)
        all_outputs = my_predictor.dump_all_outputs()
        layer = 0
        for golden, output in zip(all_golden, all_outputs):
            # print("Golden shape:", np.array(golden).shape)
            # print("Output shape:", output.get_shape())
            print(f"Layer {layer} L2 norm: {l2_norm(output.get_data(), golden)}")
            layer += 1
            # if l2_norm(output.get_data(), golden) == 0:
            #     print("output value" , output.get_data())
            #     print("golden value" , golden)
    else:
        goldens = gen_golden(model_path, input)
        golden = goldens[0]
        outputs = my_predictor.get_output()
        output = outputs[0]
        print(l2_norm(output.get_data(), golden))
    return


if __name__ == "__main__":
    model_path = "models/mobilenetv2-10.onnx"
    new_model_path = "models/mobilenetv2-10.gynn"

    test_model(new_model_path, model_path, True)
