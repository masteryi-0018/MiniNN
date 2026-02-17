import argparse
import glob
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
        if args.compiler == "clang":
            cmake_args.extend(
                ["-DCMAKE_C_COMPILER=clang", "-DCMAKE_CXX_COMPILER=clang++"]
            )
        else:
            print(
                "Unsupported compiler for Android. Use 'clang'. Set default compiler to 'clang'."
            )
            cmake_args.extend(
                ["-DCMAKE_C_COMPILER=clang", "-DCMAKE_CXX_COMPILER=clang++"]
            )

        ndk_home = os.environ.get("ANDROID_NDK_HOME")
        if not ndk_home:
            raise EnvironmentError("ANDROID_NDK_HOME not set")
        toolchain = os.path.join(ndk_home, "build", "cmake", "android.toolchain.cmake")
        cmake_args.extend([f"-DCMAKE_TOOLCHAIN_FILE={toolchain}"])
        cmake_args.extend(["-DANDROID_ABI=arm64-v8a"])
        cmake_args.extend(
            ["-DANDROID_PLATFORM=android-21"]
        )  # Android 5 (Lollipop)	android-21, minimal version

    elif current_platform == "windows":

        if args.generator == "ninja":
            cmake_args.extend(["-G", "Ninja"])
        elif args.generator == "vs2022":
            cmake_args.extend(["-G", "Visual Studio 17 2022"])
        else:
            print(
                "Unsupported generator for Windows. Use 'ninja' or 'vs2022'. Set default generator to 'ninja'."
            )
            cmake_args.extend(["-G", "Ninja"])

        if args.compiler == "clang":
            cmake_args.extend(
                ["-DCMAKE_C_COMPILER=clang", "-DCMAKE_CXX_COMPILER=clang++"]
            )
        elif args.compiler == "cl":
            cmake_args.extend(["-DCMAKE_C_COMPILER=cl", "-DCMAKE_CXX_COMPILER=cl"])
        else:
            print(
                "Unsupported compiler for Windows. Use 'clang' or 'cl'. Set default compiler to 'clang'."
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

    elif current_platform == "darwin":

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
        else:
            print(
                "Unsupported compiler for Linux. Use 'clang'. Set default compiler to 'clang'."
            )
            cmake_args.extend(
                ["-DCMAKE_C_COMPILER=clang", "-DCMAKE_CXX_COMPILER=clang++"]
            )

    if args.wheel:
        cmake_args.extend(["-DCMAKE_BUILD_TYPE=Release"])
        cmake_args.extend(
            [
                "-DWITH_MULTI_THREADS=OFF",
                "-DWITH_AVX=OFF",
                "-DWITH_SSE=OFF",
                "-DWITH_NEON=OFF",
            ]
        )
    else:
        arch = platform.machine()
        if args.target == "android":
            cmake_args.extend(
                [
                    "-DWITH_MULTI_THREADS=OFF",
                    "-DWITH_AVX=OFF",
                    "-DWITH_SSE=OFF",
                ]
            )
        elif args.target == "darwin" and arch == "arm64": # M chip
            cmake_args.extend(
                [
                    "-DWITH_AVX=OFF",
                    "-DWITH_SSE=OFF",
                ]
            )
        else:
            cmake_args.extend(
                [
                    "-DWITH_NEON=OFF",
                ]
            )
    run_command(cmake_args)
    run_command(["cmake", "--build", "."], cwd="build")
    run_command(["cmake", "--install", ".", "--config Debug"], cwd="build")
    if args.wheel and args.target != "android":
        build_wheel(args)


def clean_cmake():
    if os.path.exists("build"):
        print(f"Removing build directory: 'build'")
        shutil.rmtree("build")
    else:
        print(f"No directory to remove: 'build'")

    for file in glob.glob("cmake_test_discovery_*.json"):
        os.remove(file)
        print(f"Removing: {file}")
    clean_wheel()


def clean_wheel():
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


def build_wheel(args):
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
    elif sys.platform == "darwin":
        cmake_output = os.path.abspath(
            "./build/python/mininn_capi.cpython-313-darwin.so"
        )
    else:
        raise RuntimeError(f"Unsupported platform: {sys.platform}")

    target_dir = os.path.join("python", "mininn")

    if os.path.exists(cmake_output):
        if sys.platform == "win32":
            shutil.copyfile(cmake_output, os.path.join(target_dir, "mininn_capi.pyd"))
        elif sys.platform == "linux" or "darwin":
            shutil.copyfile(cmake_output, os.path.join(target_dir, "mininn_capi.so"))
    else:
        raise FileNotFoundError(
            f"{cmake_output} does not exist. Build with CMake first."
        )

    run_command(["python", "-m", "build"], cwd="python")


def main():
    parser = argparse.ArgumentParser(
        description="Build and clean script for multiple tools, generator and compiler"
    )
    parser.add_argument("--target", choices=["windows", "linux", "mac", "android"])
    parser.add_argument(
        "--generator", choices=["ninja", "vs2022", "make"], default="ninja"
    )
    parser.add_argument("--compiler", choices=["clang", "cl", "gcc"], default="clang")
    parser.add_argument("--clean", action="store_true")
    parser.add_argument("--wheel", action="store_true")

    args = parser.parse_args()

    if args.clean:
        clean_cmake()
    else:
        build_cmake(args)


if __name__ == "__main__":
    main()
