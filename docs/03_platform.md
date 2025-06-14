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

设置环境

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
