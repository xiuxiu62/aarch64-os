cmake_minimum_required(VERSION 3.10)
project(os C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(TARGET_ARCH "aarch64-none-elf")

set(CMAKE_C_COMPILER clang)
set(CMAKE_ASM_COMPILER clang)
# set(CMAKE_OBJCOPY ${CMAKE_SOURCE_DIR}/tools/bin/aarch64-none-elf-objcopy)
set(CMAKE_OBJCOPY llvm-objcopy)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --target=${TARGET_ARCH} -ffreestanding -fno-builtin -nostdlib -mcpu=cortex-a53")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} --target=${TARGET_ARCH} -mcpu=cortex-a53")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/linker.ld -nostdlib")

include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB C_SOURCES   "src/*.c")
file(GLOB C_HEADERS   "src/*.h")
file(GLOB ASM_SOURCES "src/*.s")

add_executable(kernel.elf ${C_SOURCES} ${C_HEADERS} ${ASM_SOURCES})

set_target_properties(kernel.elf PROPERTIES LINK_DEPENDS ${CMAKE_SOURCE_DIR}/linker.ld)

add_custom_command(
    TARGET kernel.elf
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary kernel.elf kernel.bin
    COMMENT "Generating binary"
)

# Custom target to run QEMU
add_custom_target(run
    COMMAND qemu-system-aarch64 -M virt -cpu cortex-a53 -m 128M -kernel kernel.bin -nographic -chardev stdio,id=char0,mux=on -mon chardev=char0,mode=readline -serial chardev:char0 -d guest_errors,unimp,cpu_reset -D qemu.log
    DEPENDS kernel.elf
    COMMENT "Booting OS"
)
