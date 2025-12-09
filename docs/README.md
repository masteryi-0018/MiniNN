# 开发前

## 定义

在开发前，我认知中的端侧深度学习推理框架的定义：

1. 推理框架和训练框架的区别在于是否有反向传播，如果不进行梯度更新，那么就是推理框架
2. 端侧主要是指移动端设备，与桌面端的区别在于计算条件限制多，例如算力小，内存少，功耗低

## 基本能力

那么一个端侧深度学习推理框架应该具备的最基本的能力应该是：

1. convertor 部分，既然不自己训练模型，那么就需要将其他训练框架训练得到的模型（pytorch，tensorflow），或者业界通用的一些模型（onnx）转换为自己可以处理的格式（本框架的 IR，intermediate representation），这就是一个框架的第一个功能：convertor。这个接口一般是 python，需要用到 pytorch，tensorflow，onnx 的 py 库，解析模型，转换模型，并保存为自己的文件格式。
2. 核心的 IR 部分，需要涉及类与结构体，能够表示神经网络的图结构（op、tensor，以及拓扑关系）。这时候得到的模型是一个文件，需要解析成为 C++的 IR，使用 C++语言表示的模型（图），这个功能一般叫做模型解析器（model parser），这一步就是使用 C++将文件变为待计算的图表示。
3. runtime 部分，这是框架的承上启下的部分，运用多后端的算力，整体调度计算资源，安排一次运行。
4. kernel 部分，决定跑在什么后端，每个 op 都会对应具体的 kernel，描述这个算子具体的计算行为。

以上，理论上就可以将一个 onnx conv2d 单算子网络跑出一个结果。

## 拓展能力

进一步思考，一个框架逐渐发现，还应该具备的能力有：

1. 图优化——框架的核心竞争力。原始的网络节点可能算起来比较慢，框架一般会对网络进行通用（后端无关的）优化，例如算子融合、算子拆分等，保证交给后端硬件的计算是能发挥最大性能的软件代码。
2. 下游生态对接，框架虽然有自己直接操作硬件后端（cpu，gpu，npu 等）的能力，但是某些情况下 kernel 可能并没有 SoC 的厂商实现更好，或者有一定限制，必须使用另一个更底层的框架来承接计算任务，这时候可以采用 delegate 的方式（类似于 tflite 的方式）。
3. 上游生态对接，框架可能会提供被其他框架使用的能力，这时候就不需要显式的序列化模型了，可以直接进行 IR 的 mapping，也就是图级别的接入，也可以采取 delegate 的方式（如果上层框架提供了这种能力）
4. 量化，这也是框架重要的能力之一。因为端侧设备的内存限制和算力限制，以及低时延高速度的要求，float32（fp32）的模型往往不能满足要求，基本都会走一下量化的步骤，得到 int8 的模型。再加上目前大模型的出现，int4 的量化重要性逐渐增加。
5. 异构。如何发挥各个后端的性能，统一调配计算资源，发挥 SoC 的最大性能。
6. 几个难点：动态形状，控制流算子等

![MiniNN](../assets/mininn.svg)

# 开发中

> 本项目作者受 tensorflow lite，paddle-lite，Android NNAPI 影响较大，一些设计可能借鉴上述项目

## 构建

### cmake

| Host        | Target  | Generator             | gcc         | clang                  | msvc     |
| ----------- | ------- | --------------------- | ----------- | ---------------------- | -------- |
| **Windows** | Windows | Visual Studio 17 2022 | ❌          | ❌                     | ✅       |
|             |         | Ninja                 | ✅          | ✅                     | ⚠️       |
|             |         | MinGW Makefiles       | ✅          | ✅                     | ⚠️       |
|             |         | NMake Makefiles       | vs shell    | vs shell               | vs shell |
|             |         | Unix Makefiles        | rename      | rename                 | rename   |
|             |         | MSYS Makefiles        | msys2 shell | mingw-w64-x86_64-clang | ⚠️       |
|             | Android | Ninja                 | ❌          | ✅                     | ❌       |
|             |         | Unix Makefiles        | ❌          | ✅                     | ❌       |
| **Linux**   | Linux   | Unix Makefiles        | ✅          | ✅                     | ❌       |
|             |         | Ninja                 | ✅          | ✅                     | ❌       |
|             | Android | Ninja                 | ❌          | ✅                     | ❌       |
|             |         | Unix Makefiles        | ❌          | ✅                     | ❌       |

1. 使用 Unix Makefile 需要 rename mingw32-make -> make，不推荐
2. 使用 MSYS Makefiles 需要特定的 shell，否则找不到默认的使用 sh.exe，不推荐，而且不容易配置 clang 和 cl
3. 使用 NMake Makefiles 需要特定的 shell，不推荐
4. 通过执行 vcvarsall.bat 脚本，可以获得 vs shell 的环境
5. 获得环境之后依旧不推荐使用不匹配的工具链进行开发，容易出问题
6. 不推荐手动配置 cl 的环境，windows SDK（mt.exe）的环境，甚至 windows SDK（kernel32.lib）的环境

```ps1
cmd /c "`"E:\visual studio\VC\Auxiliary\Build\vcvarsall.bat`" x64 && pwsh"
```

### bazel

自己配置 cpp 工具链比较负责，使用默认的测试

| Host Platform | Build Tool | gcc | clang | msvc |
| ------------- | ---------- | --- | ----- | ---- |
| **Windows**   | bazel      | ❌  | ❌    | ✅   |
| **Linux**     | bazel      | ✅  | ❌    | ❌   |

### 命令

1. windows local

```ps1
# cmake ninja
python .\build_mininn.py --target windows --tool cmake --generator ninja --compiler clang
python .\build_mininn.py --target windows --tool cmake --generator ninja --compiler gcc
python .\build_mininn.py --target windows --tool cmake --generator ninja --compiler cl

# cmake vs2022
python .\build_mininn.py --target windows --tool cmake --generator vs2022 --compiler cl

# cmake mingw（后续移除）
python .\build_mininn.py --target windows --tool cmake --generator mingw --compiler clang
python .\build_mininn.py --target windows --tool cmake --generator mingw --compiler gcc
python .\build_mininn.py --target windows --tool cmake --generator mingw --compiler cl

# cmake nmake（后续移除）
python .\build_mininn.py --target windows --tool cmake --generator nmake --compiler clang
python .\build_mininn.py --target windows --tool cmake --generator nmake --compiler gcc
python .\build_mininn.py --target windows --tool cmake --generator nmake --compiler cl

# cmake make（后续移除）
python .\build_mininn.py --target windows --tool cmake --generator make --compiler clang
python .\build_mininn.py --target windows --tool cmake --generator make --compiler gcc
python .\build_mininn.py --target windows --tool cmake --generator make --compiler cl

# cmake msys2（后续移除）这里需要clang-gnu版本
python ./build_mininn.py --target windows --tool cmake --generator msys2 --compiler clang
python ./build_mininn.py --target windows --tool cmake --generator msys2 --compiler gcc

# bazel msvc bzlmod
python .\build_mininn.py --target windows --tool bazel
```

2. windows android

```ps1
# cmake ninja
python .\build_mininn.py --target android --tool cmake --generator ninja --compiler clang

# cmake make
python .\build_mininn.py --target android --tool cmake --generator make --compiler clang
```

3. linux local

```sh
# cmake ninja
python ./build_mininn.py --target linux --tool cmake --generator ninja --compiler clang
python ./build_mininn.py --target linux --tool cmake --generator ninja --compiler gcc

# cmake make
python ./build_mininn.py --target linux --tool cmake --generator make --compiler clang
python ./build_mininn.py --target linux --tool cmake --generator make --compiler gcc

# bazel gcc bzlmod
python ./build_mininn.py --target linux --tool bazel
```

4. linux android

```sh
# cmake ninja
python ./build_mininn.py --target android --tool cmake --generator ninja --compiler clang

# cmake make
python ./build_mininn.py --target android --tool cmake --generator make --compiler clang
```

## 第三方依赖

| third_party    | version  |
| -------------- | -------- |
| flatbuffers    | v25.9.23 |
| googletest     | v1.17.0  |
| pybind11       | v3.0.1   |
| pybind11_bazel | v3.0.0   |
| rules_cuda     | v0.2.5   |
| rules_python   | 1.6.2    |
