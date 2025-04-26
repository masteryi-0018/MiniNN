import onnxruntime as ort
import numpy as np

session = ort.InferenceSession('add_model.onnx')

input_shape = (100, 3, 224, 224)
input0 = np.full(input_shape, 1.0, dtype=np.float32)
input1 = np.full(input_shape, 2.0, dtype=np.float32)

outputs = session.run(
    None,
    {'input0': input0, 'input1': input1}
)

print("输出结果:", outputs[0][0][0][0][0], outputs[0].shape)