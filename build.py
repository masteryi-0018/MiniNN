import os
import sys
import subprocess
import shutil
import argparse
import platform

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
    if current_platform == "windows":

        if args.generator == "ninja":
            cmake_args.extend(["-G", "Ninja"])
        elif args.generator == "mingw":
            cmake_args.extend(["-G", "MinGW Makefiles"])
        elif args.generator == "vs2022":
            cmake_args.extend(["-G", "Visual Studio 17 2022"])
        else:
            print("Unsupported generator for Windows. Use 'ninja', 'mingw', or 'vs2022'.")

        if args.compiler == "clang":
            cmake_args.extend(["-DCMAKE_C_COMPILER=clang", "-DCMAKE_CXX_COMPILER=clang++"])
        elif args.compiler == "gcc":
            cmake_args.extend(["-DCMAKE_C_COMPILER=gcc", "-DCMAKE_CXX_COMPILER=g++"])
        elif args.compiler == "cl":
            cmake_args.extend(["-DCMAKE_C_COMPILER=cl", "-DCMAKE_CXX_COMPILER=cl"])
        else:
            print("Unsupported compiler for Windows. Use 'clang', 'gcc', or 'cl'.")

        if args.generator == "vs2022" and args.compiler != "cl":
            print("Using Visual Studio generator only uses the MSVC compiler. Set --compiler to 'cl'.")

    elif current_platform == "linux":

        if args.generator == "ninja":
            cmake_args.extend(["-G", "Ninja"])
        elif args.generator == "make":
            cmake_args.extend(["-G", "Unix Makefiles"])
        else:
            print("Unsupported generator for Linux. Use 'ninja' or 'make'.")

        if args.compiler == "clang":
            cmake_args.extend(["-DCMAKE_C_COMPILER=clang", "-DCMAKE_CXX_COMPILER=clang++"])
        elif args.compiler == "gcc":
            cmake_args.extend(["-DCMAKE_C_COMPILER=gcc", "-DCMAKE_CXX_COMPILER=g++"])
        else:
            print("Unsupported compiler for Linux. Use 'clang' or 'gcc'.")

        if args.compiler == "cl":
            print("MSVC compiler is not supported on Linux. Please use 'clang' or 'gcc'.")
            sys.exit(1)

    run_command(cmake_args)
    run_command(["cmake", "--build", "."], cwd="build")


def clean_cmake():
    if os.path.exists("build"):
        print(f"Removing build directory: 'build'")
        shutil.rmtree("build")
    else:
        print(f"No build directory to remove: 'build'")

def build_bazel():
    run_command(["bazel", "build", "//mininn/..."])

def clean_bazel():
    # use --expunge to clean all
    run_command(["bazel", "clean"])


def main():
    parser = argparse.ArgumentParser(description="Build and clean script for multiple platforms and build systems")
    parser.add_argument("--tool", choices=["cmake", "bazel"], default="cmake")
    parser.add_argument("--generator", choices=["ninja", "mingw", "vs2022", "make"], default="vs2022")
    parser.add_argument("--compiler", choices=["gcc", "clang", "cl"], default="clang")
    parser.add_argument("--clean", action="store_true")

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
            build_bazel()

    else:
        print(f"Unsupported tool: {args.tool}. Use 'cmake' or 'bazel'.")
        sys.exit(1)

if __name__ == "__main__":
    main()