#include "../include/Utils.h"
#include <random>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <cstring>

namespace Utils {
    int charToBucketIndex(char c) {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
        return 0; // 不合法字符，归到 0 桶
    }
    
    std::string toUpperStr(const std::string& s) {
        std::string result = s;
        for (auto& ch : result) {
            ch = std::toupper(static_cast<unsigned char>(ch));
        }
        return result;
    }
    
    // 检查字符是否为有效的车牌编号字符（数字或字母，排除I和O）
    bool isValidPlateChar(char c) {
        if (c >= '0' && c <= '9') return true;
        if (c >= 'A' && c <= 'Z' && c != 'I' && c != 'O') return true;
        return false;
    }
    
    bool isValidPlate(const std::string& plate) {
        // 目前仅支持辽宁省车牌：
        // 燃油车：辽 + 字母 + 5位编号  -> UTF-8 总字节数 3 + 1 + 5 = 9
        // 新能源：辽 + 字母 + D/F + 5位编号 -> UTF-8 总字节数 3 + 1 + 1 + 5 = 10
        
        // 检查省份简称"辽"（UTF-8编码：0xE8 0xBE 0xBD）
        const unsigned char liao_utf8[] = {0xE8, 0xBE, 0xBD};
        if (plate.size() < 3 || 
            static_cast<unsigned char>(plate[0]) != liao_utf8[0] ||
            static_cast<unsigned char>(plate[1]) != liao_utf8[1] ||
            static_cast<unsigned char>(plate[2]) != liao_utf8[2]) {
            return false;
        }
        
        // 燃油车：总长度 9 字节
        if (plate.size() == 9) {
            // 第2段：1位字母（发牌机关代码），不能是I和O
            char letter = plate[3];
            if (letter < 'A' || letter > 'Z' || letter == 'I' || letter == 'O') {
                return false;
            }
            
            // 第3段：5位编号，由数字和字母组成，不能有I和O
            for (size_t i = 4; i < 9; ++i) {
                if (!isValidPlateChar(plate[i])) {
                    return false;
                }
            }
            
            return true;
        }
        
        // 新能源车：总长度 10 字节
        if (plate.size() == 10) {
            // 第2段：1位字母（发牌机关代码），不能是I和O
            char letter = plate[3];
            if (letter < 'A' || letter > 'Z' || letter == 'I' || letter == 'O') {
                return false;
            }
            
            // 第3段：能源类型 D/F（纯电/插混）
            char energyType = plate[4];
            if (energyType != 'D' && energyType != 'F') {
                return false;
            }
            
            // 第4段：5位编号，由数字和字母组成，不能有I和O
            for (size_t i = 5; i < 10; ++i) {
                if (!isValidPlateChar(plate[i])) {
                    return false;
                }
            }
            
            return true;
        }
        
        // 其他长度一律视为非法
        return false;
    }
    
    bool isNewEnergyPlate(const std::string& plate) {
        if (!isValidPlate(plate)) {
            return false;
        }
        // 新能源车牌长度为 10，且第 5 个字节为 D/F
        if (plate.size() != 10) {
            return false;
        }
        char energyType = plate[4];
        return energyType == 'D' || energyType == 'F';
    }
    
    std::string getPlateCategory(const std::string& plate) {
        if (!isValidPlate(plate)) {
            return "";
        }
        return isNewEnergyPlate(plate) ? "电车" : "油车";
    }
    
    // 从车牌中提取发牌机关代码字母（第2个字符，跳过省份简称）
    char extractPlateLetter(const std::string& plate) {
        // "辽"在UTF-8中占3个字节，所以字母在第4个字节（索引3）
        if (plate.size() >= 4) {
            return plate[3];
        }
        return '\0';
    }
    
    std::vector<std::string> split(const std::string& line) {
        std::vector<std::string> res;
        std::string cur;
        for (char c : line) {
            if (std::isspace(static_cast<unsigned char>(c))) {
                if (!cur.empty()) {
                    res.push_back(cur);
                    cur.clear();
                }
            } else {
                cur.push_back(c);
            }
        }
        if (!cur.empty()) res.push_back(cur);
        return res;
    }
    
    std::string generateRandomPlate() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> digitDist(0, 9);
        
        // 有效字母：A-H, J-N, P-Z (排除I和O)
        static std::vector<char> validLetters = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
            'J', 'K', 'L', 'M', 'N',
            'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
        };
        static std::uniform_int_distribution<int> letterDist(0, static_cast<int>(validLetters.size()) - 1);
        static std::uniform_int_distribution<int> charTypeDist(0, 1); // 0=数字, 1=字母
        
        // 辽宁省车牌格式：辽 + 字母 + 5位编号
        // UTF-8编码："辽" = 0xE8 0xBE 0xBD
        std::string plate;
        plate.resize(9);
        
        // 第1段：省份简称"辽"（UTF-8编码）
        plate[0] = static_cast<char>(0xE8);
        plate[1] = static_cast<char>(0xBE);
        plate[2] = static_cast<char>(0xBD);
        
        // 第2段：1位字母（发牌机关代码），不能是I和O
        plate[3] = validLetters[letterDist(gen)];
        
        // 第3段：5位编号，由数字和字母组成，不能有I和O
        for (int i = 4; i < 9; ++i) {
            int charType = charTypeDist(gen);
            if (charType == 0) {
                // 数字
                plate[i] = char('0' + digitDist(gen));
            } else {
                // 字母（排除I和O）
                plate[i] = validLetters[letterDist(gen)];
            }
        }
        
        return plate;
    }
    
    // 根据城市生成随机车牌号（车牌字母与城市对应，燃油车）
    std::string generateRandomPlateByCity(const std::string& city) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> digitDist(0, 9);
        
        // 有效字母：A-H, J-N, P-Z (排除I和O)
        static std::vector<char> validLetters = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
            'J', 'K', 'L', 'M', 'N',
            'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
        };
        static std::uniform_int_distribution<int> charTypeDist(0, 1); // 0=数字, 1=字母
        
        // 根据城市获取对应的车牌字母
        char plateLetter = getPlateLetterByCity(city);
        if (plateLetter == '\0') {
            // 如果城市无效，使用随机字母
            static std::uniform_int_distribution<int> letterDist(0, static_cast<int>(validLetters.size()) - 1);
            plateLetter = validLetters[letterDist(gen)];
        }
        
        // 辽宁省燃油车车牌格式：辽 + 字母 + 5位编号
        std::string plate;
        plate.resize(9);
        
        // 第1段：省份简称"辽"（UTF-8编码）
        plate[0] = static_cast<char>(0xE8);
        plate[1] = static_cast<char>(0xBE);
        plate[2] = static_cast<char>(0xBD);
        
        // 第2段：使用城市对应的字母
        plate[3] = plateLetter;
        
        // 第3段：5位编号，由数字和字母组成，不能有I和O
        static std::uniform_int_distribution<int> letterDist(0, static_cast<int>(validLetters.size()) - 1);
        for (int i = 4; i < 9; ++i) {
            int charType = charTypeDist(gen);
            if (charType == 0) {
                // 数字
                plate[i] = char('0' + digitDist(gen));
            } else {
                // 字母（排除I和O）
                plate[i] = validLetters[letterDist(gen)];
            }
        }
        
        return plate;
    }
    
    // 根据城市生成随机新能源车牌号（车牌字母与城市对应，类型 D/F）
    std::string generateRandomNewEnergyPlateByCity(const std::string& city) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> digitDist(0, 9);
        
        // 有效字母：A-H, J-N, P-Z (排除I和O)
        static std::vector<char> validLetters = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
            'J', 'K', 'L', 'M', 'N',
            'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
        };
        static std::uniform_int_distribution<int> charTypeDist(0, 1); // 0=数字, 1=字母
        static std::uniform_int_distribution<int> energyDist(0, 1);   // 0=D, 1=F
        
        // 根据城市获取对应的车牌字母
        char plateLetter = getPlateLetterByCity(city);
        if (plateLetter == '\0') {
            static std::uniform_int_distribution<int> letterDist(0, static_cast<int>(validLetters.size()) - 1);
            plateLetter = validLetters[letterDist(gen)];
        }
        
        // 新能源车牌格式：辽 + 字母 + D/F + 5位编号
        std::string plate;
        plate.resize(10);
        
        // 第1段：省份简称"辽"（UTF-8编码）
        plate[0] = static_cast<char>(0xE8);
        plate[1] = static_cast<char>(0xBE);
        plate[2] = static_cast<char>(0xBD);
        
        // 第2段：使用城市对应的字母
        plate[3] = plateLetter;
        
        // 第3段：能源类型 D/F
        plate[4] = energyDist(gen) == 0 ? 'D' : 'F';
        
        // 第4段：5位编号，由数字和字母组成，不能有I和O
        static std::uniform_int_distribution<int> letterDist(0, static_cast<int>(validLetters.size()) - 1);
        for (int i = 5; i < 10; ++i) {
            int charType = charTypeDist(gen);
            if (charType == 0) {
                plate[i] = char('0' + digitDist(gen));
            } else {
                plate[i] = validLetters[letterDist(gen)];
            }
        }
        
        return plate;
    }
    
    // 辽宁省车牌字母与城市映射表
    static const std::map<char, std::string> PLATE_LETTER_TO_CITY = {
        {'A', "沈阳"}, {'B', "大连"}, {'C', "鞍山"}, {'D', "抚顺"},
        {'E', "本溪"}, {'F', "丹东"}, {'G', "锦州"}, {'H', "营口"},
        {'J', "阜新"}, {'K', "辽阳"}, {'L', "盘锦"}, {'M', "铁岭"},
        {'N', "朝阳"}, {'P', "葫芦岛"}
        // 注意：没有I和O，因为容易与数字1和0混淆
    };
    
    static const std::unordered_map<std::string, char> CITY_TO_PLATE_LETTER = {
        {"沈阳", 'A'}, {"大连", 'B'}, {"鞍山", 'C'}, {"抚顺", 'D'},
        {"本溪", 'E'}, {"丹东", 'F'}, {"锦州", 'G'}, {"营口", 'H'},
        {"阜新", 'J'}, {"辽阳", 'K'}, {"盘锦", 'L'}, {"铁岭", 'M'},
        {"朝阳", 'N'}, {"葫芦岛", 'P'}
    };
    
    std::string getCityByPlateLetter(char letter) {
        letter = std::toupper(static_cast<unsigned char>(letter));
        auto it = PLATE_LETTER_TO_CITY.find(letter);
        if (it != PLATE_LETTER_TO_CITY.end()) {
            return it->second;
        }
        return "";
    }
    
    char getPlateLetterByCity(const std::string& city) {
        auto it = CITY_TO_PLATE_LETTER.find(city);
        if (it != CITY_TO_PLATE_LETTER.end()) {
            return it->second;
        }
        return '\0';
    }
    
    bool validatePlateCityMatch(const std::string& plate, const std::string& city) {
        if (plate.empty() || city.empty()) {
            return false;
        }
        
        // 从车牌中提取发牌机关代码字母（跳过省份简称"辽"）
        char plateLetter = extractPlateLetter(plate);
        
        if (plateLetter == '\0') {
            return false; // 无法提取字母
        }
        
        // 获取车牌字母对应的城市
        std::string expectedCity = getCityByPlateLetter(plateLetter);
        if (expectedCity.empty()) {
            return false; // 无效的车牌字母
        }
        
        // 比较城市名
        return expectedCity == city;
    }
    
    std::vector<char> getValidPlateLetters() {
        std::vector<char> letters;
        for (const auto& pair : PLATE_LETTER_TO_CITY) {
            letters.push_back(pair.first);
        }
        return letters;
    }
    
    std::vector<std::string> getAllCities() {
        std::vector<std::string> cities;
        for (const auto& pair : PLATE_LETTER_TO_CITY) {
            cities.push_back(pair.second);
        }
        return cities;
    }
}

