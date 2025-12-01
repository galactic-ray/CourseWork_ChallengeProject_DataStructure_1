#include "../include/PlateDatabase.h"
#include "../include/FileIO.h"
#include "../include/Utils.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include <sstream>
#include <cmath>
#include <vector>

PlateDatabase::PlateDatabase() 
    : sortedByPlate(false), cityIndexBuilt(false), 
      totalOperations(0), totalSearches(0) {
}

bool PlateDatabase::addRecord(const std::string& plate, 
                              const std::string& city,
                              const std::string& owner) {
    std::string upperPlate = Utils::toUpperStr(plate);
    
    if (!Utils::isValidPlate(upperPlate)) {
        std::cout << "车牌号格式非法，录入失败！" << std::endl;
        return false;
    }
    
    // 检查是否已存在
    if (findRecord(upperPlate) != -1) {
        std::cout << "车牌号已存在，录入失败！" << std::endl;
        return false;
    }
    
    records.emplace_back(upperPlate, city, owner);
    // 根据车牌确定车辆类别（油车/电车）
    records.back().category = Utils::getPlateCategory(upperPlate);
    sortedByPlate = false;
    cityIndexBuilt = false;
    totalOperations++;
    
    return true;
}

bool PlateDatabase::modifyRecord(const std::string& plate,
                                 const std::string& newCity,
                                 const std::string& newOwner) {
    std::string upperPlate = Utils::toUpperStr(plate);
    int idx = findRecord(upperPlate);
    
    if (idx == -1) {
        std::cout << "未找到该车牌号！" << std::endl;
        return false;
    }
    
    records[idx].city = newCity;
    records[idx].owner = newOwner;
    cityIndexBuilt = false;
    totalOperations++;
    
    return true;
}

bool PlateDatabase::deleteRecord(const std::string& plate) {
    std::string upperPlate = Utils::toUpperStr(plate);
    int idx = findRecord(upperPlate);
    
    if (idx == -1) {
        std::cout << "未找到该车牌号！" << std::endl;
        return false;
    }
    
    records.erase(records.begin() + idx);
    sortedByPlate = false;
    cityIndexBuilt = false;
    totalOperations++;
    
    return true;
}

int PlateDatabase::findRecord(const std::string& plate) const {
    totalSearches++;
    
    if (sortedByPlate) {
        return SearchAlgorithms::binarySearch(records, plate);
    } else {
        return SearchAlgorithms::linearSearch(records, plate);
    }
}

int PlateDatabase::binarySearchPlate(const std::string& plate) const {
    if (!sortedByPlate) {
        std::cout << "当前未按车牌排序，将自动使用基数排序..." << std::endl;
        const_cast<PlateDatabase*>(this)->radixSortByPlate();
    }
    
    return SearchAlgorithms::binarySearch(records, plate);
}

bool PlateDatabase::loadFromFile(const std::string& filename) {
    std::vector<PlateRecord> newRecords;
    if (!FileIO::loadFromFile(filename, newRecords)) {
        return false;
    }
    
    records.insert(records.end(), newRecords.begin(), newRecords.end());
    sortedByPlate = false;
    cityIndexBuilt = false;
    totalOperations++;
    
    return true;
}

void PlateDatabase::generateRandomData(int count) {
    static const std::vector<std::string> cities = {
        "沈阳", "大连", "鞍山", "抚顺", "本溪", "丹东",
        "锦州", "营口", "阜新", "辽阳", "盘锦", "铁岭", "朝阳", "葫芦岛"
    };
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> cityDist(0, static_cast<int>(cities.size()) - 1);
    std::uniform_int_distribution<int> typeDist(0, 1); // 0=油车, 1=电车
    
    for (int i = 0; i < count; ++i) {
        // 先随机选择一个城市
        std::string city = cities[cityDist(gen)];
        
        // 随机决定是油车还是电车
        bool isNewEnergy = (typeDist(gen) == 1);
        
        // 根据城市和类型生成对应的车牌（确保车牌字母与城市匹配）
        std::string plate = isNewEnergy
            ? Utils::generateRandomNewEnergyPlateByCity(city)
            : Utils::generateRandomPlateByCity(city);
        std::string owner = "随机车主" + std::to_string(i + 1);
        
        records.emplace_back(plate, city, owner);
        records.back().category = isNewEnergy ? "电车" : "油车";
    }
    
    sortedByPlate = false;
    cityIndexBuilt = false;
    totalOperations++;
    
    std::cout << "随机生成 " << count << " 条记录完成！" << std::endl;
}

void PlateDatabase::radixSortByPlate() {
    if (records.empty()) {
        sortedByPlate = true;
        return;
    }
    
    RadixSort::sort(records);
    sortedByPlate = true;
    cityIndexBuilt = false;
    
    std::cout << "已使用静态链表链式基数排序对车牌进行排序！" << std::endl;
    std::cout << "排序记录数：" << RadixSort::getLastSortCount() 
              << "，耗时：" << RadixSort::getLastSortTime() << " 毫秒" << std::endl;
}

std::vector<PlateRecord> PlateDatabase::searchByCity(const std::string& city) {
    if (!cityIndexBuilt) {
        std::cout << "城市索引未建立，正在建立..." << std::endl;
        buildCityIndex();
    }
    
    int blockId = SearchAlgorithms::findCityBlock(cityIndex, city);
    if (blockId == -1) {
        return std::vector<PlateRecord>();
    }
    
    CityBlock& block = cityIndex[blockId];
    std::vector<PlateRecord> result;
    result.reserve(block.count);
    
    for (int i = block.start; i < block.start + block.count; ++i) {
        result.push_back(records[i]);
    }
    
    return result;
}

void PlateDatabase::buildCityIndex() {
    if (records.empty()) {
        std::cout << "当前无记录，无法建立索引。" << std::endl;
        return;
    }
    
    // 按 city, plate 排序
    std::sort(records.begin(), records.end(),
              [](const PlateRecord& a, const PlateRecord& b) {
                  if (a.city != b.city) return a.city < b.city;
                  return a.plate < b.plate;
              });
    
    cityIndex.clear();
    int n = static_cast<int>(records.size());
    int i = 0;
    
    while (i < n) {
        int j = i + 1;
        while (j < n && records[j].city == records[i].city) {
            j++;
        }
        
        cityIndex.emplace_back(records[i].city, i, j - i);
        i = j;
    }
    
    std::sort(cityIndex.begin(), cityIndex.end());
    cityIndexBuilt = true;
    sortedByPlate = false;
    
    std::cout << "已建立城市分块索引，共有 " << cityIndex.size() << " 个城市块。" << std::endl;
}

std::vector<PlateRecord> PlateDatabase::prefixSearch(const std::string& prefix) const {
    return SearchAlgorithms::prefixSearch(records, prefix);
}

void PlateDatabase::showAllRecords() const {
    if (records.empty()) {
        std::cout << "当前无任何记录。" << std::endl;
        return;
    }
    
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << std::left << std::setw(12) << "车牌号"
              << std::left << std::setw(12) << "城市"
              << std::left << std::setw(15) << "车主"
              << std::left << std::setw(8)  << "类别" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    for (const auto& rec : records) {
        rec.print();
    }
    
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "共 " << records.size() << " 条记录" << std::endl;
}

void PlateDatabase::showRecord(int index) const {
    if (index < 0 || index >= static_cast<int>(records.size())) {
        std::cout << "索引越界！" << std::endl;
        return;
    }
    
    std::cout << std::left << std::setw(12) << "车牌号"
              << std::left << std::setw(12) << "城市"
              << std::left << std::setw(15) << "车主"
              << std::left << std::setw(8)  << "类别" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    records[index].print();
}

void PlateDatabase::statistics() const {
    std::cout << "\n========== 统计信息 ==========" << std::endl;
    std::cout << "当前共有记录条数：" << records.size() << std::endl;
    
    if (records.empty()) {
        std::cout << "=============================" << std::endl;
        return;
    }
    
    std::unordered_map<std::string, int> cnt;
    for (const auto& rec : records) {
        cnt[rec.city]++;
    }
    
    std::cout << "各城市车牌数量统计：" << std::endl;
    for (const auto& p : cnt) {
        std::cout << "  城市：" << std::setw(8) << p.first 
                  << "  数量：" << p.second << std::endl;
    }
    
    std::cout << "=============================" << std::endl;
}

int PlateDatabase::getCityCount() const {
    std::unordered_map<std::string, int> cities;
    for (const auto& rec : records) {
        cities[rec.city] = 1;
    }
    return static_cast<int>(cities.size());
}

bool PlateDatabase::saveToFile(const std::string& filename) const {
    return FileIO::saveToFile(filename, records);
}

bool PlateDatabase::exportToCSV(const std::string& filename) const {
    return FileIO::exportToCSV(filename, records);
}

void PlateDatabase::clearAll() {
    records.clear();
    cityIndex.clear();
    sortedByPlate = false;
    cityIndexBuilt = false;
    totalOperations = 0;
    totalSearches = 0;
    std::cout << "已清空所有数据。" << std::endl;
}

std::string PlateDatabase::getPerformanceStats() const {
    std::ostringstream oss;
    oss << "========== 性能统计 ==========\n";
    oss << "总操作次数：" << totalOperations << "\n";
    oss << "总查找次数：" << totalSearches << "\n";
    oss << "当前记录数：" << records.size() << "\n";
    oss << "是否已排序：" << (sortedByPlate ? "是" : "否") << "\n";
    oss << "城市索引已建立：" << (cityIndexBuilt ? "是" : "否") << "\n";
    
    if (RadixSort::getLastSortCount() > 0) {
        oss << "\n【排序统计】\n";
        oss << "上次排序记录数：" << RadixSort::getLastSortCount() << "\n";
        oss << "上次排序耗时：" << std::fixed << std::setprecision(2) 
            << RadixSort::getLastSortTime() << " 毫秒\n";
        if (RadixSort::getLastSortCount() > 0) {
            double avgTime = RadixSort::getLastSortTime() / RadixSort::getLastSortCount();
            oss << "平均每条记录耗时：" << std::fixed << std::setprecision(4) << avgTime << " 毫秒\n";
        }
    }
    
    if (SearchAlgorithms::getLastSearchCount() > 0) {
        oss << "\n【查找统计】\n";
        oss << "上次查找比较次数：" << SearchAlgorithms::getLastSearchCount() << "\n";
        oss << "上次查找耗时：" << std::fixed << std::setprecision(2) 
            << SearchAlgorithms::getLastSearchTime() << " 毫秒\n";
    }
    
    // 计算平均查找时间（如果有查找记录）
    if (totalSearches > 0 && records.size() > 0) {
        oss << "\n【性能分析】\n";
        if (sortedByPlate) {
            oss << "当前使用折半查找，时间复杂度：O(log n)\n";
            oss << "理论最大比较次数：" << static_cast<int>(std::ceil(std::log2(records.size()))) << "\n";
        } else {
            oss << "当前使用顺序查找，时间复杂度：O(n)\n";
            oss << "理论平均比较次数：" << records.size() / 2 << "\n";
        }
    }
    
    oss << "=============================";
    return oss.str();
}

void PlateDatabase::showPerformanceStats() const {
    std::cout << "\n" << getPerformanceStats() << std::endl;
}

bool PlateDatabase::batchImport(const std::vector<PlateRecord>& newRecords) {
    int validCount = 0;
    for (const auto& rec : newRecords) {
        if (Utils::isValidPlate(rec.plate)) {
            records.push_back(rec);
            validCount++;
        }
    }
    
    sortedByPlate = false;
    cityIndexBuilt = false;
    totalOperations++;
    
    std::cout << "批量导入完成，成功导入 " << validCount << " 条记录。" << std::endl;
    return validCount > 0;
}

std::vector<std::pair<std::string, int>> PlateDatabase::getCityStatistics() const {
    std::unordered_map<std::string, int> cnt;
    for (const auto& rec : records) {
        cnt[rec.city]++;
    }
    
    std::vector<std::pair<std::string, int>> result;
    result.reserve(cnt.size());
    for (const auto& p : cnt) {
        result.emplace_back(p.first, p.second);
    }
    
    std::sort(result.begin(), result.end(),
              [](const std::pair<std::string, int>& a,
                 const std::pair<std::string, int>& b) {
                  return a.second > b.second; // 按数量降序
              });
    
    return result;
}

std::string PlateDatabase::getValidateDataResult() const {
    std::ostringstream oss;
    std::unordered_map<std::string, int> plateCount;
    int invalidCount = 0;
    std::vector<std::string> invalidPlates;
    std::vector<std::string> duplicatePlates;
    
    for (const auto& rec : records) {
        if (!Utils::isValidPlate(rec.plate)) {
            invalidCount++;
            invalidPlates.push_back(rec.plate);
        }
        plateCount[rec.plate]++;
    }
    
    int duplicateCount = 0;
    for (const auto& p : plateCount) {
        if (p.second > 1) {
            duplicateCount += p.second - 1;
            duplicatePlates.push_back(p.first);
        }
    }
    
    oss << "========== 数据验证结果 ==========\n";
    oss << "总记录数：" << records.size() << "\n";
    oss << "有效记录数：" << (records.size() - invalidCount) << "\n";
    oss << "非法车牌数：" << invalidCount << "\n";
    oss << "重复车牌数：" << duplicateCount << "\n";
    
    bool isValid = (invalidCount == 0 && duplicateCount == 0);
    oss << "数据完整性：" << (isValid ? "✓ 通过" : "✗ 未通过") << "\n";
    
    if (invalidCount > 0) {
        oss << "\n【非法车牌列表】\n";
        int showCount = std::min(10, static_cast<int>(invalidPlates.size()));
        for (int i = 0; i < showCount; ++i) {
            oss << "  " << invalidPlates[i] << "\n";
        }
        if (invalidPlates.size() > 10) {
            oss << "  ... 还有 " << (invalidPlates.size() - 10) << " 个非法车牌\n";
        }
    }
    
    if (duplicateCount > 0) {
        oss << "\n【重复车牌列表】\n";
        int showCount = std::min(10, static_cast<int>(duplicatePlates.size()));
        for (int i = 0; i < showCount; ++i) {
            oss << "  " << duplicatePlates[i] << " (出现 " << plateCount[duplicatePlates[i]] << " 次)\n";
        }
        if (duplicatePlates.size() > 10) {
            oss << "  ... 还有 " << (duplicatePlates.size() - 10) << " 个重复车牌\n";
        }
    }
    
    // 验证车牌字母与城市匹配
    int mismatchCount = 0;
    std::vector<std::string> mismatchPlates;
    for (const auto& rec : records) {
        if (Utils::isValidPlate(rec.plate)) {
            if (!Utils::validatePlateCityMatch(rec.plate, rec.city)) {
                mismatchCount++;
                if (mismatchPlates.size() < 10) {
                    mismatchPlates.push_back(rec.plate + " (城市: " + rec.city + ")");
                }
            }
        }
    }
    
    if (mismatchCount > 0) {
        oss << "\n【车牌与城市不匹配】\n";
        oss << "不匹配数量：" << mismatchCount << "\n";
        for (const auto& plate : mismatchPlates) {
            oss << "  " << plate << "\n";
        }
        if (mismatchCount > 10) {
            oss << "  ... 还有 " << (mismatchCount - 10) << " 个不匹配记录\n";
        }
    } else {
        oss << "\n【车牌与城市匹配】✓ 全部匹配\n";
    }
    
    oss << "=============================";
    return oss.str();
}

bool PlateDatabase::validateData() const {
    std::string result = getValidateDataResult();
    std::cout << "\n" << result << std::endl;
    
    std::unordered_map<std::string, int> plateCount;
    int invalidCount = 0;
    for (const auto& rec : records) {
        if (!Utils::isValidPlate(rec.plate)) {
            invalidCount++;
        }
        plateCount[rec.plate]++;
    }
    
    int duplicateCount = 0;
    for (const auto& p : plateCount) {
        if (p.second > 1) {
            duplicateCount += p.second - 1;
        }
    }
    
    return invalidCount == 0 && duplicateCount == 0;
}

