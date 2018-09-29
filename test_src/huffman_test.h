#pragma once

#include "autotest.h"

class HuffmanArchiverTest: public Autotest {
public:
    virtual void RunAllTests();
private:
    void frequencies_add_test();
    void frequencies_save_load_test();

    void codes_test();

    void encode_decode_test_1();
    void encode_decode_test_2();
    void encode_decode_test_3();
    
    void encode_decode_by_codes_test_1();
    void encode_decode_by_codes_test_2();
};
