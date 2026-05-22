#ifndef STRING_SORTS_H
#define STRING_SORTS_H

#include <algorithm>
#include <string>
#include <vector>
#include <utility>

class StringSorts {
 public:
    struct SortStats {
        long long charComparisons = 0;
    };

    static void quickSort(std::vector<std::string>& strings, SortStats& stats) {
        if (strings.empty()) {
            return;
        }

        quickSort(strings, 0, static_cast<int>(strings.size()) - 1, stats);
    }

    static void mergeSort(std::vector<std::string>& strings, SortStats& stats) {
        if (strings.empty()) {
            return;
        }

        std::vector<std::string> buffer(strings.size());
        mergeSort(strings, buffer, 0, static_cast<int>(strings.size()) - 1, stats);
    }

    static void stringQuickSort(std::vector<std::string>& strings, SortStats& stats) {
        if (strings.empty()) {
            return;
        }

        stringQuickSort(strings, 0, static_cast<int>(strings.size()) - 1, 0, stats);
    }

    static void stringMergeSort(std::vector<std::string>& strings, SortStats& stats) {
        if (strings.empty()) {
            return;
        }

        std::vector<LcpItem> sortedItems = stringMergeSort(strings, 0, static_cast<int>(strings.size()) - 1, stats);

        for (int i = 0; i < static_cast<int>(sortedItems.size()); ++i) {
            strings[i] = sortedItems[i].value;
        }
    }

    static void msdRadixSort(std::vector<std::string>& strings, SortStats& stats) {
        if (strings.empty()) {
            return;
        }

        msdRadixSort(strings, 0, static_cast<int>(strings.size()) - 1, 0, stats);
    }

    static void msdRadixSortWithSwitch(std::vector<std::string>& strings, SortStats& stats) {
        if (strings.empty()) {
            return;
        }

        msdRadixSortWithSwitch(strings, 0, static_cast<int>(strings.size()) - 1, 0, stats);
    }

 private:
    static const int ASCII_SIZE = 256;
    static const int ALPHABET_SIZE = 74;

    struct LcpItem {
        std::string value;
        int lcp = 0;
    };

    struct LcpCompareResult {
        int compareResult = 0;
        int lcp = 0;
    };

    static int charAt(const std::string& value, int depth) {
        if (depth >= static_cast<int>(value.size())) {
            return -1;
        }

        return static_cast<unsigned char>(value[depth]);
    }

    static int compareChars(int firstChar, int secondChar, SortStats& stats) {
        if (firstChar != -1 && secondChar != -1) {
            ++stats.charComparisons;
        }

        if (firstChar < secondChar) {
            return -1;
        }

        if (firstChar > secondChar) {
            return 1;
        }

        return 0;
    }

    static int compareStrings(const std::string& first, const std::string& second, SortStats& stats) {
        int firstSize = static_cast<int>(first.size());
        int secondSize = static_cast<int>(second.size());
        int minSize = std::min(firstSize, secondSize);

        for (int i = 0; i < minSize; ++i) {
            ++stats.charComparisons;

            if (first[i] < second[i]) {
                return -1;
            }

            if (first[i] > second[i]) {
                return 1;
            }
        }

        if (firstSize < secondSize) {
            return -1;
        }

        if (firstSize > secondSize) {
            return 1;
        }

        return 0;
    }

    static LcpCompareResult lcpCompare(
        const std::string& first,
        const std::string& second,
        int knownPrefixLength,
        SortStats& stats
    ) {
        int firstSize = static_cast<int>(first.size());
        int secondSize = static_cast<int>(second.size());
        int minSize = std::min(firstSize, secondSize);

        int position = knownPrefixLength;

        while (position < minSize) {
            ++stats.charComparisons;

            if (first[position] < second[position]) {
                return {-1, position};
            }

            if (first[position] > second[position]) {
                return {1, position};
            }

            ++position;
        }

        if (firstSize < secondSize) {
            return {-1, position};
        }

        if (firstSize > secondSize) {
            return {1, position};
        }

        return {0, position};
    }

    static void quickSort(std::vector<std::string>& strings, int left, int right, SortStats& stats) {
        if (left >= right) {
            return;
        }

        int i = left;
        int j = right;
        std::string pivot = strings[(left + right) / 2];

        while (i <= j) {
            while (compareStrings(strings[i], pivot, stats) < 0) {
                ++i;
            }

            while (compareStrings(strings[j], pivot, stats) > 0) {
                --j;
            }

            if (i <= j) {
                std::swap(strings[i], strings[j]);
                ++i;
                --j;
            }
        }

        if (left < j) {
            quickSort(strings, left, j, stats);
        }

        if (i < right) {
            quickSort(strings, i, right, stats);
        }
    }

    static void mergeSort(
        std::vector<std::string>& strings,
        std::vector<std::string>& buffer,
        int left,
        int right,
        SortStats& stats
    ) {
        if (left >= right) {
            return;
        }

        int middle = (left + right) / 2;

        mergeSort(strings, buffer, left, middle, stats);
        mergeSort(strings, buffer, middle + 1, right, stats);
        merge(strings, buffer, left, middle, right, stats);
    }

    static void merge(
        std::vector<std::string>& strings,
        std::vector<std::string>& buffer,
        int left,
        int middle,
        int right,
        SortStats& stats
    ) {
        int i = left;
        int j = middle + 1;
        int k = left;

        while (i <= middle && j <= right) {
            if (compareStrings(strings[i], strings[j], stats) <= 0) {
                buffer[k] = strings[i];
                ++i;
            } else {
                buffer[k] = strings[j];
                ++j;
            }

            ++k;
        }

        while (i <= middle) {
            buffer[k] = strings[i];
            ++i;
            ++k;
        }

        while (j <= right) {
            buffer[k] = strings[j];
            ++j;
            ++k;
        }

        for (int index = left; index <= right; ++index) {
            strings[index] = buffer[index];
        }
    }

    static void stringQuickSort(
        std::vector<std::string>& strings,
        int left,
        int right,
        int depth,
        SortStats& stats
    ) {
        if (left >= right) {
            return;
        }

        int less = left;
        int greater = right;
        int i = left;

        int pivot = charAt(strings[(left + right) / 2], depth);

        while (i <= greater) {
            int current = charAt(strings[i], depth);
            int compareResult = compareChars(current, pivot, stats);

            if (compareResult < 0) {
                std::swap(strings[less], strings[i]);
                ++less;
                ++i;
            } else if (compareResult > 0) {
                std::swap(strings[i], strings[greater]);
                --greater;
            } else {
                ++i;
            }
        }

        stringQuickSort(strings, left, less - 1, depth, stats);

        if (pivot != -1) {
            stringQuickSort(strings, less, greater, depth + 1, stats);
        }

        stringQuickSort(strings, greater + 1, right, depth, stats);
    }

    static std::vector<LcpItem> stringMergeSort(
        const std::vector<std::string>& strings,
        int left,
        int right,
        SortStats& stats
    ) {
        if (left == right) {
            return {{strings[left], 0}};
        }

        int middle = (left + right) / 2;

        std::vector<LcpItem> firstPart = stringMergeSort(strings, left, middle, stats);
        std::vector<LcpItem> secondPart = stringMergeSort(strings, middle + 1, right, stats);

        return stringMerge(firstPart, secondPart, stats);
    }

    static std::vector<LcpItem> stringMerge(
        std::vector<LcpItem>& firstPart,
        std::vector<LcpItem>& secondPart,
        SortStats& stats
    ) {
        std::vector<LcpItem> result;
        result.reserve(firstPart.size() + secondPart.size());

        int i = 0;
        int j = 0;

        while (i < static_cast<int>(firstPart.size()) && j < static_cast<int>(secondPart.size())) {
            if (firstPart[i].lcp > secondPart[j].lcp) {
                result.push_back(firstPart[i]);
                ++i;
            } else if (firstPart[i].lcp < secondPart[j].lcp) {
                result.push_back(secondPart[j]);
                ++j;
            } else {
                LcpCompareResult compareResult = lcpCompare(
                    firstPart[i].value,
                    secondPart[j].value,
                    firstPart[i].lcp,
                    stats);

                if (compareResult.compareResult <= 0) {
                    result.push_back(firstPart[i]);
                    secondPart[j].lcp = compareResult.lcp;
                    ++i;
                } else {
                    result.push_back(secondPart[j]);
                    firstPart[i].lcp = compareResult.lcp;
                    ++j;
                }
            }
        }

        while (i < static_cast<int>(firstPart.size())) {
            result.push_back(firstPart[i]);
            ++i;
        }

        while (j < static_cast<int>(secondPart.size())) {
            result.push_back(secondPart[j]);
            ++j;
        }

        return result;
    }

    static void msdRadixSort(
        std::vector<std::string>& strings,
        int left,
        int right,
        int depth,
        SortStats& stats
    ) {
        if (left >= right) {
            return;
        }

        std::vector<std::vector<std::string>> buckets(ASCII_SIZE + 1);

        for (int i = left; i <= right; ++i) {
            int current = charAt(strings[i], depth);
            buckets[current + 1].push_back(strings[i]);
        }

        int position = left;

        for (int bucketIndex = 0; bucketIndex <= ASCII_SIZE; ++bucketIndex) {
            int start = position;

            for (int i = 0; i < static_cast<int>(buckets[bucketIndex].size()); ++i) {
                strings[position] = buckets[bucketIndex][i];
                ++position;
            }

            int finish = position - 1;

            if (bucketIndex > 0 && start < finish) {
                msdRadixSort(strings, start, finish, depth + 1, stats);
            }
        }
    }

    static void msdRadixSortWithSwitch(
        std::vector<std::string>& strings,
        int left,
        int right,
        int depth,
        SortStats& stats
    ) {
        if (left >= right) {
            return;
        }

        int currentSize = right - left + 1;

        if (currentSize < ALPHABET_SIZE) {
            stringQuickSort(strings, left, right, depth, stats);
            return;
        }

        std::vector<std::vector<std::string>> buckets(ASCII_SIZE + 1);

        for (int i = left; i <= right; ++i) {
            int current = charAt(strings[i], depth);
            buckets[current + 1].push_back(strings[i]);
        }

        int position = left;

        for (int bucketIndex = 0; bucketIndex <= ASCII_SIZE; ++bucketIndex) {
            int start = position;

            for (int i = 0; i < static_cast<int>(buckets[bucketIndex].size()); ++i) {
                strings[position] = buckets[bucketIndex][i];
                ++position;
            }

            int finish = position - 1;

            if (bucketIndex > 0 && start < finish) {
                msdRadixSortWithSwitch(strings, start, finish, depth + 1, stats);
            }
        }
    }
};

#endif
