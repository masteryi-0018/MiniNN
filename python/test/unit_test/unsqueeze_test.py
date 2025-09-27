import numpy as np
import onnx
from common import convert_model, test_model
from onnx import TensorProto, helper


def make_model(input_shape=[1, 3, 224, 224], output_shape=[1, 3, 224, 224], axes=0):
    input0 = helper.make_tensor_value_info("input0", TensorProto.FLOAT, [])

    output0 = helper.make_tensor_value_info("output0", TensorProto.FLOAT, [1])

    unsqueeze_node = helper.make_node(
        "Unsqueeze",
        inputs=["input0"],
        outputs=["output0"],
        axes=axes,
        name="unsqueeze_node",
    )

    graph = helper.make_graph([unsqueeze_node], "unsqueeze_model", [input0], [output0])

    model = helper.make_model(
        graph,
        producer_name="onnx-mininn",
        opset_imports=[helper.make_opsetid("", 10)],  # means the imports
        ir_version=6,  # means the format
    )

    onnx.checker.check_model(model)

    model_path = "models/unsqueeze_model.onnx"
    onnx.save(model, model_path)
    print(f"模型已保存为 {model_path}")

    return model_path


if __name__ == "__main__":

    axes = [0]
    input_shape = (3, 224, 224)
    output_shape = (1, 3, 224, 224)

    model_path = make_model(
        input_shape=input_shape, output_shape=output_shape, axes=axes
    )

    new_model_path = convert_model(model_path)

    test_model(new_model_path, model_path, True)
