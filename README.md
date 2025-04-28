# MiniNN

Build a deep learning inference framework from scratch

## quick start

环境需求：
- conda，python=3.11
- mininn_test.gynn [下载](https://github.com/masteryi-0018/MiniNN/releases/download/mininn-0.0.1/mininn_test.gynn)

```sh
conda create -n mininn python=3.11
conda activate mininn
pip install mininn
python python/test/pip_test.py
```

## Build System Compatibility

| OS / Platform       | Architecture | CMake Support | Bazel Support  | Notes                     |
|---------------------|--------------|---------------|----------------|---------------------------|
| **Linux**           | x86_64       | ✅ Yes        | ✅ Yes        | Tested on Ubuntu 20.04    |
| **Windows**         | x86_64       | ⚠️ Partial    | ⚠️ Partial    | Windows 11                |
| **Android**         | ARM64        | ⚠️ Partial    | ⚠️ Partial    | NDK required              |
| **macOS**           | x86_64       | ❌ No         | ❌ No         | intel                     |
|                     | ARM64        | ❌ No         | ❌ No         | M1/M2/M3...               |
| **iOS**             | ARM64        | ❌ No         | ❌ No         |                           |

## download

```sh
# https
git clone --recursive https://github.com/masteryi-0018/MiniNN.git

# ssh
git clone --recursive git@github.com:masteryi-0018/MiniNN.git
```

## build

### windows

1. cmake

```ps1
python .\build.py
```

2. bazel

```ps1
python .\build.py --tool bazel
```

更多请查看：<https://github.com/masteryi-0018/MiniNN/blob/main/docs/README.md>

### linux

1. cmake

```sh
./build.sh
```

2. bazel

```sh
./bazel.sh
```

更多请查看：<https://github.com/masteryi-0018/MiniNN/blob/main/docs/README.md>

## unit test

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
- [x] 支持将onnx模型转换为gynn格式
- [ ] 支持将pytorch模型转换为gynn格式
- [ ] 支持将tensorflow模型转换为gynn格式

2. mininn IR
- [x] 支持多算子构图

3. mininn kernel
- [x] 增加opencl后端
- [x] 增加cuda后端
- [x] 增加avx后端
- [x] 增加sse后端
- [x] 增加mkl后端

4. mininn release
- [ ] 增加示例程序，链接mininn.so
- [ ] cpp sdk
- [x] pip package

5. mininn build
- [x] cmake
- [x] bazel
- [x] clang
- [x] gcc
- [x] msvc