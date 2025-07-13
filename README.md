# MiniNN

Build a deep learning inference framework from scratch

## Quick Start

envirooment requirement：

- conda，python=3.13
- mininn_test.gynn [download](https://github.com/masteryi-0018/MiniNN/releases/download/mininn-0.0.1/mininn_test.gynn)

```sh
conda create -n mininn python=3.13
conda activate mininn
pip install mininn
python python/test/pip_test.py
```

## Build from source

### Environment Version

| Host        | Target  | Tool         | Compiler     | Backend                           | Note         |
| ----------- | ------- | ------------ | ------------ | --------------------------------- | ------------ |
| **Windows** | Windows | cmake 3.26.4 | msvc 14.44   | cuda✅ opencl✅ avx✅ sse✅ mkl✅ | use VS shell |
|             |         |              | clang 20.1.5 | cuda❌ opencl✅ avx✅ sse✅ mkl✅ |              |
|             |         |              | g++ 13.1.0   | cuda❌ opencl✅ avx✅ sse✅ mkl✅ |              |
|             |         | bazel 7.3.1  | msvc 14.44   | cuda✅ opencl✅ avx✅ sse✅ mkl✅ | default msvc |
|             |         |              | clang 20.1.5 |                                   |              |
|             |         |              | g++ 13.1.0   |                                   |              |
| **Linux**   | Linux   | cmake 3.16.3 | clang 10.0.0 | cuda✅ opencl✅ avx✅ sse✅ mkl✅ |              |
|             |         |              | g++ 9.4.0    | cuda✅ opencl✅ avx✅ sse✅ mkl✅ |              |
|             |         | bazel 8.0.0  | clang 10.0.0 |                                   |              |
|             |         |              | g++ 9.4.0    | cuda✅ opencl✅ avx✅ sse✅ mkl❌ | default g++  |

### Download

```sh
# https
git clone --recursive https://github.com/masteryi-0018/MiniNN.git

# ssh
git clone --recursive git@github.com:masteryi-0018/MiniNN.git
```

### Build

```sh
# cmake
python build.py

# bazel
python build.py --tool bazel

# if you want to identify generator, add this flag
python build.py --tool bazel --generator ninja

# if you want to identify compiler, add this flag
python build.py --tool bazel --compiler clang

# if you want build python wheel, add this flag
python build.py --tool bazel --wheel
```

## Unit Test

### windows

```ps1
# cmake
.\build\mininn\test\gtest-main.exe

# bazel
.\bazel-bin\mininn\gtest-main.exe
```

### linux

```sh
# cmake
./build/mininn/test/gtest-main

# bazel
./bazel-bin/mininn/gtest-main
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

- [ ] demo
- [ ] cpp sdk
- [x] pip package

5. mininn build

- [x] cmake
- [x] bazel
- [x] clang++
- [x] g++
- [x] msvc
