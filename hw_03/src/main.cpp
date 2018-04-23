#include <exception>
#include "huffman.h"
#include "fstream"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        throw(std::runtime_error("kek"));
    }
    ifstream in(argv[2]);
    ofstream out(argv[3]);

    if (argv[1] == string("to")) {
        HuffmanArchiver::encode(in, out);
    } else {
        HuffmanArchiver::decode(in, out);
    }
}
