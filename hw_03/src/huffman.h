#pragma once

#include <istream>
#include <ostream>
#include <vector>
#include <cstddef>
#include <cstdint>

namespace HuffmanArchiver {
    
    class IO_error : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    const std::size_t NUM_OF_BYTES = 256;
    const std::size_t FREQUENCY_TABLE_SIZE = 2048;
    const std::size_t SYSTEM_INFO_SIZE = 8;
    const std::uint64_t HEADER_SIZE = SYSTEM_INFO_SIZE + FREQUENCY_TABLE_SIZE;

    void encode(std::istream& in, std::ostream& out, 
                std::uint64_t& in_size, std::uint64_t& out_size);
    void decode(std::istream& in, std::ostream& out,
                std::uint64_t& in_size, std::uint64_t& out_sizse);
    
    class Frequencies { 
    public:
        Frequencies();
        ~Frequencies() = default;
        Frequencies(const Frequencies&) = default;
        Frequencies& operator=(const Frequencies&) = default;

        std::uint64_t operator[](std::size_t ind) const;
        std::uint64_t& operator[](std::size_t ind);
        void add(std::istream& in);
        void save(std::ostream& out);
        void load_saved(std::istream& in);
    private: 
        uint64_t arr[NUM_OF_BYTES];
    };
    
    using Codeword = typename std::vector<bool>; 
    
    class Codes {
    public:
        Codes(const Frequencies& frequencies);
        ~Codes() = default;
        Codes(const Codes&) = default;
        Codes& operator=(const Codes&) = default;

        Codeword operator[](std::size_t ind) const;
        Codeword& operator[](std::size_t ind);
    private:
        Codeword arr[NUM_OF_BYTES];
    };

    void encode(const Codes& codes, std::istream& in, std::ostream& out, 
                std::uint64_t& in_size, std::uint64_t& out_size);

    void decode(const Codes& codes, std::istream& in, 
                std::ostream& out, std::uint64_t bytes_encoded,
                std::uint64_t& in_size, std::uint64_t& out_size);
} 
