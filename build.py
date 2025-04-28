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

def build_cmake(build_dir):
    os.makedirs(build_dir, exist_ok=True)
    run_command(["cmake", ".."], cwd=build_dir)
    run_command(["cmake", "--build", "."], cwd=build_dir)

def clean_cmake(build_dir):
    if os.path.exists(build_dir):
        print(f"Removing build directory: {build_dir}")
        shutil.rmtree(build_dir)
    else:
        print(f"No build directory to remove: {build_dir}")

def build_bazel():
    run_command(["bazel", "build", "//mininn/..."])

def clean_bazel():
    # use --expunge to clean all
    run_command(["bazel", "clean"])

def main():
    parser = argparse.ArgumentParser(description="Build and clean script for multiple platforms and build systems")
    parser.add_argument("--platform", choices=["windows", "linux"], default=None, help="Target platform (default: current)")
    parser.add_argument("--tool", choices=["cmake", "bazel"], default="cmake", help="Build tool to use")
    parser.add_argument("--action", choices=["build", "clean"], default="build", help="Action to perform")
    parser.add_argument("--build-dir", default="build", help="Build directory for cmake (default: build)")

    args = parser.parse_args()

    current_platform = platform.system().lower()
    target_platform = args.platform or current_platform

    if target_platform != current_platform:
        print(f"Warning: You are building for {target_platform} on {current_platform}!")
        # Cross-compilation logic could be added here if needed.

    if args.tool == "cmake":
        if args.action == "build":
            build_cmake(args.build_dir)
        elif args.action == "clean":
            clean_cmake(args.build_dir)
    elif args.tool == "bazel":
        if args.action == "build":
            build_bazel()
        elif args.action == "clean":
            clean_bazel()
    else:
        print(f"Unknown tool: {args.tool}")
        sys.exit(1)

if __name__ == "__main__":
    main()