cmake_minimum_required(VERSION 3.10)

project(AtariSTCreativeplayground)

# Set globals options ==============================================================================

# Set C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# Enable all warning
add_compile_options(-Wall)

# Strip binary in release mode
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

# include
include_directories(src/includes)

# add_compile_definitions(ENABLE_STDLIB)

# Add app.prg ======================================================================================
if(CMAKE_C_COMPILER_VERSION VERSION_LESS 6.3) # m68k-atari-ming-gcc is 4.3

add_executable(app.prg
    # make sure crt0 is included first
    $ENV{HOME}/libcmini-0.54/lib/crt0.o
    # then add sources
    src/linea.c
    src/main.c
    src/matrix.c
    src/utils.c
    src/video.c
)

# Set "app.prg" target m68x CPU with short int
target_compile_options(app.prg PRIVATE -m68000)
target_compile_options(app.prg PRIVATE -mshort)
target_link_options(app.prg    PRIVATE -Wl,--traditional-format)

# Set "app.prg" build without stdlib and use "libcmini" instead
target_link_libraries(app.prg
    PRIVATE cmini
    PRIVATE gcc
)
target_link_directories(app.prg
    PRIVATE $ENV{HOME}/libcmini-0.54/lib/mshort
)
target_link_options(app.prg PRIVATE -nostdlib)

endif()

# Add tests ========================================================================================
include(CTest)

add_subdirectory(tests)
