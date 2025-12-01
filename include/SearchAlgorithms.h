#ifndef SEARCH_ALGORITHMS_H
#define SEARCH_ALGORITHMS_H

#include "PlateRecord.h"
#include <vector>
#include <string>

/**
 * 查找算法模块
 * 包含折半查找和分块索引查找
 */
class SearchAlgorithms {
public:
    /**
     * 折半查找车牌号（要求记录已按车牌号排序）
     * @param records 已排序的记录向量
     * @param plate 要查找的车牌号
     * @return 找到返回下标，未找到返回-1
     */
    static int binarySearch(const std::vector<PlateRecord>& records, 
                           const std::string& plate);
    
    /**
     * 顺序查找车牌号（用于未排序数据）
     * @param records 记录向量
     * @param plate 要查找的车牌号
     * @return 找到返回下标，未找到返回-1
     */
    static int linearSearch(const std::vector<PlateRecord>& records,
                           const std::string& plate);
    
    /**
     * 在分块索引中查找城市块
     * @param cityIndex 城市索引向量
     * @param city 要查找的城市名
     * @return 找到返回块索引，未找到返回-1
     */
    static int findCityBlock(const std::vector<CityBlock>& cityIndex,
                            const std::string& city);
    
    /**
     * 前缀匹配查找
     * @param records 记录向量
     * @param prefix 车牌前缀
     * @return 匹配的记录向量
     */
    static std::vector<PlateRecord> prefixSearch(
        const std::vector<PlateRecord>& records,
        const std::string& prefix);
    
    /**
     * 获取查找统计信息
     */
    static int getLastSearchCount() { return lastSearchCount; }
    static double getLastSearchTime() { return lastSearchTime; }

private:
    static int lastSearchCount;
    static double lastSearchTime;
};

#endif // SEARCH_ALGORITHMS_H

