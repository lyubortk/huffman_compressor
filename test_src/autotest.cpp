#include <iostream>
#include <cstddef>
#include "autotest.h"

int Autotest::total_num = 0;
int Autotest::failed_num = 0;

void Autotest::Check(bool expr, const char *func, 
                 const char  *filename, size_t line_num) {
    ++total_num;
    if (!expr) {
        ++failed_num;
        std::cerr << "test failed: " << func << " in " 
                  << filename << ":" << line_num << std::endl;
    } 
}

void Autotest::ShowFinalResults() {
    if (failed_num == 0) {
        std::cout << "Testing passed." << std::endl;
    } else {
        std::cout << "Failed " << failed_num << " of " 
                  << total_num << " tests." << std::endl;
    }
}

