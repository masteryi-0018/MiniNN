# 环境问题

最理想的效果：一份代码，到处运行；不需要额外的环境开销

> Write once, run anywhere (WORA)

使用的环境：

1. Windows，vscode，git，msys2(cmake，make)
2. WSL2，Ubuntu，vscode，git，cmake，make

## Windows，x86

1. 目前使用cmake+make完成构建与编译

- 首先编写语法较为复杂的`CMakeLists.txt`文件
- 为了不让中间产物污染当前目录，新建`build`文件夹处理
- 带来问题是需要手动创建`build`，并执行`make`命令，于是需要写一个`shell`脚本
- Windows下运行shell依靠`git`，安装之后运行`sh ./build.sh`或者`bash ./build.sh`
- 没有的话也可以使用`powershell`，也写了一份，执行`.\build.ps1`

## 已知问题

1. Windows下执行`bash ./build.sh`编译产物为Linux平台下文件，执行`sh ./build.sh`正常

2. 编译出`.exe`文件，运行需要鼠标双击，给调试造成了不方便

- 解决方法，在powershell中运行`.\MiniNN-test.exe`，可以看到结果

3. Windows下将各模块解耦，即libutils，libgraph，libkernel等dll分开编译，去除彼此的依赖关系，产物分散在各个目录，导致在命令行执行`.\build\mininn\test\gtest-main.exe`没有任何输出，日志也无法打印

- 问题原因：因为缺少依赖库。在dll找不到的情况下，ps1命令行不会输出任何信息，造成了上述的诡异问题，回到窗口中双击exe启动，提示缺少dll库，此问题被记录在Windows的事件查看器中
- 解决方法：针对找不到dll的问题，Windows没有像Linux一样记录了动态库链接的路径，然后寻找，Windows只会在exe的同级目录寻找，所以可以复制dll到当前目录，exe成功启动

4. Windows下执行gtest-main，涉及到虚函数动态绑定的接口，程序直接崩溃，没有段错误等日志打印；涉及到注册kernel的接口，程序直接崩溃，没有段错误等日志打印

- 问题原因：未知
- 解决方法：将多个dll合成一个dll，发现问题依旧存在，应该和多个dll或者一个dll没有关系，将dll修改为a的静态库，测试成功通过

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


# 第三方库

1. 众所周知的单元测试工具`googletest(gtest)`

```
git submodule add https://github.com/google/googletest.git third_party/googletest
```

执行这个命令可以在本仓库中引入外部的仓库，作为`submodule`被git追踪（下载本仓库时一并下载），同时指定该仓库的位置在`third_party/googletest`，这样做的好处是源码编译，进一步了解构建项目的过程（给自己的项目构建增加难度）；[官方](https://github.com/google/googletest/tree/main/googletest)推荐：

- （单独构建）单独编译gtest，使用引入头文件+链接外部库的操作
- （作为项目的一部分）使用cmake的函数`FetchContent_MakeAvailable`

这时候对照ncnn项目，ncnn确实没有使用任何三方库，单元测试也是自己写的。