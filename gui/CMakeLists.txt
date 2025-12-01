cmake_minimum_required(VERSION 3.10)
project(PlateQuerySystemGUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt5
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

# 设置Qt5的MOC、UIC、RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# GUI 源文件
set(GUI_SOURCES
    main_gui.cpp
    MainWindow.cpp
)

set(GUI_HEADERS
    MainWindow.h
)

add_executable(PlateQuerySystem ${GUI_SOURCES} ${GUI_HEADERS})

target_include_directories(PlateQuerySystem PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 链接 Qt 和核心库
target_link_libraries(PlateQuerySystem PRIVATE Qt5::Core Qt5::Widgets platecore)

set_target_properties(PlateQuerySystem PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

