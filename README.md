# MiniNN

Build a deep learning inference framework from scratch

## Quick Start

envirooment requirement：

- conda，python=3.13
- your onnx model like: [mobilenetv2-10.onnx](https://github.com/masteryi-0018/MiniNN/blob/main/models/mobilenetv2-10.onnx)

```sh
conda create -n mininn python=3.13
conda activate mininn
pip install mininn

# download mv2 onnx
wget https://raw.githubusercontent.com/masteryi-0018/MiniNN/main/models/mobilenetv2-10.onnx
python python/test/model_test/mv2_test.py
```

## Build from source

### Environment Version

| Host        | Target  | Tool         | Compiler     | Backend                           | Note         |
| ----------- | ------- | ------------ | ------------ | --------------------------------- | ------------ |
| **Windows** | Windows | cmake 3.26.4 | msvc 14.44   | cuda✅ opencl✅ avx✅ sse✅ mkl✅ | use VS shell |
|             |         |              | clang 20.1.5 | cuda❌ opencl✅ avx✅ sse✅ mkl✅ |              |
|             |         |              | g++ 13.1.0   | cuda❌ opencl✅ avx✅ sse✅ mkl❌ |              |
|             | Android | cmake 3.26.4 | Clang 14.0.7 | cuda❌ opencl❌ avx❌ sse❌ mkl❌ | ndk 25.2     |
| **Linux**   | Linux   | cmake 3.16.3 | clang 10.0.0 | cuda✅ opencl✅ avx✅ sse✅ mkl✅ |              |
|             |         |              | g++ 9.4.0    | cuda✅ opencl✅ avx✅ sse✅ mkl✅ |              |
|             | Android | cmake 3.16.3 | clang 9.0.9  | cuda❌ opencl❌ avx❌ sse❌ mkl❌ | ndk r21e     |

### Download

```sh
# https
git clone --recursive https://github.com/masteryi-0018/MiniNN.git

# ssh
git clone --recursive git@github.com:masteryi-0018/MiniNN.git
```

### Build

```sh
# common
python build_mininn.py --wheel
python build_mininn.py --wheel --clean

# if you want to identify generator, add this flag
python build_mininn.py --generator ninja

# if you want to identify compiler, add this flag
python build_mininn.py --compiler clang

# if you want to build all backends
python build_mininn.py
```

## Unit Test

### windows

```ps1
# cmake
.\build\mininn\test\gtest-main.exe
```

### linux

```sh
# cmake
./build/mininn/test/gtest-main
```

### python

```sh
# 1. convert onnx into gynn
python python/mininn/convertor.py

# 2. run all tests
python python/run_all_test.py
```

## demo

```sh
# win
.\build\demo\demo.exe .\models\add_model.gynn

# linux
./build/demo/demo ./models/add_model.gynn
```

## TODO

1. mininn convertor

- [x] 支持将 onnx 模型转换为 gynn 格式
- [ ] 支持将 pytorch 模型转换为 gynn 格式
- [ ] 支持将 tensorflow 模型转换为 gynn 格式

2. mininn IR

- [x] 支持多算子构图

3. mininn kernel

- [x] 增加 opencl 后端
- [x] 增加 cuda 后端
- [x] 增加 avx 后端
- [x] 增加 sse 后端
- [x] 增加 mkl 后端

4. mininn release

- [x] demo
- [x] cpp sdk
- [x] pip package

5. mininn build

- [x] cmake
- [x] clang++
- [x] g++
- [x] msvc
