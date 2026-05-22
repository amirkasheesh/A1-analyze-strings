#ifndef STRING_GENERATOR_H
#define STRING_GENERATOR_H

#include <algorithm>
#include <chrono>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>

class StringGenerator {
 public:
    static const int MIN_STRING_LENGTH = 10;
    static const int MAX_STRING_LENGTH = 200;
    static const int MAX_ARRAY_SIZE = 3000;

    struct GeneratedData {
        std::vector<std::string> randomStrings;
        std::vector<std::string> reverseSortedStrings;
        std::vector<std::string> almostSortedStrings;
        std::vector<std::string> prefixStrings;
    };

    StringGenerator() : generator(static_cast<unsigned int>(
        std::chrono::steady_clock::now().time_since_epoch().count())) {}

    explicit StringGenerator(unsigned int seed) : generator(seed) {}

    GeneratedData generateAllData() {
        GeneratedData data;

        data.randomStrings = generateRandomArray(MAX_ARRAY_SIZE);
        data.reverseSortedStrings = makeReverseSorted(data.randomStrings);
        data.almostSortedStrings = makeAlmostSorted(data.randomStrings);
        data.prefixStrings = generatePrefixArray(MAX_ARRAY_SIZE);

        return data;
    }

    std::vector<std::string> generateRandomArray(int size) {
        checkSize(size);

        std::vector<std::string> result;
        result.reserve(size);

        for (int i = 0; i < size; ++i) {
            result.push_back(generateRandomString());
        }

        return result;
    }

    std::vector<std::string> generatePrefixArray(int size) {
        checkSize(size);

        std::vector<std::string> result;
        result.reserve(size);

        for (int i = 0; i < size; ++i) {
            std::string current = generateStringWithCommonPrefix();
            result.push_back(current);
        }

        return result;
    }

    std::vector<std::string> makeReverseSorted(const std::vector<std::string>& strings) {
        std::vector<std::string> result = strings;

        std::sort(result.begin(), result.end());
        std::reverse(result.begin(), result.end());

        return result;
    }

    std::vector<std::string> makeAlmostSorted(const std::vector<std::string>& strings) {
        std::vector<std::string> result = strings;

        std::sort(result.begin(), result.end());

        int swapsCount = static_cast<int>(result.size()) / 50;

        if (swapsCount < 1) {
            swapsCount = 1;
        }

        std::uniform_int_distribution<int> indexDistribution(0, static_cast<int>(result.size()) - 1);

        for (int i = 0; i < swapsCount; ++i) {
            int firstIndex = indexDistribution(generator);
            int secondIndex = indexDistribution(generator);

            std::swap(result[firstIndex], result[secondIndex]);
        }

        return result;
    }

    std::vector<std::string> getSubarray(const std::vector<std::string>& strings, int size) const {
        if (size < 0 || size > static_cast<int>(strings.size())) {
            throw std::invalid_argument("Incorrect subarray size");
        }

        std::vector<std::string> result;

        for (int i = 0; i < size; ++i) {
            result.push_back(strings[i]);
        }

        return result;
    }

    std::string getAlphabet() const {
        return ALPHABET;
    }

    int getAlphabetSize() const {
        return static_cast<int>(ALPHABET.size());
    }

 private:
    const std::string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-";

    std::mt19937 generator;

    void checkSize(int size) const {
        if (size < 1 || size > MAX_ARRAY_SIZE) {
            throw std::invalid_argument("Incorrect array size");
        }
    }

    std::string generateRandomString() {
        std::uniform_int_distribution<int> lengthDistribution(MIN_STRING_LENGTH, MAX_STRING_LENGTH);
        std::uniform_int_distribution<int> charDistribution(0, static_cast<int>(ALPHABET.size()) - 1);

        int length = lengthDistribution(generator);
        std::string result;

        result.reserve(length);

        for (int i = 0; i < length; ++i) {
            int charIndex = charDistribution(generator);
            result.push_back(ALPHABET[charIndex]);
        }

        return result;
    }

    std::string generateStringWithCommonPrefix() {
        std::uniform_int_distribution<int> prefixLengthDistribution(5, 30);
        std::uniform_int_distribution<int> totalLengthDistribution(MIN_STRING_LENGTH, MAX_STRING_LENGTH);
        std::uniform_int_distribution<int> charDistribution(0, static_cast<int>(ALPHABET.size()) - 1);

        int prefixLength = prefixLengthDistribution(generator);
        int totalLength = totalLengthDistribution(generator);

        if (totalLength < prefixLength + 1) {
            totalLength = prefixLength + 1;
        }

        std::string result;

        result.reserve(totalLength);

        for (int i = 0; i < prefixLength; ++i) {
            result.push_back('A');
        }

        for (int i = prefixLength; i < totalLength; ++i) {
            int charIndex = charDistribution(generator);
            result.push_back(ALPHABET[charIndex]);
        }

        return result;
    }
};

#endif
