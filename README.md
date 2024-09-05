# MiniNN

Build a deep learning inference framework from scratch

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
.\build.ps1
```

2. bazel

```ps1
.\bazel.ps1
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

## quick start

### windows

1. cmake

```ps1
.\build\mininn\test-main.exe
.\build\mininn\test\gtest-main.exe
```

2. bazel

```ps1
.\bazel-bin\mininn\test-main.exe
.\bazel-bin\mininn\gtest-main.exe
```

### linux

1. cmake

```sh
./build/mininn/test-main
./build/mininn/test/gtest-main
```

2. bazel

```sh
./bazel-bin/mininn/test-main
./bazel-bin/mininn/gtest-main
```

## TODO

1. mininn convertor
- [ ] 支持将onnx模型转换为gynn格式
- [ ] 支持将pytorch模型转换为gynn格式
- [ ] 支持将tensorflow模型转换为gynn格式

2. mininn IR
- [ ] 支持多算子构图

3. mininn kernel
- [ ] 增加opencl后端
- [ ] 增加cuda后端

4. 工具类
- [ ] 增加示例程序，链接mininn.so

5. 构建系统
- [x] 引入flatbuffers头文件，集成到bazel脚本中
- [x] Windows使用bazel构建
- [x] Windows使用msvc编译问题解决
- [ ] 使用clang编译器

6. 疑问
- [ ] 堆上申请内存的示意图