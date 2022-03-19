cmake_minimum_required(VERSION 3.10)

project(AtariSTCreativeplayground)

# Set C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# Enable all warning
add_compile_options(-Wall)
# Set linker options
add_compile_options(-m68000)
add_compile_options(-mshort)
add_link_options(-Wl,--traditional-format)
add_link_options(-nostdlib)
# Strip binary in release mode
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

include_directories(src/includes)
# add_compile_definitions(ENABLE_STDLIB)

# Add app.prg
add_executable(app.prg
    # make sure crt0 is included first
    $ENV{HOME}/libcmini-0.54/lib/crt0.o
    # then add sources
    src/linea.c
    src/main.c
    src/utils.c
    src/video.c
)
target_link_directories(app.prg
    PRIVATE $ENV{HOME}/libcmini-0.54/lib/mshort
)
target_link_libraries(app.prg
    PRIVATE cmini
    PRIVATE gcc
)
