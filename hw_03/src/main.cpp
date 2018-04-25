#include <vector>
#include <exception>
#include <iostream>
#include "huffman.h"
#include "fstream"

using namespace std;

int main(int argc, char** argv) {
    try {
        ifstream in(argv[2], std::ifstream::binary);
        ofstream out(argv[3], std::ofstream::binary);
    
        if (argv[1] == string("to")) {
            HuffmanArchiver::encode(in, out);
        } else {
            HuffmanArchiver::decode(in, out);
        }
    } catch (const std::exception& excep) {
        std::cerr << excep.what(); 
    }

    return 0;
}
