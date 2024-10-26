cmake_minimum_required(VERSION 3.0)
project(corydon C ASM ASM_NASM)

set(CMAKE_ASM_NASM_FLAGS "-felf64")
set(CMAKE_C_FLAGS "-m64 -march=x86-64 -Wall -O2 -nostdlib -static -ffreestanding")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})
file(GLOB_RECURSE SOURCE_C CONFIGURE_DEPENDS "src/*.h" "src/*.c")
file(GLOB_RECURSE SOURCE_ASM CONFIGURE_DEPENDS "src/*.S" "src/*.s" "src/*.asm")
set(LDFLAGS "-melf_x86_64")
set(CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_CURRENT_LIST_DIR}/src/arch/x86_64/linker.ld")
set_source_files_properties(${SOURCE_ASM} PROPERTIES LANGUAGE ASM_NASM)
include_directories(src/arch/x86_64/)
include_directories(src/kernel/)
add_executable(corydon.elf ${SOURCE_C} ${SOURCE_ASM})

add_custom_target(limine
    COMMAND sh make.sh
    DEPENDS corydon.elf
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/scripts/test_limine"
)
