#include "../include/RadixSort.h"
#include "../include/Utils.h"
#include <algorithm>
#include <chrono>
#include <cstring>

int RadixSort::lastSortCount = 0;
double RadixSort::lastSortTime = 0.0;

bool RadixSort::sort(std::vector<PlateRecord>& records) {
    int n = static_cast<int>(records.size());
    if (n <= 1) {
        lastSortCount = n;
        lastSortTime = 0.0;
        return true;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    const int RADIX = 36;  // 0-9, A-Z 共36个字符
    const int LEN = 7;     // 车牌长度固定为7
    
    // 创建静态链表，使用 1..n 作为有效节点
    std::vector<RadixNode> nodes(n + 1);
    for (int i = 1; i <= n; ++i) {
        nodes[i].plate = records[i - 1].plate;
        nodes[i].indexInSeq = i - 1;
        nodes[i].next = i + 1;
    }
    nodes[n].next = 0; // 链尾
    int head = 1;
    
    std::vector<int> bucketHead(RADIX, 0);
    std::vector<int> bucketTail(RADIX, 0);
    
    // LSD基数排序：从最低位（第6位）到最高位（第0位）
    for (int pos = LEN - 1; pos >= 0; --pos) {
        // 初始化桶
        std::fill(bucketHead.begin(), bucketHead.end(), 0);
        std::fill(bucketTail.begin(), bucketTail.end(), 0);
        
        // 分配阶段
        int p = head;
        while (p != 0) {
            char c = nodes[p].plate[pos];
            int k = Utils::charToBucketIndex(c);
            
            if (bucketHead[k] == 0) {
                bucketHead[k] = p;
            } else {
                nodes[bucketTail[k]].next = p;
            }
            bucketTail[k] = p;
            
            int next = nodes[p].next;
            nodes[p].next = 0; // 暂时断开
            p = next;
        }
        
        // 收集阶段
        head = collect(nodes, bucketHead, bucketTail);
    }
    
    // 根据排序后的链表重排顺序表
    std::vector<PlateRecord> newRecords;
    newRecords.reserve(n);
    int p = head;
    while (p != 0) {
        newRecords.push_back(records[nodes[p].indexInSeq]);
        p = nodes[p].next;
    }
    records.swap(newRecords);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    lastSortTime = duration.count() / 1000.0; // 转换为毫秒
    lastSortCount = n;
    
    return true;
}

int RadixSort::collect(std::vector<RadixNode>& nodes,
                      const std::vector<int>& bucketHead,
                      const std::vector<int>& bucketTail) {
    int head = 0;
    int last = 0;
    
    for (size_t k = 0; k < bucketHead.size(); ++k) {
        if (bucketHead[k] != 0) {
            if (head == 0) {
                head = bucketHead[k];
                last = bucketTail[k];
            } else {
                nodes[last].next = bucketHead[k];
                last = bucketTail[k];
            }
        }
    }
    
    if (last != 0) {
        nodes[last].next = 0;
    }
    
    return head;
}

void RadixSort::distribute(const std::vector<RadixNode>& nodes, 
                          int head, int pos,
                          std::vector<int>& bucketHead, 
                          std::vector<int>& bucketTail) {
    // 这个函数在当前实现中已经内联到sort函数中
    // 保留接口以便未来扩展
}

