#pragma once

#include <cstddef>

#define CHECK(e) Check((e),__FUNCTION__,__FILE__,__LINE__)

class Autotest {
public:
    static void Check(bool expr, const char *func, 
                      const char  *filename, std::size_t line_num);
    virtual ~Autotest() = default;
    static void ShowFinalResults(); 
    virtual void RunAllTests() = 0;
protected:
    static int failed_num;
    static int total_num;
};


