#include <queue>
#include <algorithm>
#include "huffman.h"
#include "huffman_impl_io.h"
#include "huffman_impl_tree.h"

using std::uint64_t;
using std::size_t;
using HuffmanImpl::HuffmanTree;
using HuffmanImpl::HuffmanBitWriter;
using HuffmanImpl::HuffmanBitReader;

namespace HuffmanArchiver {

    void encode(const Codes& codes, std::istream& in, std::ostream& out,
                uint64_t& in_size, uint64_t& out_size) {
        HuffmanBitWriter writer(out);
        unsigned char c; 
        in_size = 0;
        while (in.read(reinterpret_cast<char*>(&c), 1)) {
            writer.write(codes[c]);
            in_size++;
        }
        writer.flush();
        out_size = writer.get_byte_cnt();
    }     

    void decode(const Codes& codes, std::istream& in, 
                std::ostream& out, uint64_t bytes_encoded, 
                uint64_t& in_size, uint64_t& out_size) {
        HuffmanBitReader reader(in);
        HuffmanTree tree(codes);
        HuffmanTree::TreeWalker walker(tree);

        out_size = bytes_encoded;

        while (bytes_encoded) {
            bool bit;
            reader.read(bit);
            walker.go(bit);
            if (walker.is_leaf()) {
                unsigned char byte = walker.get_byte();
                out.write(reinterpret_cast<char*>(&byte), 1);
                if (out.fail()) throw std::ostream::failure("write error");
                --bytes_encoded;
            }
        }
        in_size = reader.get_byte_cnt();
    }

    void encode(std::istream& in, std::ostream& out,
                uint64_t& in_size, uint64_t& out_size) {
        out.seekp(8, std::ostream::cur);

        Frequencies frequencies;
        frequencies.add(in);
        frequencies.save(out);

        Codes codes(frequencies);

        in.clear();
        in.seekg(0);
        encode(codes, in, out, in_size, out_size);
        out_size += HEADER_SIZE;

        out.seekp(0);
        out.write(reinterpret_cast<char*>(&in_size), 8);
    }

    void decode(std::istream& in, std::ostream& out,
                uint64_t& in_size, uint64_t& out_size) {
        uint64_t size;
        in.read(reinterpret_cast<char*>(&size), 8);

        Frequencies frequencies;
        frequencies.load_saved(in);
        
        Codes codes(frequencies);

        decode(codes, in, out, size, in_size, out_size);
        in_size += HEADER_SIZE;
    }

    Frequencies::Frequencies()
        : arr() {}
    
    uint64_t Frequencies::operator[](size_t ind) const {
        return arr[ind];
    }

    uint64_t& Frequencies::operator[](size_t ind) {
        return arr[ind];
    }

    void Frequencies::add(std::istream& in) {
        unsigned char c;
        while (in.read(reinterpret_cast<char*>(&c), 1)) {
            arr[c]++;
        }
    }
    
    void Frequencies::save(std::ostream& out) {
        for (int i = 0; i < 256; ++i) {
            out.write(reinterpret_cast<char*>(&arr[i]), 8);
        }
        if (out.fail()) throw std::ostream::failure("write error");
    }
    
    void Frequencies::load_saved(std::istream& in) {
        for (int i = 0; i < 256; ++i) {
            in.read(reinterpret_cast<char*>(&arr[i]), 8);
        }
        if (in.fail()) throw std::istream::failure(
                             "Wrong header / read error");
    }

    Codes::Codes(const Frequencies& frequencies) {

        std::priority_queue<HuffmanTree, std::vector<HuffmanTree>, 
                                         HuffmanTree::Greater> priority_q;
        for (int i = 0; i < 256; ++i) {
            priority_q.push(HuffmanTree(i, frequencies[i]));
        }

        while (priority_q.size() != 1) {
            HuffmanTree first = priority_q.top();
            priority_q.pop();

            HuffmanTree second = priority_q.top();
            priority_q.pop();

            priority_q.push(HuffmanTree(first, second));
        }

        HuffmanTree tree = priority_q.top();
        tree.compute_codes(*this);
    }

    Codeword Codes::operator[](size_t ind) const {
        return arr[ind];
    }

    Codeword& Codes::operator[](size_t ind) {
        return arr[ind];
    }
}
