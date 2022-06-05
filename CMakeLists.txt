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

# M68K build options ===============================================================================

if(CMAKE_C_COMPILER_VERSION VERSION_LESS 6.3) # m68k-atari-ming-gcc is 4.3

    # Set target m68x CPU with short int
    add_compile_options(-m68000)
    add_compile_options(-mshort)
    add_link_options(-Wl,--traditional-format)

    # Set build without stdlib and use "libcmini" instead
    link_directories($ENV{HOME}/libcmini-0.54/lib/mshort)
    add_link_options(-nostdlib)
    link_libraries(cmini gcc)

endif()

# User option ======================================================================================

# add_compile_definitions(ENABLE_STDLIB)
# add_compile_definitions(ENABLE_FAST_CALL)

# add_compile_definitions(USE_PROJECTION_ORTHOGRAPHIC)

# Add common library ===============================================================================

if(CMAKE_C_COMPILER_VERSION VERSION_LESS 6.3) # m68k-atari-ming-gcc is 4.3

    add_library(atari_common STATIC
        # add sources
        src/linea.c
        src/matrix.c
        src/trigo.c
        src/utils.c
        src/video.c
    )

endif()

# Add cube.prg =====================================================================================
if(CMAKE_C_COMPILER_VERSION VERSION_LESS 6.3) # m68k-atari-ming-gcc is 4.3

    add_executable(cube.prg
        # make sure crt0 is included first
        $ENV{HOME}/libcmini-0.54/lib/crt0.o
        # then add sources
        src/scenes/cube.c
    )

    target_link_libraries(cube.prg PRIVATE atari_common)

endif()

# Add tests ========================================================================================
include(CTest)

add_subdirectory(tests)
