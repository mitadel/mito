cmake_minimum_required(VERSION 2.8.12)
project(mito)

set(CMAKE_CXX_STANDARD 20)

find_package(pybind11)
pybind11_add_module(mito mito.cc)