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
- 引入头文件，得到对应的符号的声明，然后如果有文件中没有实现，那么就需要显式的链接so，寻找实现
- 引入头文件，头文件包含了实现，就不需要链接了，即使主动声明链接关系，也不会真的建立

那么，在编译时指定的源文件，如果不停的引入头文件，那么就不断地将其他的头文件复制到源文件处，这里可能会有其他模块的头文件；但是不要紧，这样的链接关系只会增加一些代码，而不会破坏之前的解耦结果；另一种情况是，头文件中包含了实现，那么尽管头文件在某一个目录下，但实际上这里的代码已经被编译到了引用此头文件的模块中。

5. cmake寻找头文件规则

项目开始变大的时候，一般会将本项目与三方依赖分开，本项目内部也会有各种的目录，用于将不同模块分开。

本项目使用三方依赖时，一般使用尖括号（<>）来引入头文件，并且指定链接关系；本项目内部模块使用本项目模块时，一般使用双引号（""）来引入头文件，并且需要让cmake找到对应头文件路径；本项目内部模块使用本项目模块时，分为2种情况，一种是使用其他模块，一种是使用自己模块内的。

我们看到一些成熟的项目，一般使用绝对路径来引入头文件，也就是在最大的cmakelists中，`include_directories`，之后include头文件就直接以此目录为起点，写路径信息；如果不使用这个方法，就需要分别`include_directories`多次，将小的目录也加进来，这样就不用写较长的路径了。但是注意，如果目录内引入自己模块内的头文件，是可以忽略这个规则的，cmake在编译时，寻找源文件目录下的头文件不需要指定路径。

6. c和c++的一些头文件

比如：
- #include <stdlib.h>
- #include <cstdlib>

两个头文件<stdlib.h>和<cstdlib>实际上是相同的，它们都是C语言和C++语言中用于定义一些常用函数、宏和类型的标准库头文件。

<cstdlib>是C++标准库的头文件，而<stdlib.h>是C语言标准库的头文件。虽然它们提供的函数和宏基本相同，但是在C++中，<cstdlib>头文件中的函数和宏都位于std命名空间中。因此，如果你使用C++语言，应该使用<cstdlib>头文件，然后在代码中使用std命名空间，例如使用std::malloc()来调用malloc函数。

在C语言中，<stdlib.h>头文件中的函数和宏不在任何命名空间中，直接使用即可，例如使用malloc()来调用malloc函数。

7. 虚函数的使用

首先说一下继承的应用场景，某个时候新建对象，但不能确定新建对象的类型时，可以使用统一的父类指针接收这个对象；在实际使用时，调用某个方法（子类和父类都实现了）：

- 父类未声明虚函数，则由于此时是父类指针，所以会走父类的方法
- 父类声明虚函数，并实现，会根据具体对象的类型，判断走哪个方法
- 父类声明纯虚函数，则不会实现，此时一定走子类的方法

8. 源文件编译哪些内容

在编译时，会将cc/cpp源文件编译为o目标文件，这个过程包含了预编译，编译，汇编的步骤，之后将很多个o目标文件，链接为so或者a的库文件；如果o文件中有重复定义的符号（symbol），那么就会在链接时期报错。下面仔细思考一下源文件究竟编译了哪些内容：

- 引入头文件就是把头文件复制过去，头文件引入的其他头文件也会一直按照依赖关系复制到当前源文件中，所以有可能出现循环引用，这时候就需要使用宏保护或者利用编译器的特性（#pragma once）
- 把头文件的内容复制过去，和在源文件中【声明】类是一样的，所以可以将类声明写在源文件，起到的作用是一样的，但是为了不在多个源文件中写一样的代码，会单独写在头文件中
- 区分变量、函数、类的声明、定义和实现：一般声明在头文件中，不分配内存；定义和实现（定义里面的具体操作就是实现，所以可以认为这2个概念相同），一般在源文件中，会分配内存
- 如果在头文件中对变量定义，那么不同源文件就定义了相同的符号，不同o目标文件中就会出现相同的符号，导致链接错误
- 经过实验：一个头文件的声明在源文件不实现，源文件引用此头文件，里面的符号也不会被编译