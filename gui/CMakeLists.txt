cmake_minimum_required(VERSION 3.10)
project(PlateQuerySystem)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt5
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

# 设置Qt5的MOC、UIC、RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 包含目录
include_directories(../include)

# 源文件
set(SOURCES
    main_gui.cpp
    MainWindow.cpp
    ../src/PlateDatabase.cpp
    ../src/RadixSort.cpp
    ../src/SearchAlgorithms.cpp
    ../src/FileIO.cpp
    ../src/Utils.cpp
)

# 头文件
set(HEADERS
    MainWindow.h
    ../include/PlateRecord.h
    ../include/PlateDatabase.h
    ../include/RadixSort.h
    ../include/SearchAlgorithms.h
    ../include/FileIO.h
    ../include/Utils.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 链接Qt5库
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets)

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

