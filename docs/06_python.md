# python

> 将 cpp 接口封装为 python，并构建 pypi 的包

## 常见问题

1. 通过`python setup.py install`安装的包不会通过 pip show 显示，但可以通过 pip uninstall 卸载，即将弃用
2. 通过`pip install .`或者`pip install -e .`，不需要先构建 whl 再安装，适用于调试
3. 通过`python setup.py bdist_wheel`之后`pip install dist/xxx.whl`，更接近 pypi 的流程
4. 通过`python -m zipfile -l python/dist/mininn-1.0.0-py3-none-any.whl`来查看whl里面的内容
5. 通过`tar -tzf python/dist/mininn-1.0.0.tar.gz`来查看tar.gz里面的内容

## 上传 pypi

1. pip install twine
2. python setup.py sdist bdist_wheel
3. twine upload dist/\*

## bug

1. cpp 可以正常注册算子，但是 py 逻辑不成功

因为 cpp 逻辑中在`use_op.h`和`use_kernel.h`中进行了 use，在 main 的入口（例如 gtest 中）include 了这 2 个头文件，所以可以防止编译器优化静态变量，成功注册，但是在 py 逻辑中没有 include 相应的文件，导致段错误。解决方式：可以在 pybind 中直接 include 这 2 个头文件。

2. windows 下使用 pybind11 编译产物.pyd 导入时报错

报错“ImportError: DLL load failed while importing mininn_capi: 找不到指定的模块”，可以使用 dumpbin /dependents 查看 pyd 依赖的 DLL 是否都在路径中。在 Windows 下推荐用 msvc 或者 clang（基于 msvc 的 toolchain），否则会有 gcc 相关的动态库，导致导入时缺少依赖。

## 格式化

考虑black+isort方案

1. black

```sh
pip install black

black .

# 如果找不到命令
python -m black .

# 排除某些目录
python -m black . --exclude="third_party|fbs_schema|python/mininn/mininn_fbs"
```

2. isort

```sh
pip install isort

isort .

# 如果找不到命令
python -m isort .

# 排除某些目录
python -m isort . --skip-glob "third_party/*" --skip-glob "fbs_schema/*" --skip-glob "python/mininn/mininn_fbs/*"
```

3. 其他

- ruff(43k star): 据说是一键式完成，https://github.com/astral-sh/ruff
- 和black一个作用：
  - autopep8: ，python -m autopep8 --in-place --recursive .
  - yapf
- 静态检查Linter
  - flake8: python -m flake8 .
  - pylint
  - pycodestyle