#ifndef UI_H
#define UI_H

#include <string>

/**
 * 用户界面模块
 * 提供美观的控制台界面
 */
class UI {
public:
    /**
     * 显示主菜单
     */
    static void printMainMenu();
    
    /**
     * 显示欢迎界面
     */
    static void printWelcome();
    
    /**
     * 显示分隔线
     */
    static void printSeparator(char ch = '=', int len = 60);
    
    /**
     * 显示标题
     */
    static void printTitle(const std::string& title);
    
    /**
     * 清屏（跨平台）
     */
    static void clearScreen();
    
    /**
     * 暂停等待用户输入
     */
    static void pause();
    
    /**
     * 获取用户输入（带提示）
     */
    static std::string getInput(const std::string& prompt);
    
    /**
     * 显示错误信息
     */
    static void showError(const std::string& message);
    
    /**
     * 显示成功信息
     */
    static void showSuccess(const std::string& message);
    
    /**
     * 显示信息
     */
    static void showInfo(const std::string& message);
};

#endif // UI_H

