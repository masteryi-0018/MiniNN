import mininn

def test_add():
    filename = "/home/gy/proj/MiniNN/convertor/onnx/add_model.gynn"
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
    filename = "/home/gy/proj/MiniNN/convertor/onnx/mobilenetv2-10.gynn"
    my_predictor= mininn.Predictor(filename)

    inputs = my_predictor.get_input()
    input1_size = inputs[0].get_size()
    a = [1.0] * input1_size

    my_predictor.set_data([a])
    my_predictor.run()
    
    outputs = my_predictor.get_output()
    c = outputs[0]
    print(c.get_data()[0], c.get_shape())

    output_size = c.get_size()
    golden = [0.0] * output_size
    assert c.get_data() == golden
    return

def main():
    test_add()
    test_mv2()


if __name__ == "__main__":
    main()
