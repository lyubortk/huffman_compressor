#pragma once

#include <ios>
#include <istream>
#include <ostream>
#include "huffman.h"

namespace HuffmanImpl {
    
    class HuffmanBitIO {
    public:
        HuffmanBitIO(std::ios& stream_param);
        virtual ~HuffmanBitIO() = 0;
        
        std::uint64_t get_byte_cnt() const;
    protected:
        std::ios& stream;
        unsigned char buf;
        int pos;
        std::uint64_t byte_cnt;
    };

    class HuffmanBitWriter : public HuffmanBitIO {
    public:
        HuffmanBitWriter(std::ostream& out_stream);
        HuffmanBitWriter(const HuffmanBitWriter&) = delete;
        HuffmanBitWriter(HuffmanBitWriter&&) = delete;
        HuffmanBitWriter& operator=(const HuffmanBitWriter&) = delete;
        ~HuffmanBitWriter();

        void write(const HuffmanArchiver::Codeword& codeword);
        void flush();
    };

    class HuffmanBitReader : public HuffmanBitIO {
    public:
        HuffmanBitReader(std::istream& in_stream);
        HuffmanBitReader(const HuffmanBitReader&) = delete;
        HuffmanBitReader(HuffmanBitReader&&) = delete;
        HuffmanBitReader& operator=(const HuffmanBitReader&) = delete;
        ~HuffmanBitReader() = default;

        bool read(bool& value);
    };
}
