#include "include/PlateDatabase.h"
#include "include/UI.h"
#include "include/Utils.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    PlateDatabase db;
    int choice;
    string input;
    
    UI::printWelcome();
    UI::showInfo("系统初始化完成，欢迎使用！");
    UI::pause();
    
    while (true) {
        UI::printMainMenu();
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            UI::showError("输入非法，请重新输入。");
            UI::pause();
            continue;
        }
        
        cin.ignore(); // 清除换行符
        
        switch (choice) {
            case 1: { // 手工录入
                UI::printTitle("手工录入车牌记录");
                string plate, city, owner;
                
                cout << "请输入车牌号(7位，数字和字母混排，如 01B7238)：";
                getline(cin, plate);
                
                cout << "请输入城市名：";
                getline(cin, city);
                
                cout << "请输入车主姓名：";
                getline(cin, owner);
                
                if (db.addRecord(plate, city, owner)) {
                    UI::showSuccess("添加成功！当前记录总数：" + to_string(db.getRecordCount()));
                }
                UI::pause();
                break;
            }
            
            case 2: { // 从文件导入
                UI::printTitle("从文件导入车牌记录");
                string filename = UI::getInput("请输入文件名：");
                if (db.loadFromFile(filename)) {
                    UI::showSuccess("导入完成！");
                } else {
                    UI::showError("导入失败！");
                }
                UI::pause();
                break;
            }
            
            case 3: { // 随机生成
                UI::printTitle("随机生成车牌记录");
                string countStr = UI::getInput("请输入要随机生成的记录条数：");
                int count = stoi(countStr);
                if (count > 0 && count <= 10000) {
                    db.generateRandomData(count);
                    UI::showSuccess("生成完成！");
                } else {
                    UI::showError("条数必须在1-10000之间！");
                }
                UI::pause();
                break;
            }
            
            case 4: { // 显示全部
                UI::printTitle("全部车牌记录");
                db.showAllRecords();
                UI::pause();
                break;
            }
            
            case 5: { // 修改记录
                UI::printTitle("修改车牌记录");
                string plate = UI::getInput("请输入要修改的车牌号：");
                int idx = db.findRecord(plate);
                if (idx != -1) {
                    db.showRecord(idx);
                    string newCity = UI::getInput("请输入新的城市名：");
                    string newOwner = UI::getInput("请输入新的车主姓名：");
                    if (db.modifyRecord(plate, newCity, newOwner)) {
                        UI::showSuccess("修改完成！");
                    }
                } else {
                    UI::showError("未找到该车牌号！");
                }
                UI::pause();
                break;
            }
            
            case 6: { // 删除记录
                UI::printTitle("删除车牌记录");
                string plate = UI::getInput("请输入要删除的车牌号：");
                int idx = db.findRecord(plate);
                if (idx != -1) {
                    db.showRecord(idx);
                    string confirm = UI::getInput("确认删除？(Y/N)：");
                    if (confirm == "Y" || confirm == "y") {
                        if (db.deleteRecord(plate)) {
                            UI::showSuccess("删除成功！当前记录总数：" + to_string(db.getRecordCount()));
                        }
                    } else {
                        UI::showInfo("已取消删除。");
                    }
                } else {
                    UI::showError("未找到该车牌号！");
                }
                UI::pause();
                break;
            }
            
            case 7: { // 基数排序
                UI::printTitle("链式基数排序");
                if (db.getRecordCount() == 0) {
                    UI::showError("当前无记录，无法排序！");
                } else {
                    db.radixSortByPlate();
                    UI::showSuccess("排序完成！");
                }
                UI::pause();
                break;
            }
            
            case 8: { // 折半查找
                UI::printTitle("折半查找车牌");
                string plate = UI::getInput("请输入要查找的车牌号：");
                if (!Utils::isValidPlate(plate)) {
                    UI::showError("车牌格式非法！");
                } else {
                    int idx = db.binarySearchPlate(plate);
                    if (idx != -1) {
                        UI::showSuccess("找到记录：");
                        db.showRecord(idx);
                    } else {
                        UI::showError("未找到该车牌！");
                    }
                }
                UI::pause();
                break;
            }
            
            case 9: { // 城市分块查找
                UI::printTitle("按城市分块索引查找");
                string city = UI::getInput("请输入要查找的城市名：");
                auto results = db.searchByCity(city);
                if (results.empty()) {
                    UI::showError("未找到该城市的记录！");
                } else {
                    UI::showSuccess("找到 " + to_string(results.size()) + " 条记录：");
                    cout << "\n" << string(50, '=') << endl;
                    cout << left << setw(12) << "车牌号"
                         << left << setw(12) << "城市"
                         << left << setw(15) << "车主" << endl;
                    cout << string(50, '-') << endl;
                    for (const auto& rec : results) {
                        rec.print();
                    }
                    cout << string(50, '=') << endl;
                }
                UI::pause();
                break;
            }
            
            case 10: { // 前缀查询
                UI::printTitle("车牌前缀模糊查询");
                string prefix = UI::getInput("请输入要查询的车牌前缀（1~7 位，如 01B）：");
                if (prefix.empty() || prefix.size() > 7) {
                    UI::showError("前缀长度不合法！");
                } else {
                    auto results = db.prefixSearch(prefix);
                    if (results.empty()) {
                        UI::showError("未找到以 " + prefix + " 为前缀的车牌。");
                    } else {
                        UI::showSuccess("找到 " + to_string(results.size()) + " 条记录：");
                        cout << "\n" << string(50, '=') << endl;
                        cout << left << setw(12) << "车牌号"
                             << left << setw(12) << "城市"
                             << left << setw(15) << "车主" << endl;
                        cout << string(50, '-') << endl;
                        for (const auto& rec : results) {
                            rec.print();
                        }
                        cout << string(50, '=') << endl;
                    }
                }
                UI::pause();
                break;
            }
            
            case 11: { // 统计信息
                UI::printTitle("统计信息");
                db.statistics();
                UI::pause();
                break;
            }
            
            case 12: { // 按城市统计排序
                UI::printTitle("按城市统计并排序");
                auto stats = db.getCityStatistics();
                if (stats.empty()) {
                    UI::showError("当前无记录！");
                } else {
                    cout << "\n" << string(50, '=') << endl;
                    cout << left << setw(15) << "城市" << "数量" << endl;
                    cout << string(50, '-') << endl;
                    for (const auto& p : stats) {
                        cout << left << setw(15) << p.first << p.second << endl;
                    }
                    cout << string(50, '=') << endl;
                }
                UI::pause();
                break;
            }
            
            case 13: { // 性能统计
                UI::printTitle("性能统计");
                db.showPerformanceStats();
                UI::pause();
                break;
            }
            
            case 14: { // 数据验证
                UI::printTitle("数据验证");
                db.validateData();
                UI::pause();
                break;
            }
            
            case 15: { // 保存到文件
                UI::printTitle("保存记录到文件");
                string filename = UI::getInput("请输入要保存的文件名：");
                if (db.saveToFile(filename)) {
                    UI::showSuccess("保存完成！");
                } else {
                    UI::showError("保存失败！");
                }
                UI::pause();
                break;
            }
            
            case 16: { // 导出CSV
                UI::printTitle("导出为CSV格式");
                string filename = UI::getInput("请输入CSV文件名：");
                if (db.exportToCSV(filename)) {
                    UI::showSuccess("导出完成！");
                } else {
                    UI::showError("导出失败！");
                }
                UI::pause();
                break;
            }
            
            case 17: { // 清空数据
                UI::printTitle("清空所有数据");
                string confirm = UI::getInput("确认清空所有数据？此操作不可恢复！(Y/N)：");
                if (confirm == "Y" || confirm == "y") {
                    db.clearAll();
                    UI::showSuccess("已清空所有数据。");
                } else {
                    UI::showInfo("已取消操作。");
                }
                UI::pause();
                break;
            }
            
            case 0: // 退出
                UI::printTitle("退出系统");
                cout << "感谢使用辽宁省汽车牌照快速查询系统！" << endl;
                return 0;
            
            default:
                UI::showError("无效的菜单编号，请重新输入。");
                UI::pause();
        }
    }
    
    return 0;
}
