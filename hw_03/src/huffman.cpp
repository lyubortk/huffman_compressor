#include "huffman.h"

namespace {

    class HuffmanBitWriter {
    public:
        HuffmanBitWriter(std::ostream& out);
        HuffmanBitWriter(const HuffmanBitWriter&) = delete;
        HuffmanBitWriter& operator=(const HuffmanBitWriter&) = delete;

        void write(const Codeword& codeword);
    private:
        std::ostream& out;
        unsigned char buf;
        int pos;
    }
    
    class HuffmanBitReader {
    public:
        HuffmanBitReader(std::ostream& in);
        HuffmanBitReader(const HuffmanBitReader&) = delete;
        HuffmanBitReader& operator=(const HuffmanBitReader&) = delete;

        unsigned char read();
    private:
        std::istream& in;
        unsigned char buf;
        int pos;
    }

    class HuffmanTree {
    public:
        HuffmanTree(unsigned char c);
        HuffmanTree(HuffmanTree& left, HuffmanTree& right);
        ~HuffmanTree();
        HuffmanTree(const HuffmanTree&) = delete;
        HuffmanTree& operator=(const HuffmanTree&) = delete;

    private:
        class HuffmanTreeNode {
        public:
            HuffmanTreeNode(HuffmanTreeNode* left, HuffmanTreeNode* right)
            HuffmanTreeNode(unsigned char c) 
            ~HuffamanTreeNode() = default;
            HuffmanTreeNode(const HuffmanTreeNode&) = delete;
            HuffmanTreeNode& operator=(const HuffmanTreeNode&) = delete;
        private:
            HuffmanTreeNode* left;
            HuffmanTreeNode* right;
            unsigned char c;
        }

        HuffamnTreeNode* root;
    }
}

namespace HuffmanArchiver {

    void encode(std::istream& in, std::ostream& out) {
        Frequencies frequencies;
        frequencies.add(in);
        Codes codes(frequencies);
        in.clear();
        in.seekg(0);
        encode(codes, in, out);
    }

    void decode(std::istream& in, std::ostream& out) {
        Frequencies frequencies;
        frequencies.add(in);
        Codes codes(frequencies);
        decode(codes, in, out);
    }

    void encode(const Codes& codes, std::istream& in, std::ostream& out) {
        HuffmanBitWriter writer(out);
        unsigned char c; 
        in.read(&c, 1);
        writer.write(codes[c]);     
    }     

    void decode(const Codes& codes, std::istream& in, std::istream& out) {
                    
    }
}
