# MiniNN

Build a deep learning inference framework from scratch

## Quick Start

环境需求：

- conda，python=3.11
- mininn_test.gynn [下载](https://github.com/masteryi-0018/MiniNN/releases/download/mininn-0.0.1/mininn_test.gynn)

```sh
conda create -n mininn python=3.11
conda activate mininn
pip install mininn
python python/test/pip_test.py
```

## Environment Version

| Host Platform | Target Platform | Build Tool   | Compiler     | Backend                           | Note        |
| ------------- | --------------- | ------------ | ------------ | --------------------------------- | ----------- |
| **Windows**   | Windows         | cmake 3.26.4 | msvc 14.44   | cuda✅ opencl✅ avx✅ sse✅ mkl✅ | VS shell 中 |
|               |                 |              | clang 20.1.5 | cuda❌ opencl✅ avx✅ sse✅ mkl✅ |             |
|               |                 |              | g++ 13.1.0   | cuda❌ opencl✅ avx✅ sse✅ mkl✅ |             |
|               |                 | bazel 7.3.1  | msvc 14.44   | cuda✅ opencl✅ avx✅ sse✅ mkl✅ | 默认 msvc   |
|               |                 |              | clang 20.1.5 |                                   |             |
|               |                 |              | g++ 13.1.0   |                                   |             |
| **Linux**     | Linux           | cmake 3.30.5 | clang 10.0.0 | cuda❌ opencl❌ avx❌ sse❌ mkl❌ |             |
|               |                 |              | g++ 9.4.0    | cuda❌ opencl❌ avx❌ sse❌ mkl❌ |             |
|               |                 | bazel 8.0.0  | clang 10.0.0 |                                   |             |
|               |                 |              | g++ 9.4.0    | cuda❌ opencl❌ avx❌ sse❌ mkl❌ |             |

## Download

```sh
# https
git clone --recursive https://github.com/masteryi-0018/MiniNN.git

# ssh
git clone --recursive git@github.com:masteryi-0018/MiniNN.git
```

## Build

### windows

1. cmake

```ps1
python .\build.py
```

2. bazel

```ps1
python .\build.py --tool bazel
```

### linux

1. cmake

```sh
python build.py
```

2. bazel

```sh
python build.py --tool bazel
```

## Unit Test

### windows

1. cmake

```ps1
.\build\mininn\test\gtest-main.exe
```

2. bazel

```ps1
.\bazel-bin\mininn\gtest-main.exe
```

### linux

1. cmake

```sh
./build/mininn/test/gtest-main
```

2. bazel

```sh
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
