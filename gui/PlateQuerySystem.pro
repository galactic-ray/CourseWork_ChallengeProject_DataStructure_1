QT += core widgets

CONFIG += c++11

TARGET = PlateQuerySystem
TEMPLATE = app

# 源文件
SOURCES += \
    main_gui.cpp \
    MainWindow.cpp \
    ../src/PlateDatabase.cpp \
    ../src/RadixSort.cpp \
    ../src/SearchAlgorithms.cpp \
    ../src/FileIO.cpp \
    ../src/Utils.cpp

# 头文件
HEADERS += \
    MainWindow.h \
    ../include/PlateRecord.h \
    ../include/PlateDatabase.h \
    ../include/RadixSort.h \
    ../include/SearchAlgorithms.h \
    ../include/FileIO.h \
    ../include/Utils.h

# 包含路径
INCLUDEPATH += ../include

# 输出目录
DESTDIR = ../bin
OBJECTS_DIR = ../obj/gui
MOC_DIR = ../obj/gui
RCC_DIR = ../obj/gui
UI_DIR = ../obj/gui

# 编译选项
QMAKE_CXXFLAGS += -Wall -Wextra -O2

# Windows特定设置
win32 {
    RC_ICONS = icon.ico
}

# Linux特定设置
unix {
    target.path = /usr/local/bin
    INSTALLS += target
}

