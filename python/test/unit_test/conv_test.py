import onnx
from onnx import helper, TensorProto

from mininn.convertor import Convertor

def make_model():
    input0 = helper.make_tensor_value_info('input0', TensorProto.FLOAT, [100, 3, 224, 224])

    input1 = helper.make_tensor(
        name='input1',
        data_type=TensorProto.FLOAT,
        dims=[64, 3, 3, 3],
        vals=(1.0,) * (64 * 3 * 3 * 3))

    input2 = helper.make_tensor(
        name='input2',
        data_type=TensorProto.FLOAT,
        dims=[64],
        vals=(2.0,) * 64)

    output0 = helper.make_tensor_value_info('output0', TensorProto.FLOAT, [100, 64, 224, 224])

    conv_node = helper.make_node(
        'Conv',
        inputs=['input0', 'input1', 'input2'],
        outputs=['output0'],
        kernel_shape=[3, 3],
        pads=[1, 1, 1, 1],
        strides=[1, 1],
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
        opset_imports=[helper.make_opsetid("", 21)]
    )

    onnx.checker.check_model(model)

    model_path = 'models/conv_model.onnx'
    onnx.save(model, model_path)
    print(f"模型已保存为 {model_path}")

    return model_path


if __name__ == "__main__":
    make_model()
    print("模型生成完成")
    model_path = "models/conv_model.onnx"
    new_model_path = model_path.replace(".onnx", ".gynn")
    my_convertor = Convertor()
    my_convertor.load_onnx_model(model_path)
    my_convertor.build_mininn(new_model_path)