#!python3

import os
import sys
import subprocess

def run_command(command):
    try:
        subprocess.run(command, check=True, shell=True)
    except subprocess.CalledProcessError as e:
        print(f"Error: Command '{command}' failed with return code {e.returncode}")
        sys.exit(1)

def main():
    if len(sys.argv) != 2:
        print("Usage: python bt.py [init|build|run]")
        sys.exit(1)

    action = sys.argv[1].lower()

    build_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "../build")

    try:
        os.makedirs(build_dir, exist_ok=True)
        os.chdir(build_dir)
    except OSError as e:
        print(f"Error: Failed to create or change to build directory: {e}")
        sys.exit(1)

    if action == "init":
        run_command("cmake ..")
    if action == "build":
        run_command("cmake --build .")
    elif action == "run":
        run_command("cmake --build . --target run")
    else:
        print("Invalid argument. Use 'init' 'build' or 'run'.")
        sys.exit(1)

    print(f"CMake {action} completed successfully.")

if __name__ == "__main__":
    main()
