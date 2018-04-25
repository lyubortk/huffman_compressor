#include <istream>
#include <ostream>
#include <vector>
#include <cstddef>
#include <cstdint>

namespace HuffmanArchiver {
    const std::size_t NUM_OF_BYTES = 256;

    std::uint64_t encode(std::istream& in, std::ostream& out);
    std::uint64_t decode(std::istream& in, std::ostream& out);
    
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

    std::uint64_t enocde(const Codes& codes, 
                         std::istream& in, std::ostream& out);

    std::uint64_t decode(const Codes& codes, std::istream& in, 
                         std::ostream& out, std::uint64_t size);
} 
