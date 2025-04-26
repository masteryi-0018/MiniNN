import onnx
import onnxruntime as ort
import numpy as np

def add_node_outputs_as_graph_outputs(model_path, output_node_names, new_model_path):
    model = onnx.load(model_path)
    onnx.checker.check_model(model)
    graph = model.graph
    new_outputs = []

    for node in graph.node:
        if node.name in output_node_names:
            for output in node.output:
                value_info = None
                for vi in graph.value_info:
                    if vi.name == output:
                        value_info = vi
                        break

                if value_info is None:
                    print(f"{node.name}'s output {output} is graph's input/output")
                    for inp in graph.input:
                        if inp.name == output:
                            value_info = inp
                            break
                    for out in graph.output:
                        if out.name == output:
                            value_info = out
                            break

                if value_info is not None:
                    new_output = onnx.helper.make_tensor_value_info(
                        value_info.name,
                        value_info.type.tensor_type.elem_type,
                        [dim.dim_value for dim in value_info.type.tensor_type.shape.dim]
                    )
                    new_outputs.append(new_output)
                    print(f"add node {node.name}'s output {output} as graph's output")
                    print(f" - type: {value_info.type}")

    graph.output.extend(new_outputs)
    onnx.save(model, new_model_path)
    print(f"新模型已保存到: {new_model_path}")

def run(model_path):
    session = ort.InferenceSession(model_path)

    input_shape = (1, 3, 224, 224)
    input = np.full(input_shape, 1.0, dtype=np.float32)

    outputs = session.run(
        None,
        {'input': input}
    )

    for out in outputs:
        print(out.shape)


if __name__ == "__main__":
    input_model = "mv2_shape.onnx"
    nodes_to_output = ["Gather_100", "Unsqueeze_101"]
    output_model = "modified_model.onnx"

    add_node_outputs_as_graph_outputs(input_model, nodes_to_output, output_model)
    run(output_model)