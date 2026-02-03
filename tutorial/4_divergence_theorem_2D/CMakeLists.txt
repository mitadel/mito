# cmake setup
cmake_minimum_required(VERSION 3.25)

# the template app project
project(template)

# find {mito}
find_package(mito)

# schedule main file for compilation
add_executable(main "main.cc")

# additional compilation flags
set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always -Wall -Wextra -pedantic -Werror -Wno-psabi")

# link against {mito}
target_link_libraries(main mito::mito)