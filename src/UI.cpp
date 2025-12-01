#include "../include/UI.h"
#include <iostream>
#include <limits>
#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

void UI::printMainMenu() {
    clearScreen();
    printWelcome();
    printSeparator('=', 60);
    std::cout << "\n";
    std::cout << "  【数据录入】\n";
    std::cout << "    1. 手工录入车牌记录\n";
    std::cout << "    2. 从文件导入车牌记录\n";
    std::cout << "    3. 随机生成车牌记录\n";
    std::cout << "\n";
    std::cout << "  【数据管理】\n";
    std::cout << "    4. 显示全部记录\n";
    std::cout << "    5. 修改记录\n";
    std::cout << "    6. 删除记录\n";
    std::cout << "\n";
    std::cout << "  【排序与查找】\n";
    std::cout << "    7. 使用链式基数排序对车牌排序\n";
    std::cout << "    8. 折半查找车牌\n";
    std::cout << "    9. 按城市分块索引查找\n";
    std::cout << "   10. 车牌前缀模糊查询（高级功能）\n";
    std::cout << "\n";
    std::cout << "  【统计与分析】\n";
    std::cout << "   11. 统计信息（总数及各城市数量）\n";
    std::cout << "   12. 按城市统计并排序\n";
    std::cout << "   13. 性能统计\n";
    std::cout << "   14. 数据验证\n";
    std::cout << "\n";
    std::cout << "  【文件操作】\n";
    std::cout << "   15. 保存记录到文件\n";
    std::cout << "   16. 导出为CSV格式\n";
    std::cout << "\n";
    std::cout << "  【系统维护】\n";
    std::cout << "   17. 清空所有数据\n";
    std::cout << "    0. 退出系统\n";
    printSeparator('=', 60);
    std::cout << "请输入菜单编号：";
}

void UI::printWelcome() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                      ║\n";
    std::cout << "║      辽宁省汽车牌照快速查询系统 v2.0                ║\n";
    std::cout << "║      Liaoning Province Vehicle Plate Query System   ║\n";
    std::cout << "║                                                      ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
}

void UI::printSeparator(char ch, int len) {
    std::cout << std::string(len, ch) << std::endl;
}

void UI::printTitle(const std::string& title) {
    int len = static_cast<int>(title.length()) + 4;
    printSeparator('=', len);
    std::cout << "  " << title << std::endl;
    printSeparator('=', len);
}

void UI::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void UI::pause() {
    std::cout << "\n按回车键继续...";
    std::cin.ignore();
    std::cin.get();
}

std::string UI::getInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void UI::showError(const std::string& message) {
    std::cout << "\n[错误] " << message << std::endl;
}

void UI::showSuccess(const std::string& message) {
    std::cout << "\n[成功] " << message << std::endl;
}

void UI::showInfo(const std::string& message) {
    std::cout << "\n[信息] " << message << std::endl;
}

