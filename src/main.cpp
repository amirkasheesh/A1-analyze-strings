#include <exception>
#include <iostream>
#include "./string_sort_tester.h"

int main() {
    try {
        StringSortTester tester("src/results.csv");
        tester.runAllTests();
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << "\n";
        return 1;
    }

    return 0;
}
