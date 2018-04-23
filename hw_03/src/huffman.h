#include <istream>
#include <ostream>
#include <vector>
#include <cstddef>
#include <cstdint>

namespace HuffmanArchiver {
    
    void encode(std::istream& in, std::ostream& out);
    void decode(std::istream& in, std::ostream& out);
    
    class Frequencies { 
    public:
        Frequencies();
        Frequencies(std::istream& in);
        ~Frequencies() = default;
        Frequencies(const Frequencies&) = default;
        Frequencies& operator=(const Frequencies&) = default;

        std::uint64_t operator[](size_t ind) const;
        std::uint64_t& operator[](size_t ind);
        void add(std::istream& in);
    private: 
        uint64_t arr[256];
    };
    
    using Codeword = typename std::vector<bool>; 
    
    class Codes {
    public:
        Codes(const Frequencies& frequencies);
        ~Codes() = default;
        Codes(const Codes&) = default;
        Codes& operator=(const Codes&) = default;

        Codeword operator[](size_t ind) const;
        Codeword& operator[](size_t ind);
    private:
        Codeword arr[256];
    };

    void enocde(const Codes& codes, std::istream& in, std::ostream& out);
    void decode(const Codes& codes, std::istream& in, std::ostream& out);
} 
