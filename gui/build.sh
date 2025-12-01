#!/bin/bash

# GUI版本编译脚本

echo "=========================================="
echo "  汽车牌照查询系统 - GUI版本编译脚本"
echo "=========================================="

# 检查qmake
if ! command -v qmake &> /dev/null; then
    echo "错误: 未找到qmake，请先安装Qt开发库"
    echo "Ubuntu/Debian: sudo apt-get install qt5-default qtbase5-dev"
    echo "Fedora: sudo dnf install qt5-qtbase-devel"
    exit 1
fi

# 显示Qt版本
echo "Qt版本:"
qmake --version

# 创建输出目录
mkdir -p ../bin
mkdir -p ../obj/gui

# 清理旧文件
echo ""
echo "清理旧文件..."
make clean 2>/dev/null || true

# 生成Makefile
echo ""
echo "生成Makefile..."
qmake PlateQuerySystem.pro

# 编译
echo ""
echo "开始编译..."
make -j$(nproc)

# 检查编译结果
if [ $? -eq 0 ]; then
    echo ""
    echo "=========================================="
    echo "  编译成功！"
    echo "=========================================="
    echo "可执行文件位置:"
    if [ -f "../bin/PlateQuerySystem" ]; then
        echo "  ../bin/PlateQuerySystem"
    elif [ -f "PlateQuerySystem" ]; then
        echo "  ./PlateQuerySystem"
    fi
    echo ""
    echo "运行方法:"
    echo "  cd .. && ./bin/PlateQuerySystem"
    echo "  或"
    echo "  ./PlateQuerySystem"
else
    echo ""
    echo "=========================================="
    echo "  编译失败！"
    echo "=========================================="
    exit 1
fi

