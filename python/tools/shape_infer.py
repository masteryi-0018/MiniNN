import onnx

def shape_infer(model_path, new_model_path):
        onnx_model = onnx.load(model_path)
        onnx_model_shape = onnx.shape_inference.infer_shapes(onnx_model)
        onnx.save(onnx_model_shape, new_model_path)
        print(f"新模型已保存为: {new_model_path}")

if __name__ == "__main__":
    model_path = "../../models/mobilenetv2-10.onnx"
    new_model_path = model_path.replace('.onnx', '-shape.onnx')
    shape_infer(model_path, new_model_path)