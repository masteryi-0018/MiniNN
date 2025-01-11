# backend

> 对于同一个计算在不同硬件上面的优化

## CUDA

下载地址：<https://developer.nvidia.com/cuda-downloads>

1. NV显卡的驱动是共用的，也就是在wsl下执行`nvidia-smi`可以显示信息
2. cmake中增加CUDA语言
3. 将cu文件编译为库，并链接
4. 暴露cuh头文件，被kernel包含

## CUDA in Windows

1. 安装Windows平台的CUDA，检查环境变量是否添加，执行`nvcc -V`

Windows下的CUDA编译，默认使用MSVC的cl编译器，使用`-ccbin`传递参数，必须传递路径，否则不能识别；经实验，即使传递路径，也会有报错提示：Host compiler targets unsupported OS.

所以只能用cl编译器，那么就意味着需要在VS powershel中编译，即使这样，也会因为CUDA和NSVC的版本不匹配，需要添加`-allow-unsupported-compiler`来保证启用cl编译器，但是即使这样，开始编译后依旧会有代码中的错误检查`error: static assertion failed`。综合来看，**不建议**在Windows下编译CUDA程序。

## CUDA in WSL

1. Windows上面安装的CUDA在wsl中无法使用，需要在NVIDIA官网再下载一下wsl-linux版本：

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

- 官网的开发工具中，有针对opencl的页面：
   1. <https://www.intel.cn/content/www/cn/zh/developer/tools/opencl-cpu-runtime/overview.html>
   2. <https://www.intel.cn/content/www/cn/zh/developer/tools/opencl/overview.html>
- 这是一个介绍整个英特尔的opencl的页面，包括了cpu、gpu和fpga，其中，cpu的界面跳转依旧是上面的链接：<https://www.intel.cn/content/www/cn/zh/developer/articles/tool/opencl-drivers.html#cpu-section>

1. opencl是一个标准，不同硬件平台（cuda，intel等）的实现不尽相同
2. 不同平台实现后会有自己的libOpenCL.so/OpenCL.dll和头文件
   1. NV平台的cuda opencl的时候，安装CUDA toolkit会一并安装
   2. intel平台的cpu opencl，需要单独安装
3. KhronosGroup提供了一个示例的实现OpenCL-SDK，和上面的厂商实现是同一个级别的概念
4. ocl-icd-opencl-dev是一个管理多个opencl平台的工具
5. 根据参考资料3显示，目前无法通过clinfo发现cuda设备

## opencl in Windows

1. 一个Windows上的clinfo工具，安装了cuda和intel的cl驱动之后，就可以显示2个平台：<https://github.com/engineer1109/clinfo/releases/tag/v1.0>
2. 使用intel平台的opencl，会出现error，所以使用**CUDA**下的opencl进行测试

## opencl in WSL

1. 因为wsl中不支持CUDA的opencl接口，所以使用**intel**的opencl SDK进行测试
2. WSL中按照Linux的安装指导：
   1. 使用`sudo apt install intel-oneapi-runtime-opencl`，发现没有这个包
   2. 需要看着文档设置一些东西：[ubuntu APT](https://www.intel.com/content/www/us/en/docs/oneapi/installation-guide-linux/2023-0/apt.html#GUID-560A487B-1B5B-4406-BB93-22BC7B526BCD)
   3. 再次执行就可以了

## AVX

查看当前系统的指令集支持`cat /proc/cpuinfo | grep flags`
进一步查看关于AVX的支持：`cat /proc/cpuinfo | grep avx`
查看编译器支持的指令集选项：`g++ --target-help`

AVX的指令集，分为：
- avx：256位寄存器
- avx2：256位寄存器
- avx512：512位寄存器，目前intel的CPU支持，AMD的CPU不支持
  - avx_vnni：avx512的子集，只能处理int8的数据；本机只显示这个，说明不能使用完整的avx512的指令，计算float的时候，只能退回到avx/avx2的256bit长度

## AVX的测试

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

使用g++编译时，加上`-mavx`，`-mavx2`，`-mavx512f`或者`-mavx512vnni`；是向下兼容的，所以编译时加上`-mavx512vnni`，前面的也都可以使用

编译命令：
```sh
g++ test.cc -o test -mavx512vnni
```

## SSE

有以下SSE的指令集：
- sse
- sse2
- ssse3
- sse4_1
- sse4_2

## SSE的测试

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

使用g++编译时，加上`-msse`，`-msse2`，`-msse3`，`-mssse3`，-`msse4.1`，-`msse4.2`；是向下兼容的，所以编译时加上`-msse4.2`，前面的也都可以使用

编译命令：
```sh
g++ test.cc -o test -msse4.2
```

## intel MKL

安装页面：[官网](https://www.intel.cn/content/www/cn/zh/developer/tools/oneapi/onemkl-download.html?operatingsystem=linux&linux-install=apt)

和opencl是一样的，都被集成到了oneapi中，但是opencl藏得更深

前面的环境变量配置和安装opencl应该是一样的，我在opencl安装的过程中已经配置了，所以这里可以直接安装

```sh
sudo apt install intel-oneapi-mkl-devel
```

## intel MKL的测试

编译命令：
```sh
g++ test.cc -o test -I/opt/intel/oneapi/mkl/latest/include -L/opt/intel/oneapi/mkl/latest/lib -lmkl_rt -lpthread -Wl,-rpath,/opt/intel/oneapi/mkl/latest/lib
```

## opengl

有以下opengl的分类：
1. OpenGL Core Profile
2. OpenGL Compatibility Profile
3. OpenGL ES（Embedded Systems）
4. OpenGL Vulkan

## opengl in Windows

1. 一般来说，安装了NV的driver就会有opengl，但实际发现并没有，opengl也不是跟随CUDA安装的，这一点和opencl不同。
2. 链接：<https://developer.nvidia.com/opengl>，虽然说NV支持opengl，但是下面的更新到2016年就没有了，驱动链接点进去也是NV的driver官网，合理推测希望大家都使用cuda的接口。
3. 在Windows上依靠NV进行opengl开发应该不可行

## opengl in WSL

1. 依赖：`sudo apt install mesa-utils`
2. 执行：`glxinfo | grep "OpenGL version"`，安装了NV的cuda后，这个就直接有了
3. 依赖：`sudo apt-get install libglfw3 libglfw3-dev`，`glfw`不是 OpenGL 本身的必需部分，但它是一个非常流行且实用的库[https://github.com/glfw/glfw](https://github.com/glfw/glfw)，通常用来简化一些与窗口管理、输入处理、上下文创建等相关的工作。简单来说，GLFW 为 OpenGL 提供了一个平台无关的窗口管理和输入处理的框架，方便开发者进行图形编程
4. 与opencl类似，在WSL中，即使安装了NV的驱动（使用nvidia-smi有输出），opengl也是只能借助MS的虚拟机实现

编译命令：
```sh
g++ -o test test.cc -lGLEW -lGL -lglfw
```

## 参考资料

1. <https://github.com/KhronosGroup/OpenCL-Guide>
2. 一个博客，用来在WSL中发现opencl的cuda设备，还没有验证有效性，<https://medium.com/@tackboon97_98523/how-to-install-opencl-on-wsl-ubuntu-to-detect-a-cuda-gpu-device-30f334a415ec>
3. 一个GitHub Issue，讨论在WSL中使用clinfo发现cuda设备的问题，讨论的很火热，并且目前还没有关闭：<https://github.com/microsoft/WSL/issues/6951>