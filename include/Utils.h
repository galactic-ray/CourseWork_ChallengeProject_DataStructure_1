#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <cctype>

/**
 * 工具函数集合
 */
namespace Utils {
    // 将字符映射到桶编号：0-9 -> 0-9, A-Z -> 10-35
    int charToBucketIndex(char c);
    
    // 将字符串统一转换为大写
    std::string toUpperStr(const std::string& s);
    
    // 车牌合法性校验
    // 燃油车：辽 + 字母(A-Z,排除I和O) + 5位编号(数字和字母,排除I和O)
    // 新能源车：辽 + 字母(A-Z,排除I和O) + D/F(纯电/插混) + 6位编号(数字和字母,排除I和O)
    // 目前系统仍以辽宁省为例，因此省份简称固定为“辽”
    bool isValidPlate(const std::string& plate);
    
    // 判断是否为新能源车牌（在 isValidPlate 通过的前提下，再区分燃油/新能源）
    bool isNewEnergyPlate(const std::string& plate);
    
    // 根据车牌获取车辆类别："油车" 或 "电车"（无效车牌返回空字符串）
    std::string getPlateCategory(const std::string& plate);
    
    // 检查字符是否为有效的车牌编号字符（数字或字母，排除I和O）
    bool isValidPlateChar(char c);
    
    // 从车牌中提取发牌机关代码字母（第2个字符，跳过省份简称）
    char extractPlateLetter(const std::string& plate);
    
    // 简单的分割函数（按空格）
    std::vector<std::string> split(const std::string& line);
    
    // 生成随机车牌号
    std::string generateRandomPlate();
    
    // 根据城市生成随机车牌号（车牌字母与城市对应）
    std::string generateRandomPlateByCity(const std::string& city);
    
    // 根据城市生成随机新能源车牌号（车牌字母与城市对应，类型为 D/F）
    std::string generateRandomNewEnergyPlateByCity(const std::string& city);
    
    // 根据车牌字母获取对应城市（辽宁省）
    // 返回城市名，如果字母无效返回空字符串
    std::string getCityByPlateLetter(char letter);
    
    // 根据城市名获取对应的车牌字母
    // 返回字母，如果城市无效返回'\0'
    char getPlateLetterByCity(const std::string& city);
    
    // 验证车牌字母和城市是否匹配
    bool validatePlateCityMatch(const std::string& plate, const std::string& city);
    
    // 获取所有有效的车牌字母
    std::vector<char> getValidPlateLetters();
    
    // 获取所有城市列表
    std::vector<std::string> getAllCities();
}

#endif // UTILS_H

