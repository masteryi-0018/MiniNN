import sys

from setuptools import find_packages, setup, Extension
from setuptools.command.build_ext import build_ext

VERSION = "1.1.0"
DESCRIPTION = "python interface of MiniNN"
LONG_DESCRIPTION = "Build a deep learning inference framework from scratch"

# windows can not use empty sources in Extension, so we create a dummy build_ext command
class DummyBuildExt(build_ext):
    def build_extension(self, ext):
        print(f"Skipping build for {ext.name} (prebuilt binary assumed)")

ext_modules = [Extension("mininn.mininn_capi", sources=[])]

setup(
    name="mininn",
    version=VERSION,
    author="masteryi-0018",
    author_email="<1536474741@qq.com>",
    description=DESCRIPTION,
    long_description=LONG_DESCRIPTION,
    packages=find_packages(),
    package_data={
        "mininn": (
            ["mininn_capi.pyd"] if sys.platform == "win32" else ["mininn_capi.so"]
        ),
    },
    ext_modules=ext_modules,
    cmdclass={"build_ext": DummyBuildExt},
    install_requires=[
        "onnxruntime", # >=1.23.2
        "onnx", # >=1.19.1
    ],
)
