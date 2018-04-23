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
        Freaquencies() = default;
        ~Frequencies() = default;
        Frequencies(const Frequencies&) = default;
        Frequencies& operator=(const Frequencies&) = default;

        uint64_t operator[](size_t ind) const;
        uint64_t& operator[](size_t ind);
        void add(std::istream& in);
    private: 
        uint64_t cnt[256];
    }
    
    struct Codeword {
        size_t len;
        std::vector<unsigned char> code;
    }
    
    class Codes {
    public:
        Codes(const Frequencies&);
        ~Codes() = default;
        Codes(const Codes&) = default;
        Codes& operator=(const Codes&) = default;
        
        Codeword operator[](size_t ind) const;
        Codeword& operator[](size_t ind);
    private:
        Codeword val[256];
    }

    void enocde(const Codes& codes, std::istream& in, std::ostream& out);
    void decode(const Codes& codes, std::istream& in, std::ostream& out);

}     
