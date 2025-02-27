cmake_minimum_required(VERSION 3.10)
project(corydon C ASM_NASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TOOLCHAIN_DIR "${PROJECT_SOURCE_DIR}/tools/toolchain/x86_64")
set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/x86_64-elf-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/x86_64-elf-g++)
set(CMAKE_ASM_NASM_COMPILER ${TOOLCHAIN_DIR}/bin/nasm)
set(CMAKE_LINKER ${TOOLCHAIN_DIR}/bin/x86_64-elf-ld)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

set(C_COMMON "-mcmodel=kernel -pipe -Wall -Wextra -O2 -fno-pic -mno-red-zone -ffreestanding -nostartfiles -nostdlib -lgcc")

set(NASM_FLAGS "-felf64")
set(CMAKE_C_FLAGS   ${C_COMMON})
set(CMAKE_CXX_FLAGS ${C_COMMON})

set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG ${C_COMMON} -O0")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG ${C_COMMON} -O0")
set(CMAKE_ASM_NASM_FLAGS_DEBUG "-g -felf64")

file(GLOB_RECURSE SOURCE_C CONFIGURE_DEPENDS "src/*.h" "src/*.c")
file(GLOB_RECURSE SOURCE_ASM CONFIGURE_DEPENDS "src/*.s" )

set(CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_CURRENT_LIST_DIR}/src/arch/x86_64/linker.ld")

set_source_files_properties(${SOURCE_ASM} PROPERTIES LANGUAGE ASM_NASM)

include_directories(src/include)
add_executable(corydon ${SOURCE_C} ${SOURCE_ASM})

add_custom_target(gdb
    COMMAND sh boot_limine.sh ${CMAKE_BINARY_DIR}/corydon
    COMMAND sh debug_qemu_gdb.sh ${CMAKE_BINARY_DIR}/corydon
    DEPENDS corydon
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tools"
)

add_custom_target(toolchain
    COMMAND sh build_toolchain.sh ${PROJECT_SOURCE_DIR}/tools/toolchain/
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tools"
)

add_custom_target(qemu
    COMMAND sh boot_limine.sh ${CMAKE_BINARY_DIR}/corydon
    COMMAND sh debug_qemu.sh
    DEPENDS corydon
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tools"
)

add_custom_target(iso
    COMMAND sh boot_limine.sh ${CMAKE_BINARY_DIR}/corydon
    COMMAND cp make_iso/corydon.iso ${CMAKE_BINARY_DIR}/
    DEPENDS corydon
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tools"
)
