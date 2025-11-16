import argparse
import os
import platform
import shutil
import subprocess
import sys


def run_command(cmd, cwd=None):
    print(f"Running command: {' '.join(cmd)}")
    try:
        subprocess.check_call(cmd, cwd=cwd)
    except subprocess.CalledProcessError as e:
        print(f"Command failed with exit code {e.returncode}")
        sys.exit(e.returncode)


def build_cmake(args):
    cmake_args = ["cmake", "-B", "build", "-S", "."]
    os.makedirs("build", exist_ok=True)

    current_platform = platform.system().lower()
    if args.target == "android":
        if args.generator == "ninja":
            cmake_args.extend(["-G", "Ninja"])
        elif args.generator == "make":
            cmake_args.extend(["-G", "Unix Makefiles"])
        else:
            print(
                "Unsupported generator for Android. Use 'ninja' or 'make'. Set default generator to 'ninja'."
            )
            cmake_args.extend(["-G", "Ninja"])

        # if set compiler, cmake will re-generate the build files of linux target
        # if args.compiler == "clang":
        #     cmake_args.extend(["-DCMAKE_C_COMPILER=clang", "-DCMAKE_CXX_COMPILER=clang++"])
        # else:
        #     print("Unsupported compiler for Android. Use 'clang'. Set default compiler to 'clang'.")
        #     cmake_args.extend(["-DCMAKE_C_COMPILER=clang", "-DCMAKE_CXX_COMPILER=clang++"])

        if current_platform == "windows":
            cmake_args.extend(
                [
                    "-DCMAKE_TOOLCHAIN_FILE=E:\\android_sdk\\ndk\\25.2.9519653\\build\\cmake\\android.toolchain.cmake"
                ]
            )
        elif current_platform == "linux":
            cmake_args.extend(
                [
                    "-DCMAKE_TOOLCHAIN_FILE=/home/gy/tools/android-ndk-r21e/build/cmake/android.toolchain.cmake"
                ]
            )
        cmake_args.extend(["-DANDROID_ABI=arm64-v8a"])
        cmake_args.extend(
            ["-DANDROID_PLATFORM=android-21"]
        )  # Android 5 (Lollipop)	android-21, minimal version

    elif current_platform == "windows":

        if args.generator == "ninja":
            cmake_args.extend(["-G", "Ninja"])
        elif args.generator == "mingw":
            cmake_args.extend(["-G", "MinGW Makefiles"])
        elif args.generator == "vs2022":
            cmake_args.extend(["-G", "Visual Studio 17 2022"])
        else:
            print(
                "Unsupported generator for Windows. Use 'ninja', 'mingw', or 'vs2022'. Set default generator to 'ninja'."
            )
            cmake_args.extend(["-G", "Ninja"])

        if args.compiler == "clang":
            cmake_args.extend(
                ["-DCMAKE_C_COMPILER=clang", "-DCMAKE_CXX_COMPILER=clang++"]
            )
        elif args.compiler == "gcc":
            cmake_args.extend(["-DCMAKE_C_COMPILER=gcc", "-DCMAKE_CXX_COMPILER=g++"])
            cmake_args.extend(["-DWITH_MKL=OFF"])  # MKL not supported with MinGW
        elif args.compiler == "cl":
            cmake_args.extend(["-DCMAKE_C_COMPILER=cl", "-DCMAKE_CXX_COMPILER=cl"])
        else:
            print(
                "Unsupported compiler for Windows. Use 'clang', 'gcc', or 'cl'. Set default compiler to 'clang'."
            )
            cmake_args.extend(
                ["-DCMAKE_C_COMPILER=clang", "-DCMAKE_CXX_COMPILER=clang++"]
            )

        if args.generator == "vs2022" and args.compiler != "cl":
            print(
                "Visual Studio generator only uses the MSVC compiler. Set default compiler to 'cl'."
            )
            cmake_args.extend(["-DCMAKE_C_COMPILER=cl", "-DCMAKE_CXX_COMPILER=cl"])

    elif current_platform == "linux":

        if args.generator == "ninja":
            cmake_args.extend(["-G", "Ninja"])
        elif args.generator == "make":
            cmake_args.extend(["-G", "Unix Makefiles"])
        else:
            print(
                "Unsupported generator for Linux. Use 'ninja' or 'make'. Set default generator to 'ninja'."
            )
            cmake_args.extend(["-G", "Ninja"])

        if args.compiler == "clang":
            cmake_args.extend(
                ["-DCMAKE_C_COMPILER=clang", "-DCMAKE_CXX_COMPILER=clang++"]
            )
        elif args.compiler == "gcc":
            cmake_args.extend(["-DCMAKE_C_COMPILER=gcc", "-DCMAKE_CXX_COMPILER=g++"])
        else:
            print(
                "Unsupported compiler for Linux. Use 'clang' or 'gcc'. Set default compiler to 'clang'."
            )
            cmake_args.extend(
                ["-DCMAKE_C_COMPILER=clang", "-DCMAKE_CXX_COMPILER=clang++"]
            )

    if args.target == "android":
        cmake_args.extend(
            [
                "-DWITH_MULTI_THREADS=OFF",
                "-DWITH_CUDA=OFF",
                "-DWITH_OPENCL=OFF",
                "-DWITH_AVX=OFF",
                "-DWITH_SSE=OFF",
                "-DWITH_MKL=OFF",
                "-DWITH_NEON=ON",
            ]
        )
    else:
        cmake_args.extend(
            [
                "-DWITH_NEON=OFF",
            ]
        )
    if args.wheel:
        cmake_args.extend(
            [
                "-DWITH_MULTI_THREADS=OFF",
                "-DWITH_CUDA=OFF",
                "-DWITH_OPENCL=OFF",
                "-DWITH_AVX=OFF",
                "-DWITH_SSE=OFF",
                "-DWITH_MKL=OFF",
                "-DWITH_NEON=OFF",
            ]
        )
    run_command(cmake_args)
    run_command(["cmake", "--build", "."], cwd="build")
    if args.wheel and args.target != "android":
        build_wheel(args)


def clean_cmake():
    if os.path.exists("build"):
        print(f"Removing build directory: 'build'")
        shutil.rmtree("build")
    else:
        print(f"No directory to remove: 'build'")

    if os.path.exists("mininn_sdk"):
        print(f"Removing mininn_sdk directory: 'mininn_sdk'")
        shutil.rmtree("mininn_sdk")
    else:
        print(f"No directory to remove: 'mininn_sdk'")

    if os.path.exists("python/dist"):
        shutil.rmtree("python/dist")
    if os.path.exists("python/mininn.egg-info"):
        shutil.rmtree("python/mininn.egg-info")
    if os.path.exists("python/mininn/mininn_capi.pyd"):
        os.remove("python/mininn/mininn_capi.pyd")
    if os.path.exists("python/mininn/mininn_capi.so"):
        os.remove("python/mininn/mininn_capi.so")

    pycache_count = 0
    for root, dirs, files in os.walk("."):
        if "__pycache__" in dirs:
            pycache_path = os.path.join(root, "__pycache__")
            try:
                shutil.rmtree(pycache_path)
                print(f"Removing pycache directory: '{pycache_path}'")
                pycache_count += 1
            except Exception as e:
                print(f"Error removing {pycache_path}: {e}")

    if pycache_count > 0:
        print(f"Removed {pycache_count} __pycache__ directories")
    else:
        print("No __pycache__ directories to remove")


def build_bazel(args):
    bazel_args = ["bazel", "build", "//mininn/...", "//python/...", "//demo/..."]
    if args.wheel:
        bazel_args.extend(["--define", "with_wheel=1"])
    run_command(bazel_args)


def clean_bazel():
    # use --expunge to clean all
    run_command(["bazel", "clean", "--expunge"])
    os.remove("MODULE.bazel.lock")


def build_wheel(args):
    if args.tool == "cmake":
        if sys.platform == "win32":
            if args.generator == "vs2022":
                cmake_output = os.path.abspath(
                    "./build/python/Debug/mininn_capi.cp313-win_amd64.pyd"
                )
            else:
                cmake_output = os.path.abspath(
                    "./build/python/mininn_capi.cp313-win_amd64.pyd"
                )
        elif sys.platform == "linux":
            cmake_output = os.path.abspath(
                "./build/python/mininn_capi.cpython-313-x86_64-linux-gnu.so"
            )

    elif args.tool == "bazel":
        if sys.platform == "win32":
            cmake_output = os.path.abspath("../bazel-bin/python/mininn_capi.pyd")
        elif sys.platform == "linux":
            cmake_output = os.path.abspath("../bazel-bin/python/mininn_capi.so")

    # you can cp .pyd/.so file to pybuild/lib/mininn directly, but you should mkdir before python setup.py
    # target_dir = os.path.join("pybuild/lib", "mininn")
    # if not os.path.exists(target_dir):
    #     os.makedirs(target_dir)

    target_dir = os.path.join("python", "mininn")

    if os.path.exists(cmake_output):
        if sys.platform == "win32":
            shutil.copyfile(cmake_output, os.path.join(target_dir, "mininn_capi.pyd"))
        elif sys.platform == "linux":
            shutil.copyfile(cmake_output, os.path.join(target_dir, "mininn_capi.so"))
    else:
        raise FileNotFoundError(
            f"{cmake_output} does not exist. Build with CMake first."
        )

    run_command(
        ["python", "-m", "build"], cwd="python"
    )


def main():
    parser = argparse.ArgumentParser(
        description="Build and clean script for multiple tools, generator and compiler"
    )
    parser.add_argument("--target", choices=["windows", "linux", "android"])
    parser.add_argument("--tool", choices=["cmake", "bazel"], default="cmake")
    parser.add_argument(
        "--generator", choices=["ninja", "mingw", "vs2022", "make"], default="ninja"
    )
    parser.add_argument("--compiler", choices=["gcc", "clang", "cl"], default="clang")
    parser.add_argument("--clean", action="store_true")
    parser.add_argument("--wheel", action="store_true")

    args = parser.parse_args()

    if args.tool == "cmake":
        if args.clean:
            clean_cmake()
        else:
            build_cmake(args)

    elif args.tool == "bazel":
        if args.clean:
            clean_bazel()
        else:
            build_bazel(args)

    else:
        print(f"Unsupported tool: {args.tool}. Use 'cmake' or 'bazel'.")
        sys.exit(1)


if __name__ == "__main__":
    main()
