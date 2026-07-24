# cmake setup
cmake_minimum_required(VERSION 3.25)

# the cg_chapter5_verification app project
project(cg_chapter5_verification)

# find {mito}
find_package(mito)

# schedule main file for compilation
add_executable(main "main.cc")

# link against {mito}
target_link_libraries(main mito::mito)
