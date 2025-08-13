from common import convert_model, test_model

import onnx
from onnx import helper, TensorProto
import numpy as np


def make_model(input_shape=[1, 3, 224, 224],
               output_shape=[1, 64, 224, 224],
               min=0,
               max=6
               ):
    input0 = helper.make_tensor_value_info('input0', TensorProto.FLOAT, input_shape)

    output0 = helper.make_tensor_value_info('output0', TensorProto.FLOAT, output_shape)

    clip_node = helper.make_node(
        'Clip',
        inputs=['input0'],
        outputs=['output0'],
        min=min,
        max=max,
        name='clip_node'
    )

    graph = helper.make_graph(
        [clip_node],
        'clip_model',
        [input0],
        [output0]
    )

    model = helper.make_model(
        graph,
        producer_name='onnx-mininn',
        opset_imports=[helper.make_opsetid("", 6)], # means the imports
        ir_version = 10 # means the format
    )

    onnx.checker.check_model(model)

    model_path = 'models/clip_model.onnx'
    onnx.save(model, model_path)
    print(f"模型已保存为 {model_path}")

    return model_path


if __name__ == "__main__":

    batch_size = 1
    channel = 3
    height = 224
    width = 224
    input_shape = (batch_size, channel, height, width)
    output_shape = (batch_size, channel, height, width)
    min = 0.0
    max = 6.0

    model_path = make_model(input_shape=input_shape,
                            output_shape=output_shape,
                            min=min,
                            max=max
                            )

    new_model_path = convert_model(model_path)

    test_model(new_model_path, model_path, True)