#include "Huffman.h"
#include "fstream"

using namespace std;

int main(int argc, char** argv) {
    ifstream in(argv[2]);
    ofstream out(argv[3]);

    if (argv[1] == string("to")) {
        HuffmanArchiver::encode(in, out);
    } else {
        HuffmanArchiver::decode(in, out);
    }
}
