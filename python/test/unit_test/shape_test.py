import onnx
from common import convert_model, run_model
from onnx import TensorProto, helper


def make_model(
    input_shape=[1, 3, 224, 224],
    output_shape=[1, 3, 224, 224],
):
    input0 = helper.make_tensor_value_info("input0", TensorProto.FLOAT, input_shape)

    output0 = helper.make_tensor_value_info(
        "output0", TensorProto.INT64, [len(output_shape)]
    )

    shape_node = helper.make_node(
        "Shape", inputs=["input0"], outputs=["output0"], name="shape_node"
    )

    graph = helper.make_graph([shape_node], "shape_model", [input0], [output0])

    model = helper.make_model(
        graph,
        producer_name="onnx-mininn",
        opset_imports=[helper.make_opsetid("", 10)],  # means the imports
        ir_version=6,  # means the format
    )

    onnx.checker.check_model(model)

    model_path = "models/shape_model.onnx"
    onnx.save(model, model_path)
    print(f"模型已保存为 {model_path}")

    return model_path

def test_shape():
    batch_size = 1
    channel = 3
    height = 224
    width = 224
    input_shape = (batch_size, channel, height, width)
    output_shape = (batch_size, channel, height, width)

    model_path = make_model(input_shape=input_shape, output_shape=output_shape)

    new_model_path = convert_model(model_path)

    run_model(new_model_path, model_path, True)

if __name__ == "__main__":
    test_shape()