#ifndef PLATE_DATABASE_H
#define PLATE_DATABASE_H

#include "PlateRecord.h"
#include "RadixSort.h"
#include "SearchAlgorithms.h"
#include <vector>
#include <string>
#include <unordered_map>

/**
 * 车牌数据库核心类
 * 管理车牌记录的增删改查、排序、统计等功能
 */
class PlateDatabase {
private:
    std::vector<PlateRecord> records;      // 顺序表存储
    std::vector<CityBlock> cityIndex;      // 城市分块索引
    bool sortedByPlate;                    // 是否按车牌排序
    bool cityIndexBuilt;                   // 城市索引是否已建立
    
    // 性能统计
    mutable int totalOperations;
    mutable int totalSearches;
    
public:
    PlateDatabase();
    
    // ========== 基本操作：增删改查 ==========
    
    /**
     * 添加记录（手工输入）
     */
    bool addRecord(const std::string& plate, const std::string& city, 
                   const std::string& owner);
    
    /**
     * 修改记录
     */
    bool modifyRecord(const std::string& plate, const std::string& newCity,
                     const std::string& newOwner);
    
    /**
     * 删除记录
     */
    bool deleteRecord(const std::string& plate);
    
    /**
     * 查找记录（自动选择最优算法）
     */
    int findRecord(const std::string& plate) const;
    
    /**
     * 折半查找车牌
     */
    int binarySearchPlate(const std::string& plate) const;
    
    // ========== 数据输入 ==========
    
    /**
     * 从文件导入
     */
    bool loadFromFile(const std::string& filename);
    
    /**
     * 随机生成数据
     */
    void generateRandomData(int count);
    
    // ========== 排序操作 ==========
    
    /**
     * 使用链式基数排序对车牌排序
     */
    void radixSortByPlate();
    
    /**
     * 检查是否已排序
     */
    bool isSorted() const { return sortedByPlate; }
    
    // ========== 查找操作 ==========
    
    /**
     * 按城市分块索引查找
     */
    std::vector<PlateRecord> searchByCity(const std::string& city);
    
    /**
     * 建立城市分块索引
     */
    void buildCityIndex();
    
    /**
     * 前缀模糊查询
     */
    std::vector<PlateRecord> prefixSearch(const std::string& prefix) const;
    
    // ========== 统计与显示 ==========
    
    /**
     * 显示所有记录
     */
    void showAllRecords() const;
    
    /**
     * 显示指定记录
     */
    void showRecord(int index) const;
    
    /**
     * 统计信息
     */
    void statistics() const;
    
    /**
     * 获取记录总数
     */
    size_t getRecordCount() const { return records.size(); }
    
    /**
     * 获取所有记录（用于GUI显示）
     */
    std::vector<PlateRecord> getAllRecords() const { return records; }
    
    /**
     * 获取城市数量
     */
    int getCityCount() const;
    
    // ========== 文件操作 ==========
    
    /**
     * 保存到文件
     */
    bool saveToFile(const std::string& filename) const;
    
    /**
     * 导出为CSV
     */
    bool exportToCSV(const std::string& filename) const;
    
    // ========== 系统维护 ==========
    
    /**
     * 清空所有数据
     */
    void clearAll();
    
    /**
     * 获取性能统计（返回字符串）
     */
    std::string getPerformanceStats() const;
    
    /**
     * 显示性能统计（控制台输出）
     */
    void showPerformanceStats() const;
    
    // ========== 高级功能 ==========
    
    /**
     * 批量导入（从向量）
     */
    bool batchImport(const std::vector<PlateRecord>& newRecords);
    
    /**
     * 按城市统计并排序
     */
    std::vector<std::pair<std::string, int>> getCityStatistics() const;
    
    /**
     * 验证数据完整性（返回验证结果字符串）
     */
    std::string getValidateDataResult() const;
    
    /**
     * 验证数据完整性（控制台输出）
     */
    bool validateData() const;
};

#endif // PLATE_DATABASE_H

