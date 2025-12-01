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
    
    // 车牌合法性校验：长度固定 7，只含数字和大写字母
    bool isValidPlate(const std::string& plate);
    
    // 简单的分割函数（按空格）
    std::vector<std::string> split(const std::string& line);
    
    // 生成随机车牌号
    std::string generateRandomPlate();
}

#endif // UTILS_H

