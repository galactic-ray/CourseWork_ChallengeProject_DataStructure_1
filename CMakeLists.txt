cmake_minimum_required(VERSION 3.10)
project(PlateQuerySystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 全局包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 核心源文件（供 GUI、测试复用）
set(CORE_SOURCES
    src/FileIO.cpp
    src/PlateDatabase.cpp
    src/RadixSort.cpp
    src/SearchAlgorithms.cpp
    src/Utils.cpp
)

add_library(platecore ${CORE_SOURCES})
target_include_directories(platecore PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(gui)

