#include <vector>
#include <exception>
#include <iostream>
#include "huffman.h"
#include "fstream"

using namespace std;

int main(int argc, char** argv) {
    try {
        if (argc != 4) throw std::runtime_error("u sosi");
        ifstream in(argv[2], std::ifstream::binary);
        ofstream out(argv[3], std::ofstream::binary);
    
        std::uint64_t ins, outs;

        if (argv[1] == string("to")) {
            HuffmanArchiver::encode(in, out, ins, outs);
        } else {
            HuffmanArchiver::decode(in, out, ins, outs);
        }

        std::cout << ins << '\n' << outs << '\n';
    } catch (const std::ios::failure& excep) {
        std::cerr << "I/O Error:\n";
        std::cerr << excep.what() << '\n'; 
    }

    return 0;
}
