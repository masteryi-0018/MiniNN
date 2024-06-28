# C++项目中的一些思考

1. 头文件重复引用
```cpp
#ifndef XXX_H
#define XXX_H

// code

#endif // XXX_H
```

2. 不同模块的编译问题

例如将`operator`编译成一个库，静态库不需要处理与其他模块的依赖关系，动态库需要使用`target_link_libraries(operator xxx)`来链接，否则就会有链接问题，提示找不到某些符号。

最终提供的编译产物应该是唯一的库，所以需要将各个模块都进行链接，静态库使用`add_dependencies`，动态库使用`target_link_libraries`，最终都链接到一个最终的库上面。

3. 第三方库的编译问题

使用外部库，后一个参数指定编译产物路径，默认`${CMAKE_CURRENT_BINARY_DIR}/${source_dir}`，可能出现找不到的情况，建议提前设置其路径，再提供参数给cmake：

```sh
set(GTEST_BUILD third_party/googletest)
add_subdirectory(../third_party/googletest ${GTEST_BUILD})
```

另一种方式是使用cmake的模块，`ExternalProject_Add`，具体来说，有2种方式使用`ExternalProject_Add`：

- 直接在项目的cmakelists种添加对应代码
- 新建自己的cmake模块（以.cmake结尾的文件夹），添加对应代码

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

4. 编译产物的理解

将不同的模块解耦是一种好的设计方式，这里提到的解耦，就是不将全部的源文件编译为一个so，而是分成几个不同的so；so里面是符号表，也就是源码中的函数等，被编译到so中，然后被其他模块使用。

使用的方法是：
1. 引入头文件，得到对应的符号的声明，然后如果有文件中没有实现，那么就需要显式的链接so，寻找实现
2. 引入头文件，头文件包含了实现，就不需要链接了，即使主动声明链接关系，也不会真的建立

那么，在编译时指定的源文件，如果不停的引入头文件，那么就不断地将其他的头文件复制到源文件处，这里可能会有其他模块的头文件；但是不要紧，这样的链接关系只会增加一些代码，而不会破坏之前的解耦结果；另一种情况是，头文件中包含了实现，那么尽管头文件在某一个目录下，但实际上这里的代码已经被编译到了引用此头文件的模块中。