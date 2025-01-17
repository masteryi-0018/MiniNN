import mininn

def main():
    tensor = mininn.Tensor()
    tensor.set_shape([1, 3, 224, 224])
    print(tensor.get_shape())
    print(tensor.get_size())
    print(tensor.get_buffer())
    print(tensor.get_length())
    return

if __name__ == "__main__":
    main()
