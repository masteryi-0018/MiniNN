# 构建系统

## cmake

1. cmake 寻找头文件规则

项目开始变大的时候，一般会将本项目与三方依赖分开，本项目内部也会有各种的目录，用于将不同模块分开。

本项目使用三方依赖时，一般使用尖括号（<>）来引入头文件，并且指定链接关系；本项目内部模块使用本项目模块时，一般使用双引号（""）来引入头文件，并且需要让 cmake 找到对应头文件路径；本项目内部模块使用本项目模块时，分为 2 种情况，一种是使用其他模块，一种是使用自己模块内的。

我们看到一些成熟的项目，一般使用绝对路径来引入头文件，也就是在最大的 cmakelists 中，`include_directories`，之后 include 头文件就直接以此目录为起点，写路径信息；如果不使用这个方法，就需要分别`include_directories`多次，将小的目录也加进来，这样就不用写较长的路径了。但是注意，如果目录内引入自己模块内的头文件，是可以忽略这个规则的，cmake 在编译时，寻找源文件目录下的头文件不需要指定路径。

2. 第三方库的编译问题

使用外部库，后一个参数指定编译产物路径，默认`${CMAKE_CURRENT_BINARY_DIR}/${source_dir}`，可能出现找不到的情况，建议提前设置其路径，再提供参数给 cmake：

```sh
set(GTEST_BUILD third_party/googletest)
add_subdirectory(../third_party/googletest ${GTEST_BUILD})
```

另一种方式是使用 cmake 的模块，`ExternalProject_Add`，具体来说，有 2 种方式使用`ExternalProject_Add`：

- 直接在项目的 cmakelists 种添加对应代码
- 新建自己的 cmake 模块（以.cmake 结尾的文件夹），添加对应代码

```sh
include(ExternalProject)

# 设置glog的源码和构建目录
set(GLOG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glog)
set(GLOG_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/glog)

# 使用ExternalProject_Add来构建glog
ExternalProject_Add(
    glog
    PREFIX ${GLOG_BINARY_DIR}
    SOURCE_DIR ${GLOG_SOURCE_DIR}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${GLOG_BINARY_DIR}
)
```

## bazel

Bazel 可以在许多不同的 build 配置中运行，包括使用 Android 原生开发套件 (NDK) 工具链的几种配置。这意味着，正常的 cc_library 和 cc_binary 规则可以直接在 Bazel 中针对 Android 进行编译。Bazel 通过使用 android_ndk_repository 代码库规则及其相关的 bzlmod 扩展程序来实现此目的。

android_ndk_repository是自带的，不支持新的ndk；使用新的ndk就需要用rules_android_ndk；如果用enable_bzlmod，需要手动设置`$env:ANDROID_NDK_HOME = "D:\project\MiniNN\29.0.13846066"`，但是也会报错，不推荐。

参考：<https://bazel.google.cn/docs/android-ndk?hl=zh-cn>