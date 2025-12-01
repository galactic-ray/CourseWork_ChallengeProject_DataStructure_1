#ifndef PLATE_RECORD_H
#define PLATE_RECORD_H

#include <string>
#include <iostream>
#include <iomanip>

/**
 * 车牌记录数据结构
 * 包含车牌号、城市、车主信息
 */
struct PlateRecord {
    std::string plate;  // 车牌号，如 "01B7238"
    std::string city;   // 城市，如 "沈阳"
    std::string owner;  // 车主姓名
    
    PlateRecord() = default;
    PlateRecord(const std::string& p, const std::string& c, const std::string& o)
        : plate(p), city(c), owner(o) {}
    
    // 打印记录
    void print() const {
        std::cout << std::left << std::setw(12) << plate
                  << std::left << std::setw(12) << city
                  << std::left << std::setw(15) << owner << std::endl;
    }
    
    // 比较操作符（用于排序）
    bool operator<(const PlateRecord& other) const {
        return plate < other.plate;
    }
    
    bool operator==(const PlateRecord& other) const {
        return plate == other.plate;
    }
};

/**
 * 城市分块索引结构
 * 用于分块索引查找
 */
struct CityBlock {
    std::string city;   // 城市名
    int start;          // 在顺序表中的起始下标
    int count;          // 该城市记录数
    
    CityBlock() : start(0), count(0) {}
    CityBlock(const std::string& c, int s, int cnt) 
        : city(c), start(s), count(cnt) {}
    
    bool operator<(const CityBlock& other) const {
        return city < other.city;
    }
};

/**
 * 静态链表节点（用于链式基数排序）
 */
struct RadixNode {
    std::string plate;      // 车牌号
    int indexInSeq;         // 对应顺序表中的下标
    int next;               // 下一个节点下标，0 表示空
    
    RadixNode() : indexInSeq(0), next(0) {}
};

#endif // PLATE_RECORD_H

