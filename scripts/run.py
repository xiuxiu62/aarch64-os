#!python3

import os
import subprocess
import argparse

# Configuration
DEFAULT_QEMU = "qemu-system-aarch64"
DEFAULT_MACHINE = "virt"
DEFAULT_CPU = "cortex-a53"
DEFAULT_MEMORY = "128M"
DEFAULT_BUILD_DIR = "build"
DEFAULT_KERNEL = "os.bin"

def run_qemu(args):
    qemu_cmd = [
        args.qemu,
        "-machine", args.machine,
        "-cpu", args.cpu,
        "-m", args.memory,
        "-kernel", os.path.join(args.build_dir, args.kernel),
        "-nographic",
        "-chardev", "stdio,id=char0,mux=on",
        "-mon", "chardev=char0,mode=readline",
        "-serial", "chardev:char0",
        "-d", "guest_errors,unimp,exec,cpu_reset",
        "-D", "qemu.log"
    ]

    if args.debug:
        qemu_cmd.extend(["-s", "-S"])
        print("QEMU is waiting for a debugger to connect. Use GDB to connect to localhost:1234")

    print(f"{' '.join(qemu_cmd)}")
    
    try:
        subprocess.run(qemu_cmd, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error running QEMU: {e}")
    except FileNotFoundError:
        print(f"Error: QEMU executable '{args.qemu}' not found. Please ensure QEMU is installed and in your PATH.")

def main():
    parser = argparse.ArgumentParser(description="Run ARM OS in QEMU")
    parser.add_argument("--qemu", default=DEFAULT_QEMU, help=f"Path to QEMU executable (default: {DEFAULT_QEMU})")
    parser.add_argument("--machine", default=DEFAULT_MACHINE, help=f"QEMU machine type (default: {DEFAULT_MACHINE})")
    parser.add_argument("--cpu", default=DEFAULT_CPU, help=f"CPU type (default: {DEFAULT_CPU})")
    parser.add_argument("--memory", default=DEFAULT_MEMORY, help=f"Memory size (default: {DEFAULT_MEMORY})")
    parser.add_argument("--build-dir", default=DEFAULT_BUILD_DIR, help=f"Build directory (default: {DEFAULT_BUILD_DIR})")
    parser.add_argument("--kernel", default=DEFAULT_KERNEL, help=f"Kernel filename (default: {DEFAULT_KERNEL})")
    parser.add_argument("--debug", action="store_true", help="Start QEMU in debug mode, waiting for GDB to connect")

    args = parser.parse_args()
    run_qemu(args)

if __name__ == "__main__":
    main()
