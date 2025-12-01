# GUI界面使用说明

## 编译GUI版本

### 方法1：使用Qt Creator（推荐）

1. 打开Qt Creator
2. 打开项目：`File -> Open File or Project`
3. 选择 `gui/PlateQuerySystem.pro` 文件
4. 配置Kit（选择Qt版本和编译器）
5. 点击 `Build -> Build Project` 或按 `Ctrl+B`

### 方法2：使用qmake命令行

```bash
cd gui
qmake PlateQuerySystem.pro
make
```

### 方法3：使用CMake

```bash
cd gui
mkdir build
cd build
cmake ..
make
```

## 运行GUI程序

编译完成后，可执行文件位于：
- Qt Creator: `build-PlateQuerySystem-.../PlateQuerySystem`
- qmake: `../bin/PlateQuerySystem` 或当前目录
- CMake: `build/bin/PlateQuerySystem`

直接运行可执行文件即可。

## 界面功能说明

### 左侧面板

1. **数据录入**
   - 输入车牌号、城市、车主信息
   - 点击"添加记录"添加新记录
   - 点击"修改记录"修改现有记录
   - 点击"删除记录"删除记录
   - "从文件导入"：从文本文件导入数据
   - "随机生成"：生成测试数据

2. **查找功能**
   - 折半查找：输入完整车牌号进行精确查找
   - 前缀查找：输入车牌前缀进行模糊查找
   - 城市查找：选择或输入城市名查找该城市所有车牌

3. **操作功能**
   - 链式基数排序：对车牌进行排序
   - 统计信息：显示总数和城市统计
   - 城市统计排序：按城市车牌数量排序显示
   - 性能统计：显示操作和性能数据
   - 数据验证：验证数据完整性

4. **文件操作**
   - 保存到文件：保存为文本格式
   - 导出CSV：导出为CSV格式
   - 清空数据：清空所有记录

### 中间面板

- **数据表格**：显示所有车牌记录
  - 双击记录可快速填充到输入框
  - 选中记录后可以修改或删除
  - 支持排序（点击列标题）

### 右侧面板

- **系统信息**：显示操作日志和系统信息

### 菜单栏

- **文件**：导入、保存、导出、退出
- **编辑**：添加、修改、删除、清空
- **查找**：各种查找功能
- **工具**：排序、统计、性能分析
- **帮助**：使用帮助和关于信息

## 使用技巧

1. **快速编辑**：双击表格中的记录，信息会自动填充到输入框
2. **批量操作**：可以先导入文件，再进行批量操作
3. **查找高亮**：查找成功后，表格会自动滚动到找到的记录并高亮显示
4. **数据验证**：在保存前建议先进行数据验证

## 注意事项

1. 车牌号必须为7位，只包含数字和大写字母
2. 折半查找需要先对数据进行排序
3. 城市查找需要先建立索引（系统会自动建立）
4. 删除和清空操作不可恢复，请谨慎操作

## 系统要求

- Qt 5.9 或更高版本
- C++11 编译器
- Linux/macOS/Windows

## 故障排除

### 编译错误

1. 确保已安装Qt开发库
   ```bash
   # Ubuntu/Debian
   sudo apt-get install qt5-default qtbase5-dev
   
   # Fedora
   sudo dnf install qt5-qtbase-devel
   ```

2. 检查Qt版本
   ```bash
   qmake --version
   ```

3. 清理后重新编译
   ```bash
   make clean
   qmake
   make
   ```

### 运行时错误

1. 如果提示找不到Qt库，设置LD_LIBRARY_PATH（Linux）
   ```bash
   export LD_LIBRARY_PATH=/usr/lib/qt5/lib:$LD_LIBRARY_PATH
   ```

2. 检查Qt插件路径
   ```bash
   export QT_PLUGIN_PATH=/usr/lib/qt5/plugins
   ```

