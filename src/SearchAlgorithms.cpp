#include "../include/SearchAlgorithms.h"
#include "../include/Utils.h"
#include <algorithm>
#include <chrono>

int SearchAlgorithms::lastSearchCount = 0;
double SearchAlgorithms::lastSearchTime = 0.0;

int SearchAlgorithms::binarySearch(const std::vector<PlateRecord>& records, 
                                  const std::string& plate) {
    auto start = std::chrono::high_resolution_clock::now();
    
    int l = 0, r = static_cast<int>(records.size()) - 1;
    int comparisons = 0;
    
    while (l <= r) {
        comparisons++;
        int mid = l + (r - l) / 2;
        if (records[mid].plate == plate) {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            lastSearchTime = duration.count() / 1000.0;
            lastSearchCount = comparisons;
            return mid;
        } else if (records[mid].plate < plate) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    lastSearchTime = duration.count() / 1000.0;
    lastSearchCount = comparisons;
    
    return -1;
}

int SearchAlgorithms::linearSearch(const std::vector<PlateRecord>& records,
                                  const std::string& plate) {
    auto start = std::chrono::high_resolution_clock::now();
    
    int comparisons = 0;
    for (size_t i = 0; i < records.size(); ++i) {
        comparisons++;
        if (records[i].plate == plate) {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            lastSearchTime = duration.count() / 1000.0;
            lastSearchCount = comparisons;
            return static_cast<int>(i);
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    lastSearchTime = duration.count() / 1000.0;
    lastSearchCount = comparisons;
    
    return -1;
}

int SearchAlgorithms::findCityBlock(const std::vector<CityBlock>& cityIndex,
                                   const std::string& city) {
    int l = 0, r = static_cast<int>(cityIndex.size()) - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (cityIndex[mid].city == city) {
            return mid;
        } else if (cityIndex[mid].city < city) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    return -1;
}

std::vector<PlateRecord> SearchAlgorithms::prefixSearch(
    const std::vector<PlateRecord>& records,
    const std::string& prefix) {
    std::vector<PlateRecord> result;
    std::string upperPrefix = Utils::toUpperStr(prefix);
    
    for (const auto& rec : records) {
        if (rec.plate.size() >= upperPrefix.size() &&
            rec.plate.compare(0, upperPrefix.size(), upperPrefix) == 0) {
            result.push_back(rec);
        }
    }
    
    return result;
}

