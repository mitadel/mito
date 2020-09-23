cmake_minimum_required(VERSION 2.8.12)
project(example)

find_package(pybind11)
pybind11_add_module(example example.cc)