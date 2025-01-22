from setuptools import setup, find_packages, Extension

VERSION = '0.0.1' 
DESCRIPTION = 'My first Python package'
LONG_DESCRIPTION = 'My first Python package with a slightly longer description'

ext_modules = [
    Extension(
        'mininn',
        ["mininn.so"],
    ),
]


# 配置
setup(
    name="mininn", 
    version=VERSION,
    author="masteryi-0018",
    author_email="<1536474741@qq.com>",
    description=DESCRIPTION,
    long_description=LONG_DESCRIPTION,
    packages=find_packages(),
    install_requires=[],
)
