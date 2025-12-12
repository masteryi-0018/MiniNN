import numpy as np
import onnx
from common import convert_model, run_model
from onnx import TensorProto, helper


def make_model(
    input_shape=[1, 1280],
    wight_shape=[1000, 1280],
    bias_shape=[1000],
    output_shape=[1, 1000],
    alpha=1.0,
    beta=1.0,
    transA=0,
    transB=0,
):
    input0 = helper.make_tensor_value_info("input0", TensorProto.FLOAT, input_shape)

    weight_value = np.random.rand(*wight_shape).astype(np.float32)

    input1 = helper.make_tensor(
        name="input1", data_type=TensorProto.FLOAT, dims=wight_shape, vals=weight_value
    )

    bias_value = np.random.rand(*bias_shape).astype(np.float32)
    input2 = helper.make_tensor(
        name="input2", data_type=TensorProto.FLOAT, dims=bias_shape, vals=bias_value
    )

    output0 = helper.make_tensor_value_info("output0", TensorProto.FLOAT, output_shape)

    gemm_node = helper.make_node(
        "Gemm",
        inputs=["input0", "input1", "input2"],
        outputs=["output0"],
        name="gemm_node",
        alpha=alpha,
        beta=beta,
        transA=transA,
        transB=transB,
    )

    graph = helper.make_graph(
        [gemm_node], "gemm_model", [input0], [output0], initializer=[input1, input2]
    )

    model = helper.make_model(
        graph,
        producer_name="onnx-mininn",
        opset_imports=[helper.make_opsetid("", 10)],  # means the imports
        ir_version=6,  # means the format
    )

    onnx.checker.check_model(model)

    model_path = "models/gemm_model.onnx"
    onnx.save(model, model_path)
    print(f"模型已保存为 {model_path}")

    return model_path

def test_gemm():
    # test case 1
    input_shape = (1, 1280)
    wight_shape = (1000, 1280)
    bias_shape = (1000,)
    output_shape = (1, 1000)
    alpha = 1.0
    beta = 1.0
    transA = 0
    transB = 1

    model_path = make_model(
        input_shape=input_shape,
        wight_shape=wight_shape,
        bias_shape=bias_shape,
        output_shape=output_shape,
        alpha=alpha,
        beta=beta,
        transA=transA,
        transB=transB,
    )

    new_model_path = convert_model(model_path)

    run_model(new_model_path, model_path, True)

    # test case 2
    M = 512
    N = 256
    K = 64
    input_shape = (M, K)
    wight_shape = (K, N)
    bias_shape = (N,)
    output_shape = (M, N)
    alpha = 1.0
    beta = 1.0
    transA = 0
    transB = 0

    model_path = make_model(
        input_shape=input_shape,
        wight_shape=wight_shape,
        bias_shape=bias_shape,
        output_shape=output_shape,
        alpha=alpha,
        beta=beta,
        transA=transA,
        transB=transB,
    )

    new_model_path = convert_model(model_path)

    run_model(new_model_path, model_path, True)


if __name__ == "__main__":
    test_gemm()