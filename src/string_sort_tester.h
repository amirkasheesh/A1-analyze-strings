#ifndef STRING_SORT_TESTER_H
#define STRING_SORT_TESTER_H

#include "string_generator.h"
#include "string_sorts.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class StringSortTester {
 public:
    StringSortTester()
        : trialsCount(5),
          minArraySize(100),
          maxArraySize(3000),
          stepSize(100),
          outputFileName("src/results.csv") {
    }

    explicit StringSortTester(const std::string& outputFileName)
        : trialsCount(5),
          minArraySize(100),
          maxArraySize(3000),
          stepSize(100),
          outputFileName(outputFileName) {
    }

    void runAllTests() {
        StringGenerator generator(42);
        StringGenerator::GeneratedData data = generator.generateAllData();

        std::ofstream outputFile(outputFileName);

        if (!outputFile.is_open()) {
            throw std::runtime_error("Cant open");
        }

        outputFile << "algorithm,category,size,trial,time_us,char_comparisons\n";

        std::vector<TestCase> testCases = createTestCases(data);
        std::vector<Algorithm> algorithms = createAlgorithms();

        for (int testCaseIndex = 0; testCaseIndex < static_cast<int>(testCases.size()); ++testCaseIndex) {
            for (int algorithmIndex = 0; algorithmIndex < static_cast<int>(algorithms.size()); ++algorithmIndex) {
                runAlgorithmOnTestCase(
                    generator,
                    testCases[testCaseIndex],
                    algorithms[algorithmIndex],
                    outputFile);
            }
        }

        outputFile.close();

        std::cout << "all tests completed!!\n";
        std::cout << "results saved in: " << outputFileName << "\n";
    }

 private:
    struct TestCase {
        std::string name;
        std::vector<std::string> strings;
    };

    struct Algorithm {
        std::string name;
        std::function<void(std::vector<std::string>&, StringSorts::SortStats&)> sortFunction;
    };

    int trialsCount;
    int minArraySize;
    int maxArraySize;
    int stepSize;
    std::string outputFileName;

    std::vector<TestCase> createTestCases(const StringGenerator::GeneratedData& data) {
        std::vector<TestCase> testCases;

        testCases.push_back({"random", data.randomStrings});
        testCases.push_back({"reverseSorted", data.reverseSortedStrings});
        testCases.push_back({"almostSorted", data.almostSortedStrings});
        testCases.push_back({"commonPrefix", data.prefixStrings});

        return testCases;
    }

    std::vector<Algorithm> createAlgorithms() {
        std::vector<Algorithm> algorithms;

        algorithms.push_back({
            "QuickSort",
            [](std::vector<std::string>& strings, StringSorts::SortStats& stats) {
                StringSorts::quickSort(strings, stats);
            }
        });

        algorithms.push_back({
            "MergeSort",
            [](std::vector<std::string>& strings, StringSorts::SortStats& stats) {
                StringSorts::mergeSort(strings, stats);
            }
        });

        algorithms.push_back({
            "StringQuickSort",
            [](std::vector<std::string>& strings, StringSorts::SortStats& stats) {
                StringSorts::stringQuickSort(strings, stats);
            }
        });

        algorithms.push_back({
            "StringMergeSort",
            [](std::vector<std::string>& strings, StringSorts::SortStats& stats) {
                StringSorts::stringMergeSort(strings, stats);
            }
        });

        algorithms.push_back({
            "MsdRadixSort",
            [](std::vector<std::string>& strings, StringSorts::SortStats& stats) {
                StringSorts::msdRadixSort(strings, stats);
            }
        });

        algorithms.push_back({
            "MsdRadixSortWithSwitch",
            [](std::vector<std::string>& strings, StringSorts::SortStats& stats) {
                StringSorts::msdRadixSortWithSwitch(strings, stats);
            }
        });

        return algorithms;
    }

    void runAlgorithmOnTestCase(
        StringGenerator& generator,
        const TestCase& testCase,
        const Algorithm& algorithm,
        std::ofstream& outputFile
    ) {
        std::cout << "test case: " << testCase.name << ", algorithm: " << algorithm.name << "\n";

        for (int size = minArraySize; size <= maxArraySize; size += stepSize) {
            for (int trial = 1; trial <= trialsCount; ++trial) {
                std::vector<std::string> currentStrings = generator.getSubarray(testCase.strings, size);

                StringSorts::SortStats stats;

                auto startTime = std::chrono::high_resolution_clock::now();

                algorithm.sortFunction(currentStrings, stats);

                auto finishTime = std::chrono::high_resolution_clock::now();

                long long timeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(
                    finishTime - startTime
                ).count();

                checkSorted(currentStrings, algorithm.name, testCase.name, size);

                outputFile << algorithm.name << ","
                           << testCase.name << ","
                           << size << ","
                           << trial << ","
                           << timeMicroseconds << ","
                           << stats.charComparisons << "\n";
            }
        }
    }

    void checkSorted(
        const std::vector<std::string>& strings,
        const std::string& algorithmName,
        const std::string& testCaseName,
        int size
    ) {
        if (!std::is_sorted(strings.begin(), strings.end())) {
            std::string message = "sorting error! Algorithm: " + algorithmName
                + ", test case: " + testCaseName
                + ", size: " + std::to_string(size);

            throw std::runtime_error(message);
        }
    }
};

#endif
