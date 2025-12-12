import numpy as np
import onnx
from common import convert_model, run_model
from onnx import TensorProto, helper


def make_model(input_shape=[1, 3, 224, 224], output_shape=[1, 6, 224, 224], axis=0):
    input0 = helper.make_tensor_value_info("input0", TensorProto.FLOAT, input_shape)
    input1 = helper.make_tensor_value_info("input1", TensorProto.FLOAT, input_shape)

    output0 = helper.make_tensor_value_info("output0", TensorProto.FLOAT, output_shape)

    concat_node = helper.make_node(
        "Concat",
        inputs=["input0", "input1"],
        outputs=["output0"],
        axis=axis,
        name="concat_node",
    )

    graph = helper.make_graph(
        [concat_node], "concat_model", [input0, input1], [output0]
    )

    model = helper.make_model(
        graph,
        producer_name="onnx-mininn",
        opset_imports=[helper.make_opsetid("", 10)],  # means the imports
        ir_version=6,  # means the format
    )

    onnx.checker.check_model(model)

    model_path = "models/concat_model.onnx"
    onnx.save(model, model_path)
    print(f"模型已保存为 {model_path}")

    return model_path

def test_concat():
    axis = 1
    input_shape = (1, 3, 224, 224)
    output_shape = (1, 6, 224, 224)

    model_path = make_model(
        input_shape=input_shape, output_shape=output_shape, axis=axis
    )

    new_model_path = convert_model(model_path)

    run_model(new_model_path, model_path, True)

if __name__ == "__main__":
    test_concat()