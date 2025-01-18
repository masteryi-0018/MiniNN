import mininn

def main():
    # Tensor
    tensor = mininn.Tensor()
    tensor.set_shape([1, 3, 224, 224])
    print(tensor.get_shape())
    print(tensor.get_size())
    print(tensor.get_buffer())
    print(tensor.get_length())

    # parser
    filename = "../../convertor/mininn_test.gynn"
    graph = mininn.Graph()
    mininn.load_model(filename, graph)

    # graph
    node_num = len(graph.get_nodes())
    print(f"graph has {node_num} nodes") # py3.6+
    tensors = graph.get_tensors()
    for i in range(len(tensors)):
        print("tensor {} shape is {}".format(i, tensors[i].get_shape()))  # py2.7+ / py3.x
    print(graph.get_inputs())
    print(graph.get_outputs())
    return

if __name__ == "__main__":
    main()