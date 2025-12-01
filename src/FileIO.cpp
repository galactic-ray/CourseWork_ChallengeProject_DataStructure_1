#include "../include/FileIO.h"
#include "../include/Utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

// 文本格式加载：每行“车牌 城市 车主”
static bool loadTextFile(const std::string& filename,
                         std::vector<PlateRecord>& records) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "无法打开文件：" << filename << std::endl;
        return false;
    }
    
    int count = 0;
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        
        auto parts = Utils::split(line);
        if (parts.size() < 2) continue;
        
        PlateRecord rec;
        rec.plate = Utils::toUpperStr(parts[0]);
        rec.city = parts[1];
        rec.owner = (parts.size() >= 3) ? parts[2] : "未知";
        
        if (!Utils::isValidPlate(rec.plate)) {
            std::cout << "跳过非法车牌：" << rec.plate << std::endl;
            continue;
        }
        
        // 根据车牌推导车辆类别（油车/电车）
        rec.category = Utils::getPlateCategory(rec.plate);
        
        records.push_back(rec);
        count++;
    }
    
    fin.close();
    std::cout << "从文本文件读取完成，成功导入 " << count << " 条记录。" << std::endl;
    return true;
}

// CSV 格式加载：支持带表头“车牌号,城市,车主”
static bool loadCSVFile(const std::string& filename,
                        std::vector<PlateRecord>& records) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "无法打开CSV文件：" << filename << std::endl;
        return false;
    }
    
    int count = 0;
    std::string line;
    bool isFirstLine = true;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> parts;
        while (std::getline(ss, cell, ',')) {
            // 去掉首尾空白
            size_t start = cell.find_first_not_of(" \t\r\n");
            size_t end = cell.find_last_not_of(" \t\r\n");
            if (start == std::string::npos) {
                parts.emplace_back("");
            } else {
                parts.emplace_back(cell.substr(start, end - start + 1));
            }
        }
        
        if (parts.size() < 2) continue;
        
        // 若是首行且第一列不是合法车牌，则视为表头直接跳过
        if (isFirstLine) {
            isFirstLine = false;
            std::string possiblePlate = Utils::toUpperStr(parts[0]);
            if (!Utils::isValidPlate(possiblePlate)) {
                continue;
            }
        }
        
        PlateRecord rec;
        rec.plate = Utils::toUpperStr(parts[0]);
        rec.city = parts[1];
        rec.owner = (parts.size() >= 3 && !parts[2].empty()) ? parts[2] : "未知";
        
        if (!Utils::isValidPlate(rec.plate)) {
            std::cout << "跳过非法车牌：" << rec.plate << std::endl;
            continue;
        }
        
        // 根据车牌推导车辆类别（油车/电车），忽略或纠正文件中潜在的不一致
        rec.category = Utils::getPlateCategory(rec.plate);
        
        records.push_back(rec);
        count++;
    }
    
    fin.close();
    std::cout << "从CSV文件读取完成，成功导入 " << count << " 条记录。" << std::endl;
    return true;
}

bool FileIO::loadFromFile(const std::string& filename, 
                         std::vector<PlateRecord>& records) {
    // 根据扩展名自动判断文本/CSV
    std::string lowerName = filename;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                   [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
    
    if (lowerName.size() >= 4 && lowerName.substr(lowerName.size() - 4) == ".csv") {
        return loadCSVFile(filename, records);
    }
    
    // 默认按文本格式处理
    return loadTextFile(filename, records);
}

bool FileIO::saveToFile(const std::string& filename,
                       const std::vector<PlateRecord>& records) {
    std::ofstream fout(filename);
    if (!fout.is_open()) {
        std::cerr << "无法创建文件：" << filename << std::endl;
        return false;
    }
    
    for (const auto& rec : records) {
        // 文本格式增加“类别”字段，位于车主之后
        fout << rec.plate << " " << rec.city << " " << rec.owner << " " << rec.category << "\n";
    }
    
    fout.close();
    std::cout << "已保存 " << records.size() << " 条记录到文件：" << filename << std::endl;
    return true;
}

bool FileIO::exportToCSV(const std::string& filename,
                        const std::vector<PlateRecord>& records) {
    std::ofstream fout(filename);
    if (!fout.is_open()) {
        std::cerr << "无法创建文件：" << filename << std::endl;
        return false;
    }
    
    // 写入CSV头（增加“类别”列）
    fout << "车牌号,城市,车主,类别\n";
    
    for (const auto& rec : records) {
        fout << rec.plate << "," << rec.city << "," << rec.owner << "," << rec.category << "\n";
    }
    
    fout.close();
    std::cout << "已导出 " << records.size() << " 条记录到CSV文件：" << filename << std::endl;
    return true;
}

