cmake_minimum_required(VERSION 3.10)
project(PlateQuerySystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 全局包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 核心源文件（供 CLI、GUI、测试复用）
set(CORE_SOURCES
    src/FileIO.cpp
    src/PlateDatabase.cpp
    src/RadixSort.cpp
    src/SearchAlgorithms.cpp
    src/UI.cpp
    src/Utils.cpp
)

add_library(platecore ${CORE_SOURCES})
target_include_directories(platecore PUBLIC ${CMAKE_SOURCE_DIR}/include)

# ---- CLI 程序 ----
add_executable(plate_cli main.cpp)
target_link_libraries(plate_cli platecore)

# ---- 测试程序 ----
add_executable(plate_tests tests/test.cpp)
target_link_libraries(plate_tests platecore)

# ---- GUI 程序 ----
add_subdirectory(gui)

# 安装指引（可选）
install(TARGETS plate_cli plate_tests
        RUNTIME DESTINATION bin)

