cmake_minimum_required(VERSION 3.5)
project(corydon C ASM_NASM)

set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER $ENV{HOME}/opt/cross/x86_64-corydon/bin/x86_64-elf-gcc)
set(CMAKE_CXX_COMPILER $ENV{HOME}/opt/cross/x86_64-corydon/bin/x86_64-elf-g++)
set(CMAKE_ASM_NASM_COMPILER $ENV{HOME}/opt/cross/x86_64-corydon/bin/nasm)
set(CMAKE_LINKER $ENV{HOME}/opt/cross/x86_64-corydon/bin/x86_64-elf-ld)

set(CMAKE_ASM_NASM_FLAGS "-felf64")
set(CMAKE_C_FLAGS   "-pipe -Wall -Wextra -O2 -ffreestanding -nostartfiles -nostdlib -lgcc")
set(CMAKE_CXX_FLAGS "-pipe -Wall -Wextra -O2 -ffreestanding -nostartfiles -nostdlib -lgcc")

file(GLOB_RECURSE SOURCE_C CONFIGURE_DEPENDS "src/*.h" "src/*.c")
file(GLOB_RECURSE SOURCE_ASM CONFIGURE_DEPENDS "src/*.s" )

set(CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_CURRENT_LIST_DIR}/src/arch/x86_64/linker.ld -mcmodel=kernel")

set_source_files_properties(${SOURCE_ASM} PROPERTIES LANGUAGE ASM_NASM)
include_directories(src/include)
add_executable(corydon ${SOURCE_C} ${SOURCE_ASM})

add_custom_target(limine
    COMMAND sh limine.sh
    DEPENDS corydon
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tools"
)