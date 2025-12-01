#include "../include/PlateDatabase.h"
#include "../include/RadixSort.h"
#include "../include/SearchAlgorithms.h"
#include "../include/Utils.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <chrono>

using namespace std;

// 测试工具函数
void testUtils() {
    cout << "\n========== 测试工具函数 ==========" << endl;
    
    // 测试车牌验证
    assert(Utils::isValidPlate("01B7238") == true);
    assert(Utils::isValidPlate("ABC1234") == true);
    assert(Utils::isValidPlate("1234567") == true);
    assert(Utils::isValidPlate("01B723") == false);  // 长度不足
    assert(Utils::isValidPlate("01B72389") == false); // 长度过长
    assert(Utils::isValidPlate("01b7238") == false);  // 小写字母
    
    cout << "✓ 车牌验证测试通过" << endl;
    
    // 测试字符串转换
    assert(Utils::toUpperStr("abc123") == "ABC123");
    assert(Utils::toUpperStr("ABC123") == "ABC123");
    
    cout << "✓ 字符串转换测试通过" << endl;
    
    cout << "工具函数测试全部通过！" << endl;
}

// 测试基数排序
void testRadixSort() {
    cout << "\n========== 测试链式基数排序 ==========" << endl;
    
    vector<PlateRecord> records = {
        {"01B7238", "沈阳", "张三"},
        {"02C4567", "大连", "李四"},
        {"01A1234", "沈阳", "王五"},
        {"03D7890", "鞍山", "赵六"}
    };
    
    cout << "排序前：" << endl;
    for (const auto& rec : records) {
        cout << rec.plate << " ";
    }
    cout << endl;
    
    RadixSort::sort(records);
    
    cout << "排序后：" << endl;
    for (const auto& rec : records) {
        cout << rec.plate << " ";
    }
    cout << endl;
    
    // 验证排序结果
    for (size_t i = 1; i < records.size(); ++i) {
        assert(records[i-1].plate <= records[i].plate);
    }
    
    cout << "✓ 基数排序测试通过" << endl;
    cout << "排序耗时：" << RadixSort::getLastSortTime() << " 毫秒" << endl;
}

// 测试查找算法
void testSearchAlgorithms() {
    cout << "\n========== 测试查找算法 ==========" << endl;
    
    vector<PlateRecord> records = {
        {"01A1234", "沈阳", "王五"},
        {"01B7238", "沈阳", "张三"},
        {"02C4567", "大连", "李四"},
        {"03D7890", "鞍山", "赵六"}
    };
    
    // 测试折半查找
    int idx = SearchAlgorithms::binarySearch(records, "01B7238");
    assert(idx == 1);
    cout << "✓ 折半查找测试通过" << endl;
    
    // 测试顺序查找
    idx = SearchAlgorithms::linearSearch(records, "02C4567");
    assert(idx == 2);
    cout << "✓ 顺序查找测试通过" << endl;
    
    // 测试前缀查找
    auto results = SearchAlgorithms::prefixSearch(records, "01");
    assert(results.size() == 2);
    cout << "✓ 前缀查找测试通过，找到 " << results.size() << " 条记录" << endl;
}

// 测试数据库操作
void testDatabase() {
    cout << "\n========== 测试数据库操作 ==========" << endl;
    
    PlateDatabase db;
    
    // 测试添加
    assert(db.addRecord("01B7238", "沈阳", "张三") == true);
    assert(db.addRecord("02C4567", "大连", "李四") == true);
    assert(db.addRecord("01A1234", "沈阳", "王五") == true);
    assert(db.getRecordCount() == 3);
    cout << "✓ 添加记录测试通过" << endl;
    
    // 测试查找
    int idx = db.findRecord("01B7238");
    assert(idx != -1);
    cout << "✓ 查找记录测试通过" << endl;
    
    // 测试修改
    assert(db.modifyRecord("01B7238", "大连", "张三新") == true);
    cout << "✓ 修改记录测试通过" << endl;
    
    // 测试删除
    assert(db.deleteRecord("01A1234") == true);
    assert(db.getRecordCount() == 2);
    cout << "✓ 删除记录测试通过" << endl;
    
    // 测试非法数据
    assert(db.addRecord("01B723", "沈阳", "测试") == false); // 长度不足
    assert(db.addRecord("01b7238", "沈阳", "测试") == false); // 小写字母
    cout << "✓ 非法数据验证测试通过" << endl;
}

// 测试城市分块索引
void testCityIndex() {
    cout << "\n========== 测试城市分块索引 ==========" << endl;
    
    PlateDatabase db;
    db.addRecord("01B7238", "沈阳", "张三");
    db.addRecord("02C4567", "大连", "李四");
    db.addRecord("01A1234", "沈阳", "王五");
    db.addRecord("03D7890", "鞍山", "赵六");
    db.addRecord("01C1111", "沈阳", "钱七");
    
    db.buildCityIndex();
    auto results = db.searchByCity("沈阳");
    assert(results.size() == 3);
    cout << "✓ 城市分块索引测试通过，沈阳有 " << results.size() << " 条记录" << endl;
}

// 性能测试
void performanceTest() {
    cout << "\n========== 性能测试 ==========" << endl;
    
    PlateDatabase db;
    int testSizes[] = {100, 1000, 5000};
    
    for (int size : testSizes) {
        cout << "\n测试规模：" << size << " 条记录" << endl;
        
        // 生成测试数据
        db.clearAll();
        db.generateRandomData(size);
        
        // 测试排序性能
        auto start = chrono::high_resolution_clock::now();
        db.radixSortByPlate();
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "  基数排序耗时：" << duration.count() / 1000.0 << " 毫秒" << endl;
        
        // 测试查找性能
        string testPlate = db.getRecordCount() > 0 ? "01B7238" : "";
        if (!testPlate.empty()) {
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 100; ++i) {
                db.binarySearchPlate(testPlate);
            }
            end = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::microseconds>(end - start);
            cout << "  100次折半查找耗时：" << duration.count() / 1000.0 << " 毫秒" << endl;
        }
    }
}

int main() {
    cout << "╔════════════════════════════════════════╗" << endl;
    cout << "║    汽车牌照查询系统 - 测试程序        ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    try {
        testUtils();
        testRadixSort();
        testSearchAlgorithms();
        testDatabase();
        testCityIndex();
        performanceTest();
        
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║    所有测试通过！                    ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
    } catch (const exception& e) {
        cout << "\n测试失败：" << e.what() << endl;
        return 1;
    }
    
    return 0;
}

