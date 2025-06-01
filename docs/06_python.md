# python

> 将cpp接口封装为python，并构建pypi的包

## 常见问题

1. 通过`python setup.py install`安装的包不会通过pip show显示，但可以通过pip uninstall卸载，即将弃用
2. 通过`pip install .`或者`pip install -e .`，不需要先构建whl再安装，适用于调试
3. 通过`python setup.py bdist_wheel`之后`pip install dist/xxx.whl`，更接近pypi的流程

## 上传pypi

1. pip install twine
2. python setup.py sdist bdist_wheel
3. twine upload dist/*

## bug

1. cpp可以正常注册算子，但是py逻辑不成功

因为cpp逻辑中在`use_op.h`和`use_kernel.h`中进行了use，在main的入口（例如gtest中）include了这2个头文件，所以可以防止编译器优化静态变量，成功注册，但是在py逻辑中没有include相应的文件，导致段错误。解决方式：可以在pybind中直接include这2个头文件。

2. windows下使用pybind11编译产物.pyd导入时报错

报错“ImportError: DLL load failed while importing mininn_capi: 找不到指定的模块”，可以使用dumpbin /dependents 查看pyd依赖的DLL是否都在路径中。在Windows下推荐用msvc或者clang（基于msvc的toolchain），否则会有gcc相关的动态库，导致导入时缺少依赖。