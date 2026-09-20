cmake_minimum_required(VERSION 3.14)
project(signal_engine_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

include(FetchContent)
FetchContent_Declare(
    pybind11
    URL https://github.com/pybind/pybind11/archive/refs/tags/v2.12.0.zip
)
FetchContent_MakeAvailable(pybind11)

pybind11_add_module(
    signal_engine
    src/telemetry_engine.cpp
    src/bindings.cpp
)

if(WIN32 AND MINGW)
    target_link_libraries(signal_engine PRIVATE -static-libgcc -static-libstdc++)
endif()