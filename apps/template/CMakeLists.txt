# cmake setup
cmake_minimum_required(VERSION 3.12)

# the template app project
project(template)

# find {mito}
find_package(mito)

# schedule main file for compilation
add_executable(main "main.cc")

# link against {mito}
target_link_libraries(main mito::mito)