cmake_minimum_required(VERSION 3.18)
project(tumourmodel LANGUAGES CXX)

# pybind11 from PyPI provides a CMake config
find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module)
find_package(pybind11 CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

pybind11_add_module(_core
    src/tumourmodel/_core.cpp
    src/tumourmodel/cpp/Model.cpp
)

install(TARGETS _core DESTINATION tumourmodel)

target_include_directories(_core PRIVATE src/tumourmodel/cpp)
# Optimizations (safe defaults)
if (MSVC)
  target_compile_options(_core PRIVATE /O2 /EHsc)
else()
  target_compile_options(_core PRIVATE -O3)
endif()
