from common import convert_model, test_model

import onnx
from onnx import helper, TensorProto
import numpy as np


def make_model(input_shape=[1, 3, 224, 224],
               indices_shape=[1],
               output_shape=[1, 3, 224, 224],
               axis=0
               ):
    input0 = helper.make_tensor_value_info('input0', TensorProto.FLOAT, input_shape)

    indices_value = np.random.randint(0, input_shape[axis], size=[1]).astype(np.int64)
    # debug
    # print(f"indices_value: {indices_value}")
    input1 = helper.make_tensor(
        name='input1',
        data_type=TensorProto.INT64,
        dims=[],
        vals=indices_value)

    output0 = helper.make_tensor_value_info('output0', TensorProto.FLOAT, [])

    gather_node = helper.make_node(
        'Gather',
        inputs=['input0', 'input1'],
        outputs=['output0'],
        axis=axis,
        name='gather_node'
    )

    graph = helper.make_graph(
        [gather_node],
        'gather_model',
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

    model_path = 'models/gather_model.onnx'
    onnx.save(model, model_path)
    print(f"模型已保存为 {model_path}")

    return model_path


if __name__ == "__main__":

    axis = 0
    input_shape = [4]
    indices_shape = [1]
    output_shape = input_shape[:axis] + indices_shape + input_shape[axis+1:]
    # output_shape = [1]

    model_path = make_model(input_shape=input_shape,
                            indices_shape=indices_shape,
                            output_shape=output_shape,
                            axis=axis
                            )

    new_model_path = convert_model(model_path)

    test_model(new_model_path, model_path, True)