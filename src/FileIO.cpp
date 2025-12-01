#include "../include/FileIO.h"
#include "../include/Utils.h"
#include <fstream>
#include <iostream>

bool FileIO::loadFromFile(const std::string& filename, 
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
        
        records.push_back(rec);
        count++;
    }
    
    fin.close();
    std::cout << "从文件读取完成，成功导入 " << count << " 条记录。" << std::endl;
    return true;
}

bool FileIO::saveToFile(const std::string& filename,
                       const std::vector<PlateRecord>& records) {
    std::ofstream fout(filename);
    if (!fout.is_open()) {
        std::cerr << "无法创建文件：" << filename << std::endl;
        return false;
    }
    
    for (const auto& rec : records) {
        fout << rec.plate << " " << rec.city << " " << rec.owner << "\n";
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
    
    // 写入CSV头
    fout << "车牌号,城市,车主\n";
    
    for (const auto& rec : records) {
        fout << rec.plate << "," << rec.city << "," << rec.owner << "\n";
    }
    
    fout.close();
    std::cout << "已导出 " << records.size() << " 条记录到CSV文件：" << filename << std::endl;
    return true;
}

