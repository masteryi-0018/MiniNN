from common import convert_model, test_model

import onnx
from onnx import helper, TensorProto
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
        opset_imports=[helper.make_opsetid("", 10)], # means the imports
        ir_version = 6 # means the format
    )

    onnx.checker.check_model(model)

    model_path = 'models/conv_model.onnx'
    onnx.save(model, model_path)
    print(f"模型已保存为 {model_path}")

    return model_path


if __name__ == "__main__":

    batch_size = 1
    height = 224
    weight = 224
    input_channels = 3
    output_channels = 64

    dilations=[1, 1]
    group=1
    k_size = 3
    pads = [1, 1, 1, 1]
    strides = [1, 1]

    input_shape = (batch_size, input_channels, height, weight)
    weight_shape = (output_channels, input_channels, 3, 3)
    bias_shape = (output_channels,)
    output_shape = (batch_size, output_channels, height, weight)

    model_path = make_model(input_shape=input_shape,
                            weight_shape=weight_shape,
                            bias_shape=bias_shape,
                            output_shape=output_shape,
                            dilations=dilations,
                            group=group,
                            kernel_shape=(k_size, k_size),
                            pads=pads,
                            strides=strides
                            )

    new_model_path = convert_model(model_path)

    test_model(new_model_path, model_path, True)