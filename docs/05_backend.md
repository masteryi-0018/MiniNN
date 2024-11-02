# backend

> 对于同一个计算在不同硬件上面的优化

## CUDA

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

下载地址：<https://developer.nvidia.com/cuda-downloads>

2. NV显卡的驱动是共用的，也就是在wsl下执行`nvidia-smi`可以显示信息
3. cmake中增加CUDA语言
4. 将cu文件编译为库，并链接
5. 暴露cuh头文件，被kernel包含

## CUDA in Windows

Windows下的CUDA编译，默认使用MSVC的cl编译器，使用`-ccbin`传递参数，必须传递路径，否则不能识别；经实验，即使传递路径，也会有报错提示：Host compiler targets unsupported OS.

所以只能用cl编译器，那么就意味着需要在VS powershel中编译，即使这样，也会因为CUDA和NSVC的版本不匹配，需要添加`-allow-unsupported-compiler`来保证启用cl编译器，但是即使这样，开始编译后依旧会有代码中的错误检查`error: static assertion failed`。综合来看，不建议在Windows下编译CUDA程序。

## opencl

1. opencl是一个标准，不同硬件平台（cuda，intel等）的实现不尽相同
2. 不同平台实现后会有自己的opencl.so和头文件，在安装对应SDK（例如cuda）的时候，会一并安装
3. KhronosGroup提供了一个示例的实现OpenCL-SDK，和上面的厂商实现类似，但是非常简单
4. ocl-icd-opencl-dev是一个管理多个opencl平台的工具
5. 根据参考资料3显示，目前无法通过clinfo发现cuda设备

## opencl in WSL

1. 因为wsl中不支持CUDA的opencl接口，所以使用intel的opencl SDK进行测试
2. 官网的开发工具中，有针对opencl的页面：
   1. <https://www.intel.cn/content/www/cn/zh/developer/tools/opencl-cpu-runtime/overview.html>
   2. <https://www.intel.cn/content/www/cn/zh/developer/tools/opencl/overview.html>
3. 这是一个介绍整个英特尔的opencl的页面，包括了cpu、gpu和fpga，其中，cpu的界面跳转依旧是上面的链接：<https://www.intel.cn/content/www/cn/zh/developer/articles/tool/opencl-drivers.html#cpu-section>
4. WSL中按照Linux的安装指导：
   1. 使用`sudo apt install intel-oneapi-runtime-opencl`，发现没有这个包
   2. 需要看着文档设置一些东西：[ubuntu APT](https://www.intel.com/content/www/us/en/docs/oneapi/installation-guide-linux/2023-0/apt.html#GUID-560A487B-1B5B-4406-BB93-22BC7B526BCD)
   3. 再次执行就可以了

## 参考资料

1. <https://github.com/KhronosGroup/OpenCL-Guide>
2. 一个博客，用来在WSL中发现opencl的cuda设备，还没有验证有效性，<https://medium.com/@tackboon97_98523/how-to-install-opencl-on-wsl-ubuntu-to-detect-a-cuda-gpu-device-30f334a415ec>
3. 一个GitHub Issue，讨论在WSL中使用clinfo发现cuda设备的问题，讨论的很火热，并且目前还没有关闭：<https://github.com/microsoft/WSL/issues/6951>