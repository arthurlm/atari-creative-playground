cmake_minimum_required(VERSION 3.10)

project(AtariSTCreativeplayground)
# Enable all warning
add_compile_options(-Wall)
# Set linker options
add_compile_options(-m68000)
add_link_options(-Wl,--traditional-format)

add_executable(app.prg src/main.c)
