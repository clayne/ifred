# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_ARCHITECTURES x86_64)

project ("palette")
set(BUILD_EXAMPLES OFF CACHE BOOL "")

add_subdirectory(palette/pybind11)

# Include sub-projects.
add_subdirectory ("palette")

if (${BUILD_EXAMPLES})
    add_subdirectory ("standalone")
endif ()
