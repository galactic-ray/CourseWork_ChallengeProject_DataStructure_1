# 编译和运行指令

## 方法一：使用 CMake 构建（推荐）

### 1. 创建构建目录并编译

```bash
# 进入项目目录
cd /home/syh/CourseWork_ChallengeProject_DataStructure_1

# 创建 build 目录
mkdir -p build
cd build

# 运行 CMake 配置
cmake ..

# 编译项目
make

# 或者使用多线程编译（更快）
make -j4
```

### 2. 运行程序

```bash
# 在 build 目录下运行
./bin/PlateQuerySystem

# 或者从项目根目录运行
cd /home/syh/CourseWork_ChallengeProject_DataStructure_1
./build/bin/PlateQuerySystem
```

## 方法二：一键编译和运行脚本

```bash
# 在项目根目录执行
cd /home/syh/CourseWork_ChallengeProject_DataStructure_1
mkdir -p build && cd build && cmake .. && make -j4 && ./bin/PlateQuerySystem
```

## 清理构建文件

```bash
# 删除 build 目录
rm -rf build

# 或者只清理编译文件，保留 CMake 配置
cd build
make clean
```

## 依赖要求

- CMake 3.10 或更高版本
- Qt5 (Core, Widgets)
- C++11 或更高版本的编译器 (g++/clang++)

## 检查依赖

```bash
# 检查 CMake 版本
cmake --version

# 检查 Qt5 是否安装
pkg-config --modversion Qt5Core

# 检查编译器
g++ --version
```
