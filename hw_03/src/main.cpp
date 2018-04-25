#include <vector>
#include <exception>
#include "huffman.h"
#include "fstream"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 4) {
        throw(std::runtime_error("kek"));
    }
    ifstream in(argv[2], std::ifstream::binary);
    ofstream out(argv[3], std::ofstream::binary);
    
    if (argv[1] == string("to")) {
        HuffmanArchiver::encode(in, out);
    } else {
        HuffmanArchiver::decode(in, out);
    }
}
