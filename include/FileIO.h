#ifndef FILE_IO_H
#define FILE_IO_H

#include "PlateRecord.h"
#include <string>
#include <vector>

/**
 * 文件IO模块
 * 负责从文件读取和保存车牌记录
 */
class FileIO {
public:
    /**
     * 从文件加载记录
     * @param filename 文件名
     * @param records 输出参数，加载的记录
     * @return 是否成功
     */
    static bool loadFromFile(const std::string& filename, 
                            std::vector<PlateRecord>& records);
    
    /**
     * 保存记录到文件
     * @param filename 文件名
     * @param records 要保存的记录
     * @return 是否成功
     */
    static bool saveToFile(const std::string& filename,
                          const std::vector<PlateRecord>& records);
    
    /**
     * 导出为CSV格式
     * @param filename 文件名
     * @param records 要保存的记录
     * @return 是否成功
     */
    static bool exportToCSV(const std::string& filename,
                           const std::vector<PlateRecord>& records);
};

#endif // FILE_IO_H

