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