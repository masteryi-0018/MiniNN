from setuptools import setup, find_packages
from setuptools.command.build_py import build_py

import os
import shutil

VERSION = '0.0.4'
DESCRIPTION = 'python interface of MiniNN'
LONG_DESCRIPTION = 'Build a deep learning inference framework from scratch'

class CustomBuildCommand(build_py):
    """Custom build step to copy CMake-built .so file into the package."""
    def run(self):
        cmake_output = os.path.abspath("../build/python/mininn_capi.cpython-311-x86_64-linux-gnu.so")
        target_dir = os.path.join(self.build_lib, "mininn")

        if not os.path.exists(target_dir):
            os.makedirs(target_dir)

        if os.path.exists(cmake_output):
            shutil.copyfile(cmake_output, os.path.join(target_dir, "mininn_capi.so"))
        else:
            raise FileNotFoundError(f"{cmake_output} does not exist. Build with CMake first.")

        super().run()


setup(
    name = "mininn",
    version = VERSION,
    author = "masteryi-0018",
    author_email = "<1536474741@qq.com>",
    description = DESCRIPTION,
    long_description = LONG_DESCRIPTION,
    packages = find_packages(),
    cmdclass={"build_py": CustomBuildCommand},
)