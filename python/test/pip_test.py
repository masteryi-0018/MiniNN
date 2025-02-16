import mininn

def main():
    filename = "../../convertor/onnx/onnx_add_100_3_224_224.gynn"
    my_predictor= mininn.Predictor(filename)

    inputs = my_predictor.get_input()
    input1_size = inputs[0].get_size()
    input2_size = inputs[1].get_size()
    a = [1.0] * input1_size
    b = [2.0] * input2_size

    my_predictor.set_data(a, b)
    my_predictor.run()
    
    outputs = my_predictor.get_output()
    c = outputs[0]
    print(c.get_data()[0], c.get_shape())

    output_size = c.get_size()
    golden = [3.0] * output_size
    assert c.get_data() == golden
    return


if __name__ == "__main__":
    main()
