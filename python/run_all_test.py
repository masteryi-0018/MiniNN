import os
import sys
import subprocess

def run_command(cmd, cwd=None):
    print(f"Running command: {' '.join(cmd)}")
    try:
        subprocess.check_call(cmd, cwd=cwd)
    except subprocess.CalledProcessError as e:
        print(f"Command failed with exit code {e.returncode}")
        sys.exit(e.returncode)

def main():
    pybind_test = ["python", "python/test/pybind_test.py"]
    run_command(pybind_test)
    pip_test = ["python", "python/test/pip_test.py"]
    run_command(pip_test)
    fbs_test = ["python", "python/test/fbs/test.py"]
    run_command(fbs_test)
    model_test = ["python", "python/test/model_test/mv2_test.py"]
    run_command(model_test)

    folder_path = "python/test/unit_test/"
    all_files = os.listdir(folder_path)
    py_files = [f for f in all_files if f.endswith('.py') and os.path.isfile(os.path.join(folder_path, f))]
    for file_name in py_files:
        file_path = os.path.join(folder_path, file_name)
        pybind_test = ["python", file_path]
        run_command(pybind_test)

if __name__ == "__main__":
    main()
