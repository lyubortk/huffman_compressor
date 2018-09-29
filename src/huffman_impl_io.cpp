#include "huffman_impl_io.h"

namespace HuffmanImpl {
    
    HuffmanBitIO::HuffmanBitIO(std::ios& stream_param)
            : stream(stream_param), buf(0), byte_cnt(0) {
    }
    
    HuffmanBitIO::~HuffmanBitIO() {
    }

    std::uint64_t HuffmanBitIO::get_byte_cnt() const {
        return byte_cnt;
    }
    
    HuffmanBitWriter::HuffmanBitWriter(std::ostream& out_stream) 
            : HuffmanBitIO(out_stream) {
        pos = 7;
    }

    HuffmanBitWriter::~HuffmanBitWriter() {
        flush();
    }

    void HuffmanBitWriter::write(const HuffmanArchiver::Codeword& codeword) {
        for (unsigned char bit: codeword) {
            if (pos == -1) {
                flush();    
            }
            buf ^= (bit << pos);
            --pos;
        }
    }
    
    void HuffmanBitWriter::flush() {
        if (pos != 7) {
            dynamic_cast<std::ostream&>(stream).write(
                reinterpret_cast<char*>(&buf), 1);
            if(stream.fail()) {
                throw HuffmanArchiver::IO_error("write error");
            }
            pos = 7;
            buf = 0;
            byte_cnt++;
        }
    }


    HuffmanBitReader::HuffmanBitReader(std::istream& in_stream) 
            : HuffmanBitIO(in_stream) {
        pos = -1;
    }

    bool HuffmanBitReader::read(bool& value) {
        if (pos == -1) {
            dynamic_cast<std::istream&>(stream).read(
                reinterpret_cast<char*>(&buf), 1);
            if(stream.fail()) {
                throw HuffmanArchiver::IO_error("read error");
            }
            pos = 7;
            byte_cnt++;
        }
        value = (buf >> pos)&1;
        --pos;
        return static_cast<bool>(stream);
    }
}
