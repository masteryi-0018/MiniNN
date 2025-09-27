import sys

from setuptools import find_packages, setup

VERSION = "1.0.0"
DESCRIPTION = "python interface of MiniNN"
LONG_DESCRIPTION = "Build a deep learning inference framework from scratch"

setup(
    name="mininn",
    version=VERSION,
    author="masteryi-0018",
    author_email="<1536474741@qq.com>",
    description=DESCRIPTION,
    long_description=LONG_DESCRIPTION,
    packages=find_packages(where="python"),
    package_dir={"": "python"},
    package_data={
        "mininn": (
            ["mininn_capi.pyd"] if sys.platform == "win32" else ["mininn_capi.so"]
        ),
    },
)
