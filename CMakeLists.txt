cmake_minimum_required(VERSION 3.10)

#Cmake flags
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_COMPILER /usr/bin/clang)

project(asm6502 C)

# Formatter
add_executable(formatter
        formatter/formatter.c
)

# Target: debug
add_executable(asm6502-dbg
        main.c
        lib/program_list.c
        lib/helper.c
        lib/lexer.c
        lib/labels.c
        lib/instruction_handlers.c
        lib/assembler.c
)

target_include_directories(asm6502-dbg PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(asm6502-dbg PRIVATE DEBUG)
target_compile_options(asm6502-dbg PRIVATE "-g;-O0;-Wall")

# Target: Release
add_executable(asm6502
        main.c
        lib/program_list.c
        lib/helper.c
        lib/lexer.c
        lib/labels.c
        lib/instruction_handlers.c
        lib/assembler.c
)

target_include_directories(asm6502 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(asm6502 PRIVATE "-O3;-fno-plt")