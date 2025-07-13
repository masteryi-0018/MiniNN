from setuptools import setup, find_packages

VERSION = '1.0.0'
DESCRIPTION = 'python interface of MiniNN'
LONG_DESCRIPTION = 'Build a deep learning inference framework from scratch'

setup(
    name = "mininn",
    version = VERSION,
    author = "masteryi-0018",
    author_email = "<1536474741@qq.com>",
    description = DESCRIPTION,
    long_description = LONG_DESCRIPTION,
    packages=find_packages(where="python"),
    package_dir={"": "python"},
    package_data={
        'mininn': ['mininn_capi.pyd'],
        'mininn': ['mininn_capi.so'],
    },
)