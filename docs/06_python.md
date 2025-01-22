# python

> 将cpp接口封装为python，并构建pypi的包

## 常见问题

1. 通过`python setup.py install`安装的包不会通过pip show显示，但可以通过pip uninstall卸载，即将弃用
2. 通过`pip install .`或者`pip install -e .`，不需要先构建whl再安装，适用于调试
3. 通过`python setup.py bdist_wheel`之后`pip install dist/xxx.whl`，更接近pypi的流程