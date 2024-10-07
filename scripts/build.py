#!python3

import os
import subprocess
import sys

# Absolute path of the script's directory
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

# Define paths relative to the script's directory
TOOLS_DIR = os.path.join(SCRIPT_DIR, "..", "tools")
SRC_DIR   = os.path.join(SCRIPT_DIR, "..", "src")
BUILD_DIR = os.path.join(SCRIPT_DIR, "..", "build")
COMPILER  = os.path.join(TOOLS_DIR, "bin", "aarch64-none-elf-gcc-13.3.1")
ASSEMBLER = os.path.join(TOOLS_DIR, "bin", "aarch64-none-elf-as")
OBJCOPY   = os.path.join(TOOLS_DIR, "bin", "aarch64-none-elf-objcopy")
LINKER_SCRIPT = os.path.join(SCRIPT_DIR, "..", "linker.ld")

GCC_VERSION = "13.3.1"
GCC_TARGET = "aarch64-none-elf"

# ASFLAGS = ["-mcpu=cortex-a53"]
# CFLAGS = ["-mcpu=cortex-a53", "-ffreestanding", "-O2", "-Wall", "-Wextra"]
# LDFLAGS = ["-T", LINKER_SCRIPT, "-nostdlib"]

CFLAGS = [
    "-mcpu=cortex-a53",
    "-ffreestanding",
    "-fno-builtin",
    "-fno-stack-protector",
    "-fno-exceptions",
    "-fno-rtti",
    "-O2",
    "-Wall",
    "-Wextra",
    "-nostdlib",
    "-nostartfiles",
    "-nodefaultlibs",
    "-fno-pie",
    "-mgeneral-regs-only",
    "-c",
]

ASFLAGS = [
    "-mcpu=cortex-a53"
]

LDFLAGS = [
    "-ffreestanding",
    "-O2",
    "-nostdlib",
    "-nostartfiles",
    "-nodefaultlibs",
    "-static",
    "-Wl,--no-undefined",
    "-Wl,--gc-sections",
    f"-T{LINKER_SCRIPT}",
]

def set_environment():
    os.environ["PATH"] = os.path.join(TOOLS_DIR, "bin") + os.pathsep + os.environ["PATH"]
    os.environ["LIBRARY_PATH"] = os.path.join(TOOLS_DIR, GCC_TARGET, "lib")
    os.environ["C_INCLUDE_PATH"] = os.path.join(TOOLS_DIR, GCC_TARGET, "include")
    os.environ["CPLUS_INCLUDE_PATH"] = os.path.join(TOOLS_DIR, GCC_TARGET, "include")

def run_command(cmd):
    # print(f"Executing: {' '.join(cmd)}")
    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    if process.returncode != 0:
        print(f"Error executing command: {' '.join(cmd)}")
        print(f"STDOUT: {stdout.decode()}")
        print(f"STDERR: {stderr.decode()}")
        sys.exit(1)
    return stdout.decode()

def compile_file(file):
    object_file = os.path.join(BUILD_DIR, os.path.splitext(os.path.basename(file))[0] + ".o")
    if file.endswith(".c"):
        cmd = [COMPILER] + CFLAGS + [
            f"-B{os.path.join(TOOLS_DIR, 'lib', 'gcc', GCC_TARGET, GCC_VERSION)}",
            f"-I{os.path.join(TOOLS_DIR, GCC_TARGET, 'include')}",
            "-c", file, "-o", object_file
        ]
    elif file.endswith(".s"):
        cmd = [ASSEMBLER] + ASFLAGS + [file, "-o", object_file]
    else:
        print(f"Unsupported file type: {file}")
        sys.exit(1)
    run_command(cmd)
    return object_file
                        
def compile_file(file):
    object_file = os.path.join(BUILD_DIR, os.path.splitext(os.path.basename(file))[0] + ".o")
    if file.endswith(".c"):
        cmd = [COMPILER] + CFLAGS + [
            f"-B{os.path.join(TOOLS_DIR, 'lib', 'gcc', GCC_TARGET, GCC_VERSION)}"
            f"-I{os.path.join(TOOLS_DIR, GCC_TARGET)}"
            "-c", file, "-o", object_file
        ]
    elif file.endswith(".s"):
        cmd = [ASSEMBLER] + ASFLAGS + [file, "-o", object_file]
    else:
        print(f"Unsupported file type: {file}")
        sys.exit(1)
    run_command(cmd)
    return object_file

def generate_assembly(file):
    assembly_file = os.path.join(BUILD_DIR, os.path.splitext(os.path.basename(file))[0] + ".s")
    if file.endswith(".c"):
        cmd = [COMPILER] + CFLAGS + [
            f"-B{os.path.join(TOOLS_DIR, 'lib', 'gcc', GCC_TARGET, GCC_VERSION)}",
            f"-I{os.path.join(TOOLS_DIR, GCC_TARGET, 'include')}",
            "-S", file, "-o", assembly_file
        ]
        run_command(cmd)
        print(f"Generated assembly: {assembly_file}")

def main():
    set_environment()
    os.makedirs(BUILD_DIR, exist_ok=True)

    # Compile all .c and .s files
    object_files = []
    for file in os.listdir(SRC_DIR):
        if file.endswith(".c"):
            generate_assembly(os.path.join(SRC_DIR, file)); # TODO: remove after debug
        if file.endswith((".c", ".s")):
            object_files.append(compile_file(os.path.join(SRC_DIR, file)))

    if not object_files:
        print("No source files found to compile!")
        sys.exit(1)

    # Ensure boot.o is first in the list
    boot_object = next((obj for obj in object_files if obj.endswith("boot.o")), None)
    if boot_object:
        object_files.remove(boot_object)
        object_files.insert(0, boot_object)

    # Link object files
    output_elf = os.path.join(BUILD_DIR, "os.elf")
    cmd = [COMPILER] + LDFLAGS + ["-o", output_elf] + object_files
    run_command(cmd)

    # Create binary
    output_bin = os.path.join(BUILD_DIR, "os.bin")
    cmd = [OBJCOPY, "-O", "binary", output_elf, output_bin]
    run_command(cmd)

    print(f"Build complete. Output: {output_bin}")
    
if __name__ == "__main__":
    main()
