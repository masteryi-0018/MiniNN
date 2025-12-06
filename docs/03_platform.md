# 环境问题

最理想的效果：一份代码，到处运行；不需要额外的环境开销

> Write once, run anywhere (WORA)

使用的环境：

1. Windows，vscode，git，msys2(cmake，make，gcc)，clang，msvc，bazel
2. WSL2，Ubuntu，vscode，git，cmake，make，gcc，clang，msvc，bazel

## Windows，x86

1. 目前使用 cmake+ninja/make 完成构建与编译

- 首先编写语法较为复杂的`CMakeLists.txt`文件
- 为了不让中间产物污染当前目录，新建`build`文件夹处理
- 带来问题是需要手动创建`build`，并执行`ninja/make`命令，于是需要写一个`shell`脚本
- Windows 下运行 shell 依靠`git`，安装之后运行`sh ./build.sh`或者`bash ./build.sh`
- 没有的话也可以使用`powershell`，也写了一份，执行`.\build.ps1`

2. Windows 下执行`bash ./build.sh`编译产物为 Linux 平台下文件，执行`sh ./build.sh`正常

3. 编译出`.exe`文件，运行需要鼠标双击，给调试造成了不方便

- 解决方法：在 powershell 中运行`.\build\mininn\test\gtest-main.exe`，可以看到结果

4. Windows 下将各模块解耦，即 libutils，libgraph，libkernel 等 dll 分开编译，去除彼此的依赖关系，产物分散在各个目录，导致在命令行执行`.\build\mininn\test\gtest-main.exe`没有任何输出，日志也无法打印

- 问题原因：因为缺少依赖库。在 dll 找不到的情况下，ps1 命令行不会输出任何信息，造成了上述的诡异问题，回到窗口中双击 exe 启动，提示缺少 dll 库，此问题被记录在 Windows 的事件查看器中
- 解决方法：针对找不到 dll 的问题，Windows 没有像 Linux 一样记录了动态库链接的路径，然后寻找，Windows 只会在 exe 的同级目录寻找，所以可以复制 dll 到当前目录，exe 成功启动

5. Windows 下执行 gtest-main，涉及到虚函数动态绑定的接口，程序直接崩溃，没有段错误等日志打印；涉及到注册 kernel 的接口，程序直接崩溃，没有段错误等日志打印

- 问题原因：未知
- 解决方法：将多个 dll 合成一个 dll，发现问题依旧存在，应该和多个 dll 或者一个 dll 没有关系，将 dll 修改为 a 的静态库，测试成功通过

```ps1
# 6. 拷贝dll文件至可执行文件中
$destinationFolderPath = ".\build\mininn\test"
Copy-Item -Path .\build\mininn\graph\libgraph.dll -Destination $destinationFolderPath -Force
Copy-Item -Path .\build\mininn\kernel\libkernel.dll -Destination $destinationFolderPath -Force
Copy-Item -Path .\build\mininn\operator\liboperator.dll -Destination $destinationFolderPath -Force
Copy-Item -Path .\build\mininn\parser\libparser.dll -Destination $destinationFolderPath -Force
Copy-Item -Path .\build\mininn\runtime\libruntime.dll -Destination $destinationFolderPath -Force
Copy-Item -Path .\build\mininn\utils\libutils.dll -Destination $destinationFolderPath -Force
```

6. msvc 的编译问题，报错**attribute**，还有报错 error LNK2038

- 问题原因：Windows 的 msvc 没有**attribute**这个属性；在 Windows 编译 gtest 需要打开 gtest_force_shared_crt 选项，否则就会有问题
- 解决方案：使用条件宏控制，打开宏开关就可以

7. Windows 下使用 bazel 构建的问题，安装了 msvc 编译器，但是依然找不到 vc++构建工具

- 问题原因：bazel 不是单独使用编译器链接器等工具，而是使用一整套工具链
- 解决方法：不能只安装 vs build tools，一定要安装 VS IDE，IDE 中有一些脚本，能够让 bazel 找到使用 msvc 的方式，也不需要在 vs power shell 中，普通的 power shell 就可以找到 cl 等工具

8. Windows 下日志无法打印，程序崩溃，commit id`d23620646c5cbcef24dc2e46ed42092b1ba36756`之前

- 问题原因：Windows 下不同级别的目录使用反斜杠而不是斜杠，代码对斜杠进行操作导致问题
- 解决方法：对`#define __FILENAME__ (strrchr(__FILE__, '/') + 1)`进行修改

9. cmake 选择特定的编译器

- 方式 1：命令行传递参数`-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++`。不需要绝对路径
- 方式 2：cmakelists 中设置环境变量，不需要写绝对路径，命令可以被识别就可以

10. bazel 构建不能识别 flatbuffers 为 submodule 的相对路径

- 问题原因：虽然 submodule 将源代码都暴露了出来，但是 flatbuffers 内部的构建规则是自己的相对路径，无法被 mininn 识别到
- 解决方法：在 WORKSPACE 中将 flatbuffers 定义为外部依赖，就可以让 flatbuffers 管理自己的构建路径。同时在 bazel8.0+后默认使用 MODULE.bazel，需要显式声明为`--enable_workspace`

11. windows 下 bazel 编译日志乱码

- 问题原因：错误中的"936"表示系统正在使用 GBK 编码（中文 Windows 默认），MSVC 编译器期望源代码是带 BOM 的 UTF-8 或与系统区域设置匹配的编码
- 解决方法：修改系统区域设置（针对 MSVC 编译器警告）：进入控制面板 → 区域 → 管理 → 更改系统区域设置，勾选 "Beta 版：使用 Unicode UTF-8 提供全球语言支持"，重启计算机

12. glog 作为 submodule 编译错误

- 问题原因：默认 conda 环境包括了 gtest，使用 find_package 来寻找，导致链接的不是自己的 gtest
- 解决方法：新建 conda 环境，保证没有 gtest

13. gtest 在 windows 下无法编译得到 gmock_main 动态库

- 问题原因：使用 clang 编译器时无法导出某些符号，导致编译动态库失败
- 解决方法：强制指定编译静态库（-DBUILD_SHARED_LIBS=OFF），或者使用 gcc 编译器

14. windows 执行 gtest 时不打印任何信息就结束

- 问题原因：在终端执行遇到这个问题，可以双击exe，一般是缺少动态库
- 解决方法：找到动态库就行，或者将动态依赖修改为静态

15. Windows使用vs2022编译时，出现LINK : warning LNK4098: 默认库“LIBCMT”与其他库的使用冲突；

- 问题原因：同一个对象既链接了LIBCMT（静态多线程 CRT），又链接了MSVCRT（动态 CRT），在mininn里面是因为cuda自己编译的时候使用的时/MDd，但其他的应该是静态链接，所以同时出现了二者；提供静态库的时候一般是使用了/MT。动态库和可执行文件使用了/MD，可以用dumpbin /dependents xxx来查看
- 解决方法：目前来看应该是所有目标统一使用一种，但还没成功，后续应该会去掉cuda依赖

## Linux，x86

1. 额外安装的 SDK 运行时出现`cannot open shared object file: No such file or directory`

- 问题原因：在手动安装的 SDK 中，头文件和库文件都不在编译器默认的寻找路径中，所以需要手动指定路径，例如：
  - `-I/opt/intel/oneapi/mkl/latest/include`
  - `-L/opt/intel/oneapi/mkl/latest/lib`
  - 但是在运行时依然需要对应的打开库文件，这里只是让编译器编译时不出错
- 解决方法：使用`-Wl,-rpath,/opt/intel/oneapi/mkl/latest/lib`让编译产物链接到绝对路径，从而正确运行

2. 执行`bazel build //python:mininn_wheel`时出现`exec: -a: not found`

- 问题原因：这是许多现代 Linux 发行版（如 Debian、Ubuntu）的刻意设计，默认将 /bin/sh 链接到 dash 而非 bash 是出于性能和兼容性的权衡
- 解决方法：`sudo dpkg-reconfigure dash`然后选择No

## Android，arm

设置环境1

1. GitHub 下载[termux](https://github.com/termux/termux-app/releases)
2. 电脑配置 adb 工具。官网下载或借助 Android Studio 下载。
3. 连接手机。打开 开发者模式：连续点击「设置」-「关于手机」-「版本号」7 次。启用 USB 调试：在开发者选项中打开「USB 调试」。选择 正确的 USB 模式：连接时下拉通知栏，将 USB 模式设为「文件传输（MTP）」或「PTP」（部分机型需选「传输文件」）。
4. `adb install .\termux-app_v0.118.2+github-debug_arm64-v8a.apk`
5. 使用手机操作 termux
   ```sh
   pkg update
   pkg upgrade
   pkg install openssh
   passwd
   sshd
   whoami
   ifconfig
   ```
6. 使用电脑 ssh 登录，`ssh u0_a201@192.168.0.103 -p 8022`，有兴趣可以配置免密登录。
7. 手机设置 termux 自启动，允许后台活动，就可以保持随时 ssh 了。

设置环境2

1. 下载Android Studio
2. 添加adb所在路径到环境变量，就可以使用adb了
3. 在SDK中安装NDK，编译时使用NDK编译，就是Android平台的目标
4. 在Android Studio中下载一个模拟器，相当于一个虚拟的Android手机，型号不重要，下载好点击启动，就可以在adb中发现设备并且使用其运行程序了
5. 命令：
   ```sh
   adb push ./build/mininn/test/gtest-main /data/local/tmp
   adb push ./build/demo/demo /data/local/tmp
   adb push ./models/ /data/local/tmp
   adb shell chmod +x /data/local/tmp/*
   adb shell "cd /data/local/tmp/ && ./gtest-main"
   adb shell "cd /data/local/tmp/ && ./demo ./models/add_model.gynn"
   ```

设置环境3

1. 下载Android Studio
2. 使用sdkmanager安装adb，emulator，avdmanager（command-line-tool），avdmanager可能需要Java version 17 or higher，不想安装java的话可以在bat脚本中加入`set SKIP_JDK_VERSION_CHECK=1`，继续运行就好
3. 使用图形化界面或者命令行创建模拟器
4. 使用图形化界面或者命令行运行模拟器
5. 命令：
   ```
   avdmanager create avd -n test -k "system-images;android-21;default;arm64-v8a"
   emulator -avd test -no-snapshot -no-window -no-audio -gpu swiftshader_indirect
   ```
6. 之后就和上面一样了，也支持neon

问题

1. Android Studio不能运行arm64的Android镜像

- 问题原因：较低版本的emulator不能支持较高版本的API，`PANIC: Avd's CPU Architecture 'arm64' is not supported by the QEMU2 emulator on x86_64 host.`
- 解决方法：android studio日志目录在：`"C:\Users\masteryi\AppData\Local\Google\AndroidStudio2022.3\log\idea.log"`，删除，重新打开android studio运行模拟器，在新的日志中生成以下内容：

```sh
2025-08-03 10:17:21,943 [   7290]   INFO - Emulator: Pixel 3a API 34 - E:\android_sdk\emulator\emulator.exe -netdelay none -netspeed full -avd Pixel_3a_API_34 -qt-hide-window -grpc-use-token -idle-grpc-timeout 300
2025-08-03 10:17:22,211 [   7558]   INFO - Emulator: Pixel 3a API 34 - PANIC: Avd's CPU Architecture 'arm64' is not supported by the QEMU2 emulator on x86_64 host.
2025-08-03 10:17:22,211 [   7558]   INFO - Emulator: Pixel 3a API 34 - Android emulator version 35.6.11.0 (build_id 13610412) (CL:N/A)
2025-08-03 10:17:22,211 [   7558]   INFO - Emulator: Pixel 3a API 34 - Graphics backend: gfxstream
2025-08-03 10:17:22,211 [   7558]   INFO - Emulator: Pixel 3a API 34 - Found systemPath E:\android_sdk\system-images\android-34\google_apis\arm64-v8a\
2025-08-03 10:17:22,211 [   7558]   INFO - Emulator: Pixel 3a API 34 - Process finished with exit code 1
2025-08-03 10:17:22,211 [   7558] SEVERE - Emulator: Pixel 3a API 34 - Emulator terminated with exit code 1
java.lang.Throwable: Emulator terminated with exit code 1
```

查看源码：`https://android.googlesource.com/platform/external/qemu/+/refs/heads/emu-36-1-release/android/emulator/main-emulator.cpp#1095`，发现api>28就会报错，所以降低版本试试。另：platform/external/qemu/仓库main分支不是最新的，我找的是emu-36-1-release，还有emu-dev也比较新。按照这个，就找一个api<28的，也就是27（8.1）或者26（8.0），但是事与愿违，会报另一个错：`https://android.googlesource.com/platform/external/qemu/+/refs/heads/emu-36-1-release/android/emulator/main-emulator.cpp#1340`，试了几次，都不成功，遂放弃

android studio中创建已经默认不显示arm64的ABI了，勾选使用不支持的镜像才会出现，依然有感叹号提示运行会特别慢，但实际上是不支持的，可以在命令行运行:`emulator -avd Small_Phone -no-snapshot -no-window -no-audio -gpu swiftshader_indirect`，直接报错：PANIC: QEMU2 emulator does not support arm64 CPU architecture。但实际上是通过虚拟化运行 ARM 镜像，所以应该也是没问题的

2. WSL下直接再安装adb会报错

- 问题原因：一台物理机有2个adb会不兼容。
- 解决方法：直接使用Windows下的adb，`export PATH="E:\android_sdk\platform-tools:$PATH"`。详情见<https://blog.csdn.net/qq_45510888/article/details/133133338>

3. 可执行文件的属性

在Android下运行：
```sh
ldd /data/local/tmp/gtest-main
ldd /data/local/tmp/demo
```

4. 为什么emulator运行x86架构的android镜像，可以跑arm64的程序？

- 问题原因：（待确认）默认情况下，Android Studio 的 x86 模拟器 启用了 ARM 二进制翻译（即使你选择的是 x86_64 镜像）
- 解决方法：不用解决。参考：<https://www.v2ex.com/t/872539>

5. ubuntu配置Android SDK

- 按理说可以直接`sudo apt install android-sdk`，但还没有试过，看起来很多包很全
- 从官网下载可以选择完全版（Android studio 1.5GB）或者命令行工具（commandlinetools-linux-13114758_latest.zip），和ndk相似，在Windows下载后复制到ubuntu再unzip；这之后也需要安装java，`sudo apt install openjdk-17-jdk -y`，Ubuntu24似乎自带的java版本是21比较高，速度慢可以尝试apt换源