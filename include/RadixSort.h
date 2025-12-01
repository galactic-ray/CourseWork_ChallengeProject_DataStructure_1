#ifndef RADIX_SORT_H
#define RADIX_SORT_H

#include "PlateRecord.h"
#include <vector>

/**
 * 链式基数排序模块
 * 使用静态链表实现车牌号的基数排序
 */
class RadixSort {
public:
    /**
     * 对车牌记录进行链式基数排序
     * @param records 要排序的记录向量（会被修改）
     * @return 排序是否成功
     */
    static bool sort(std::vector<PlateRecord>& records);
    
    /**
     * 获取排序统计信息
     */
    static int getLastSortCount() { return lastSortCount; }
    static double getLastSortTime() { return lastSortTime; }

private:
    static int lastSortCount;
    static double lastSortTime;
    
    // 分配阶段：将链表节点分配到各个桶中
    static void distribute(const std::vector<RadixNode>& nodes, 
                          int head, int pos,
                          std::vector<int>& bucketHead, 
                          std::vector<int>& bucketTail);
    
    // 收集阶段：将各个桶的节点重新串联
    static int collect(std::vector<RadixNode>& nodes,
                      const std::vector<int>& bucketHead,
                      const std::vector<int>& bucketTail);
};

#endif // RADIX_SORT_H

