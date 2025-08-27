from common import convert_model, test_model

import onnx
from onnx import helper, TensorProto
import numpy as np


def make_model(input_shape=[1, 1280, 1, 1],
               output_shape=[1, 1280],
               shape=[1, 1280]
               ):
    input0 = helper.make_tensor_value_info('input0', TensorProto.FLOAT, input_shape)

    shape_value = np.array([1, 1280]).astype(np.int64)
    input1 = helper.make_tensor(
        name='input1',
        data_type=TensorProto.INT64,
        dims=shape,
        vals=shape_value)

    output0 = helper.make_tensor_value_info('output0', TensorProto.FLOAT, output_shape)

    reshape_node = helper.make_node(
        'Reshape',
        inputs=['input0', 'input1'],
        outputs=['output0'],
        name='reshape_node'
    )

    graph = helper.make_graph(
        [reshape_node],
        'reshape_model',
        [input0],
        [output0],
        initializer=[input1]
    )

    model = helper.make_model(
        graph,
        producer_name='onnx-mininn',
        opset_imports=[helper.make_opsetid("", 10)], # means the imports
        ir_version = 6 # means the format
    )

    onnx.checker.check_model(model)

    model_path = 'models/reshape_model.onnx'
    onnx.save(model, model_path)
    print(f"模型已保存为 {model_path}")

    return model_path


if __name__ == "__main__":

    input_shape = (1, 1280, 1, 1)
    shape = (2,)
    output_shape = (1, 1280)

    model_path = make_model(input_shape=input_shape,
                            output_shape=output_shape,
                            shape=shape
                            )

    new_model_path = convert_model(model_path)

    test_model(new_model_path, model_path, True)