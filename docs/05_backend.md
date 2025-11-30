# backend

> 对于同一个计算在不同硬件上面的优化

## CUDA

下载地址：<https://developer.nvidia.com/cuda-downloads>

1. NV 显卡的驱动是共用的，也就是在 wsl 下执行`nvidia-smi`可以显示信息
2. cmake 中增加 CUDA 语言
3. 将 cu 文件编译为库，并链接
4. 暴露 cuh 头文件，被 kernel 包含

## CUDA in Windows

1. 安装 Windows 平台的 CUDA，检查环境变量是否添加，执行`nvcc -V`

Windows 下的 CUDA 编译，默认使用 MSVC 的 cl 编译器，使用`-ccbin`传递参数，必须传递路径，否则不能识别；经实验，即使传递路径，也会有报错提示：Host compiler targets unsupported OS。CUDA 的 nvcc 在 Windows 上必须配合 MSVC 工具链（cl.exe + link.exe）才能正常工作。

所以只能用 cl 编译器，那么就意味着需要在 VS powershel 中编译，即使这样，也会因为 CUDA 和 NSVC 的版本不匹配，需要添加`-allow-unsupported-compiler`来保证启用 cl 编译器，但是即使这样，开始编译后依旧会有代码中的错误检查`error: static assertion failed`。综合来看，**不建议**在 Windows 下编译 CUDA 程序。

2. 运行`nvidia-smi`后显示的 CUDA Version 是当前安装的 NVIDIA 驱动程序（Driver）所支持的最高 CUDA 工具包（Toolkit）版本。所以硬件决定能安装什么样的驱动，只要驱动可以一直升级，那么 cuda 版本就可以一直升级。

## CUDA in WSL

1. Windows 上面安装的 CUDA 在 wsl 中无法使用，需要在 NVIDIA 官网再下载一下 wsl-linux 版本：

```sh
wget https://developer.download.nvidia.com/compute/cuda/repos/wsl-ubuntu/x86_64/cuda-wsl-ubuntu.pin
sudo mv cuda-wsl-ubuntu.pin /etc/apt/preferences.d/cuda-repository-pin-600
wget https://developer.download.nvidia.com/compute/cuda/12.6.2/local_installers/cuda-repo-wsl-ubuntu-12-6-local_12.6.2-1_amd64.deb
sudo dpkg -i cuda-repo-wsl-ubuntu-12-6-local_12.6.2-1_amd64.deb
sudo cp /var/cuda-repo-wsl-ubuntu-12-6-local/cuda-*-keyring.gpg /usr/share/keyrings/
sudo apt-get update
sudo apt-get -y install cuda-toolkit-12-6
```

没什么坑，体验很好。

## opencl

- 官网的开发工具中，有针对 opencl 的页面：
  1.  <https://www.intel.cn/content/www/cn/zh/developer/tools/opencl-cpu-runtime/overview.html>
  2.  <https://www.intel.cn/content/www/cn/zh/developer/tools/opencl/overview.html>
- 这是一个介绍整个英特尔的 opencl 的页面，包括了 cpu、gpu 和 fpga，其中，cpu 的界面跳转依旧是上面的链接：<https://www.intel.cn/content/www/cn/zh/developer/articles/tool/opencl-drivers.html#cpu-section>

1. opencl 是一个标准，不同硬件平台（cuda，intel 等）的实现不尽相同
2. 不同平台实现后会有自己的 libOpenCL.so/OpenCL.dll 和头文件
   1. NV 平台的 cuda opencl 的时候，安装 CUDA toolkit 会一并安装
   2. intel 平台的 cpu opencl，需要单独安装
3. KhronosGroup 提供了一个示例的实现 OpenCL-SDK，和上面的厂商实现是同一个级别的概念
4. ocl-icd-opencl-dev 是一个管理多个 opencl 平台的工具
5. 根据参考资料 3 显示，目前无法通过 clinfo 发现 cuda 设备

## opencl in Windows

1. 一个 Windows 上的 clinfo 工具，安装了 cuda 和 intel 的 cl 驱动之后，就可以显示 2 个平台：<https://github.com/engineer1109/clinfo/releases/tag/v1.0>
2. 使用 intel 平台的 opencl，会出现 error，所以使用**CUDA**下的 opencl 进行测试

## opencl in WSL

1. 因为 wsl 中不支持 CUDA 的 opencl 接口，所以使用**intel**的 opencl SDK 进行测试
2. WSL 中按照 Linux 的安装指导：
   1. 使用`sudo apt install intel-oneapi-runtime-opencl`，发现没有这个包
   2. 需要看着文档设置一些东西：[ubuntu APT](https://www.intel.com/content/www/us/en/docs/oneapi/installation-guide-linux/2023-0/apt.html#GUID-560A487B-1B5B-4406-BB93-22BC7B526BCD)
   3. 再次执行就可以了

## AVX

查看当前系统的指令集支持`cat /proc/cpuinfo | grep flags`
进一步查看关于 AVX 的支持：`cat /proc/cpuinfo | grep avx`
查看编译器支持的指令集选项：`g++ --target-help`

AVX 的指令集，分为：

- avx：256 位寄存器
- avx2：256 位寄存器
- avx512：512 位寄存器，目前 intel 的 CPU 支持，AMD 的 CPU 不支持
  - avx_vnni：avx512 的子集，只能处理 int8 的数据；本机只显示这个，说明不能使用完整的 avx512 的指令，计算 float 的时候，只能退回到 avx/avx2 的 256bit 长度

## AVX 的测试

```cpp
    #ifdef __AVX__
    std::cout << "AVX is supported!" << std::endl;
    #else
    std::cout << "AVX is not supported." << std::endl;
    #endif

    #ifdef __AVX2__
    std::cout << "AVX2 is supported!" << std::endl;
    #else
    std::cout << "AVX2 is not supported." << std::endl;
    #endif

    #ifdef __AVX512F__
    std::cout << "AVX512F is supported." << std::endl;
    #else
    std::cout << "AVX512F is not supported." << std::endl;
    #endif

    #ifdef __AVX512VNNI__
    std::cout << "AVX512VNNI is supported." << std::endl;
    #else
    std::cout << "AVX512VNNI is not supported." << std::endl;
    #endif
```

使用 g++编译时，加上`-mavx`，`-mavx2`，`-mavx512f`或者`-mavx512vnni`；是向下兼容的，所以编译时加上`-mavx512vnni`，前面的也都可以使用

编译命令：

```sh
g++ test.cc -o test -mavx512vnni
```

## SSE

有以下 SSE 的指令集：

- sse
- sse2
- ssse3
- sse4_1
- sse4_2

## SSE 的测试

```cpp
    #ifdef __SSE__
    std::cout << "SSE is supported!" << std::endl;
    #else
    std::cout << "SSE is not supported." << std::endl;
    #endif

    #ifdef __SSE2__
    std::cout << "SSE2 is supported!" << std::endl;
    #else
    std::cout << "SSE2 is not supported." << std::endl;
    #endif

    #ifdef __SSE3__
    std::cout << "SSE3 is supported." << std::endl;
    #else
    std::cout << "SSE3 is not supported." << std::endl;
    #endif

    #ifdef __SSSE3__
    std::cout << "SSSE3 is supported." << std::endl;
    #else
    std::cout << "SSSE3 is not supported." << std::endl;
    #endif

    #ifdef __SSE4_1__
    std::cout << "SSE4_1 is supported." << std::endl;
    #else
    std::cout << "SSE4_1 is not supported." << std::endl;
    #endif

    #ifdef __SSE4_2__
    std::cout << "SSE4_2 is supported." << std::endl;
    #else
    std::cout << "SSE4_2 is not supported." << std::endl;
    #endif
```

使用 g++编译时，加上`-msse`，`-msse2`，`-msse3`，`-mssse3`，-`msse4.1`，-`msse4.2`；是向下兼容的，所以编译时加上`-msse4.2`，前面的也都可以使用

编译命令：

```sh
g++ test.cc -o test -msse4.2
```

avx和sse对应的msvc编译器选项：
```ps1
/arch:<SSE2|SSE4.2|AVX|AVX2|AVX512|AVX10.x> 最低 CPU 体系结构要求，以下之一:
   SSE2 - (默认)允许使用通过支持 SSE2 的 CPU 提供的指令
   SSE4.2 - 允许使用通过支持 SSE4.2 的 CPU 提供的指令
   AVX - 允许使用通过支持 AVX 的 CPU 提供的指令
   AVX2 - 允许使用通过支持 AVX2 的 CPU 提供的指令
   AVX512 - 允许使用通过支持 AVX-512 的 CPU 提供的指令
   AVX - 允许使用通过支持 AVX10.x 的 CPU 提供的指令。x 的有效值为 1
```

## intel MKL

安装页面：[官网](https://www.intel.cn/content/www/cn/zh/developer/tools/oneapi/onemkl-download.html?operatingsystem=linux&linux-install=apt)

和 opencl 是一样的，都被集成到了 oneapi 中，但是 opencl 藏得更深

前面的环境变量配置和安装 opencl 应该是一样的，我在 opencl 安装的过程中已经配置了，所以这里可以直接安装

```sh
sudo apt install intel-oneapi-mkl-devel
```

## intel MKL 的测试

编译命令：

```sh
g++ test.cc -o test -I/opt/intel/oneapi/mkl/latest/include -L/opt/intel/oneapi/mkl/latest/lib -lmkl_rt -lpthread -Wl,-rpath,/opt/intel/oneapi/mkl/latest/lib
```

Windows下需要链接`mkl_intel_lp64.lib，mkl_core.lib，mkl_sequential.lib`，如果链接`mkl_rt.lib`就会在运行时缺少dll导致失败。

## opengl

有以下 opengl 的分类：

1. OpenGL Core Profile
2. OpenGL Compatibility Profile
3. OpenGL ES（Embedded Systems）
4. OpenGL Vulkan

## opengl in Windows

也需要下载对应的Windows的glew和glfw：
- glew：http://glew.sourceforge.net/
- glfw：https://www.glfw.org/download.html

很多Windows平台的包不提供mingw的版本（.a, .so），只提供msvc的（.lib, .dll），所以建议使用msvc/clang-msvc

```ps1
clang++ -o test.exe test.cc -I "E:\glew-2.1.0\include" -I "E:\glfw-3.4.bin.WIN64\include" -L "E:\glfw-3.4.bin.WIN64\lib-vc2022" -L "E:\glew-2.1.0\lib\Release\x64" -lopengl32 -lglew32 -lglfw3 -luser32 -lgdi32 -lshell32 -lmsvcrt
```

不加-lmsvcrt会找不到一些标准库的符号，加上会报错找不到glew32.dll，不建议折腾。

1. 一般来说，安装了 NV 的 driver 就会有 opengl，但实际发现并没有，opengl 也不是跟随 CUDA 安装的，这一点和 opencl 不同。
2. 链接：<https://developer.nvidia.com/opengl>，虽然说 NV 支持 opengl，但是下面的更新到 2016 年就没有了，驱动链接点进去也是 NV 的 driver 官网，合理推测希望大家都使用 cuda 的接口。
3. 在 Windows 上依靠 NV 进行 opengl 开发应该不可行
4. 被逼无奈，下载专门的软件来查看一下：GL view（GPU Extensions Viewer）<https://www.realtech-vr.com/home/?page_id=142>：发现是有的，版本 4.6 也很新，BTW，这软件还挺不错的，能看很多系统信息。
5. 重新在官网寻找，发现了 nsight-graphics：<https://developer.nvidia.com/nsight-graphics>，似乎 opengl 等图形相关的被打包进这里面了。nsight-graphics 是属于 NVIDIA Nsight Developer Tools<https://developer.nvidia.com/tools-overview>的一部分，可以单独下载。文档链接：<https://docs.nvidia.com/nsight-graphics/index.html>

## opengl in WSL

1. 依赖：`sudo apt install mesa-utils`
2. 执行：`glxinfo | grep "OpenGL version"`，安装了 NV 的 cuda 后，这个就直接有了
3. 依赖：`sudo apt install libglew-dev libglfw3-dev`，`glfw`不是 OpenGL 本身的必需部分，但它是一个非常流行且实用的库[https://github.com/glfw/glfw](https://github.com/glfw/glfw)，通常用来简化一些与窗口管理、输入处理、上下文创建等相关的工作。简单来说，GLFW 为 OpenGL 提供了一个平台无关的窗口管理和输入处理的框架，方便开发者进行图形编程
4. 与 opencl 类似，在 WSL 中，即使安装了 NV 的驱动（使用 nvidia-smi 有输出），opengl 也是只能借助 MS 的虚拟机实现

编译命令：

```sh
g++ test.cc -o test -lGL -lGLEW -lglfw
```

## 参考资料

1. <https://github.com/KhronosGroup/OpenCL-Guide>
2. 一个博客，用来在 WSL 中发现 opencl 的 cuda 设备，还没有验证有效性，<https://medium.com/@tackboon97_98523/how-to-install-opencl-on-wsl-ubuntu-to-detect-a-cuda-gpu-device-30f334a415ec>
3. 一个 GitHub Issue，讨论在 WSL 中使用 clinfo 发现 cuda 设备的问题，讨论的很火热，并且目前还没有关闭：<https://github.com/microsoft/WSL/issues/6951>

## arm neon

1. x86平台，需要交叉编译的gcc版本，或者使用ndk中的clang
2. 所以不论是Windows还是Linux，都需要交叉编译

## arm neon 的测试

这里的`aarch64-linux-android21-clang`中`aarch64`是目标架构，`linux`是目标内核，`android21`是ABI，`clang`是编译器。实际上ndk中只有目标架构的区别，分别是armv7a，i686，x86_64以及最新的risc-v

不需要开启`-march=armv8-a+simd`，因为arm64 ABI默认开启

编译命令：

```sh
E:\android_sdk\ndk\29.0.13846066\toolchains\llvm\prebuilt\windows-x86_64\bin\aarch64-linux-android21-clang .\test.cc -o test
```

## arm sve

支持 SVE（Scalable Vector Extension） 的 ARM CPU 主要集中在面向服务器和高性能计算的芯片，以及部分高端桌面和研发平台。移动端（比如手机和平板）绝大多数 ARM CPU 目前不支持 SVE，主要用的是 NEON（asimd）。目前的Android emulator不支持sve，红米k60是骁龙8gen1，也不支持sve。

## arm sve 的测试

需要开启`-march=armv8.2-a+sve`，默认不开启；很神奇的一点是，必须在cmd窗口下才行，不能用pwsh

编译命令：

```sh
E:\android_sdk\ndk\29.0.13846066\toolchains\llvm\prebuilt\windows-x86_64\bin\aarch64-linux-android21-clang .\test.cc -o test -march=armv8.2-a+sve
```
