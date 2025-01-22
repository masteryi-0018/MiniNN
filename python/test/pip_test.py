# import heruiqing

import mininn

def main():
    filename = "../../convertor/mininn_test.gynn"
    predictor= mininn.predictor(filename)

    inputs = predictor.get_input()
    input1_size = inputs[0].get_size()
    input2_size = inputs[1].get_size()
    a = [1.0] * input1_size
    b = [2.0] * input2_size

    predictor.set_data(a, b)
    predictor.run()
    
    outputs = predictor.get_output()
    c = outputs[0]
    print(c[0], c.get_shape())

    output_size = c.get_size()
    golden = [3.0] * output_size
    assert c == golden
    return


if __name__ == "__main__":
    main()