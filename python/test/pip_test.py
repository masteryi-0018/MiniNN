import mininn

import onnxruntime as ort
import numpy as np

def gen_golden(filename):
    session = ort.InferenceSession(filename)

    input_shape = (1, 3, 224, 224)
    input0 = np.full(input_shape, 1.0, dtype=np.float32)

    outputs = session.run(
        None,
        {'input': input0}
    )
    return outputs[0]

def l2_norm(a, b):
    arr_a = np.array(a, dtype=np.float32)
    arr_b = np.array(b, dtype=np.float32)
    return np.linalg.norm(arr_a - arr_b)

def test_add():
    filename = "../../models/add_model.gynn"
    my_predictor= mininn.Predictor(filename)

    inputs = my_predictor.get_input()
    input1_size = inputs[0].get_size()
    input2_size = inputs[1].get_size()
    a = [1.0] * input1_size
    b = [2.0] * input2_size

    my_predictor.set_data([a, b])
    my_predictor.run()

    outputs = my_predictor.get_output()
    c = outputs[0]
    print(c.get_data()[0], c.get_shape())

    output_size = c.get_size()
    golden = [3.0] * output_size
    assert c.get_data() == golden
    return

def test_mv2():
    filename = "../../models/mobilenetv2-10.gynn"
    my_predictor= mininn.Predictor(filename)

    inputs = my_predictor.get_input()
    input1_size = inputs[0].get_size()
    a = [1.0] * input1_size

    my_predictor.set_data([a])
    my_predictor.run()

    outputs = my_predictor.get_output()
    c = outputs[0]
    print(c.get_data()[0], c.get_shape())

    golden = gen_golden("../../models/mobilenetv2-10.onnx")
    print(l2_norm(c.get_data(), golden))
    return

def main():
    test_add()
    test_mv2()


if __name__ == "__main__":
    main()