import onnx
from onnx import helper, TensorProto

input0 = helper.make_tensor_value_info("input0", TensorProto.FLOAT, [100, 3, 224, 224])
input1 = helper.make_tensor_value_info("input1", TensorProto.FLOAT, [100, 3, 224, 224])
output0 = helper.make_tensor_value_info(
    "output0", TensorProto.FLOAT, [100, 3, 224, 224]
)

add_node = helper.make_node("Add", ["input0", "input1"], ["output0"], name="add_node")

graph = helper.make_graph([add_node], "add_model", [input0, input1], [output0])

model = helper.make_model(
    graph, producer_name="onnx-add-mininn", opset_imports=[helper.make_opsetid("", 21)]
)

onnx.checker.check_model(model)

model_path = "../../../../models/add_model.onnx"
onnx.save(model, model_path)
print(f"模型已保存为 {model_path}")
