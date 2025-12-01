## Git

1. 第三方库的维护

众所周知的单元测试工具`googletest(gtest)`

```
git submodule add https://github.com/google/googletest.git third_party/googletest
```

执行这个命令可以在本仓库中引入外部的仓库，作为`submodule`被 git 追踪（下载本仓库时一并下载），同时指定该仓库的位置在`third_party/googletest`，这样做的好处是源码编译，进一步了解构建项目的过程（给自己的项目构建增加难度）；[官方](https://github.com/google/googletest/tree/main/googletest)推荐：

- （单独构建）单独编译 gtest，使用引入头文件+链接外部库的操作
- （作为项目的一部分）使用 cmake 的函数`FetchContent_MakeAvailable`

这时候对照 ncnn 项目，ncnn 确实没有使用任何三方库，单元测试也是自己写的。

2. git删除submodule

```
git submodule deinit -f .\third_party\xxx
git rm -f  .\third_party\xxx
```