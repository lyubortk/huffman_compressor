#include "huffman_test.h"

int main() {
    HuffmanArchiverTest huffman_test;
    huffman_test.RunAllTests();
    HuffmanArchiverTest::ShowFinalResults();

    return 0;
}
