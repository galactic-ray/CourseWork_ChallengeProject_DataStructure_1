#include "../include/Utils.h"
#include <random>
#include <algorithm>

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
    
    bool isValidPlate(const std::string& plate) {
        if (plate.size() != 7) return false;
        for (char c : plate) {
            if (!std::isdigit(static_cast<unsigned char>(c)) && 
                !(c >= 'A' && c <= 'Z')) {
                return false;
            }
        }
        return true;
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
        static std::uniform_int_distribution<int> alphaDist(0, 25);
        
        std::string s(7, '0');
        for (int i = 0; i < 7; ++i) {
            int t = digitDist(gen) % 2;
            if (t == 0) {
                s[i] = char('0' + digitDist(gen));
            } else {
                s[i] = char('A' + alphaDist(gen));
            }
        }
        return s;
    }
}

