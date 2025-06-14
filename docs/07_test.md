# 测试

> mininn test

1. 功能测试
   1. tensor，graph
   2. parser
   3. runtime
2. 算子测试
   1. add
   2. mv2
3. 模型说明
   1. mininn_test.gynn（fbs test）
   2. add
   3. mv2 来源
      1. mobilenetv2-7.onnx，mobilenetv2-10.onnx（onnx model：<https://github.com/onnx/models/tree/main/validated/vision/classification/mobilenet>）
      2. mobilenetv2_050_Opset18_timm（onnx model：<https://github.com/onnx/models/tree/main/Computer_Vision/mobilenetv2_050_Opset18_timm>）
      3. mobilenetv2_pt.onnx（忘记了）
