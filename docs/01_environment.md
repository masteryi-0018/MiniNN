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


# 第三方库

1. 众所周知的单元测试工具`googletest(gtest)`

```
git submodule add https://github.com/google/googletest.git third_party/googletest
```

执行这个命令可以在本仓库中引入外部的仓库，作为`submodule`被git追踪（下载本仓库时一并下载），同时指定该仓库的位置在`third_party/googletest`，这样做的好处是源码编译，进一步了解构建项目的过程（给自己的项目构建增加难度）；[官方](https://github.com/google/googletest/tree/main/googletest)推荐：

- （单独构建）单独编译gtest，使用引入头文件+链接外部库的操作
- （作为项目的一部分）使用cmake的函数`FetchContent_MakeAvailable`

这时候对照ncnn项目，ncnn确实没有使用任何三方库，单元测试也是自己写的。