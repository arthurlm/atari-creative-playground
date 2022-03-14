cmake_minimum_required(VERSION 3.10)

project(AtariSTCreativeplayground)

# Set C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# Enable all warning
add_compile_options(-Wall)
# Set linker options
add_compile_options(-m68000)
add_link_options(-Wl,--traditional-format)

include_directories(src/includes)
add_executable(app.prg
    src/main.c
    src/linea.c
)
