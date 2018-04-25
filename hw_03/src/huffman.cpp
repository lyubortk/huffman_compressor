#include <queue>
#include <algorithm>
#include "huffman.h"

using std::uint64_t;
using std::size_t;

namespace {

    class HuffmanBitWriter {
    public:
        HuffmanBitWriter(std::ostream& out_stream);
        HuffmanBitWriter(const HuffmanBitWriter&) = delete;
        HuffmanBitWriter& operator=(const HuffmanBitWriter&) = delete;
        ~HuffmanBitWriter();

        void write(const HuffmanArchiver::Codeword& codeword);
        uint64_t get_written_cnt() const;
    private:
        std::ostream& out;
        unsigned char buf;
        int pos;
        uint64_t written_cnt;
    };
    
    class HuffmanBitReader {
    public:
        HuffmanBitReader(std::istream& in_stream);
        HuffmanBitReader(const HuffmanBitReader&) = delete;
        HuffmanBitReader& operator=(const HuffmanBitReader&) = delete;

        bool read(bool& value);
        uint64_t get_read_cnt() const;
    private:
        std::istream& in;
        unsigned char buf;
        int pos;
        uint64_t read_cnt;
    };

    class HuffmanTree {
    public:
        HuffmanTree(unsigned char byte, uint64_t frequency_val);
        HuffmanTree(HuffmanTree&& left, HuffmanTree&& right);
        HuffmanTree(const HuffmanArchiver::Codes& codes);
        ~HuffmanTree();
        HuffmanTree(HuffmanTree& other) = delete;
        HuffmanTree(HuffmanTree&& other);
        HuffmanTree& operator=(HuffmanTree other);

        uint64_t get_frequency() const;
        void compute_codes(HuffmanArchiver::Codes& codes) const;

    private:
        class HuffmanTreeNode {
            friend class HuffmanTreeWalker;
            friend class HuffmanTree;
        public:
            HuffmanTreeNode(HuffmanTreeNode* left_child, 
                            HuffmanTreeNode* right_child);
            HuffmanTreeNode(unsigned char byte, uint64_t frequency_val);
            ~HuffmanTreeNode() = default;
            HuffmanTreeNode(const HuffmanTreeNode&) = delete;
            HuffmanTreeNode& operator=(const HuffmanTreeNode&) = delete;
            
            void recursive_delete();
            void compute_codes(HuffmanArchiver::Codes& codes, 
                               std::vector<bool>& vec) const;
        private:
            HuffmanTreeNode* left;
            HuffmanTreeNode* right;
            unsigned char c;
            uint64_t frequency;
        };

        HuffmanTreeNode* root;

    public:
        class HuffmanTreeWalker {
        public:
            HuffmanTreeWalker(const HuffmanTree& tree);
            ~HuffmanTreeWalker() = default;
            HuffmanTreeWalker(const HuffmanTreeWalker&) = default;
            HuffmanTreeWalker& operator=(const HuffmanTreeWalker&) = default;
            
            void go(bool to);
            bool is_leaf() const;
            unsigned char get_byte() const;
        private:
            HuffmanTreeNode* root;
            HuffmanTreeNode* cur;
        };

        class HuffmanTreeGreater {
        public:
            bool operator()(const HuffmanTree& a, const HuffmanTree& b) const;
        };
    };

    
    bool HuffmanTree::HuffmanTreeGreater::operator()(
            const HuffmanTree& a, const HuffmanTree& b) const {
        return a.get_frequency() > b.get_frequency();
    }
    


    HuffmanBitWriter::HuffmanBitWriter(std::ostream& out_stream) 
            : out(out_stream), buf(0), pos(7), written_cnt(0) {
    }
    
    void HuffmanBitWriter::write(const HuffmanArchiver::Codeword& codeword) {
        for (unsigned char bit: codeword) {
            if (pos == -1) {
                out.write(reinterpret_cast<char*>(&buf), 1);
                pos = 7;
                buf = 0;
                written_cnt++;
            }
            buf ^= (bit << pos);
            --pos;
        }
    }

    HuffmanBitWriter::~HuffmanBitWriter() {
        out.write(reinterpret_cast<char*>(&buf), 1);
        written_cnt++;
    }
    
    uint64_t HuffmanBitWriter::get_written_cnt() const {
        return written_cnt;
    }


    HuffmanBitReader::HuffmanBitReader(std::istream& in_stream) 
            : in(in_stream), buf(0), pos(-1), read_cnt(0) {
    }
    
    bool HuffmanBitReader::read(bool& value) {
        if (pos == -1) {
            in.read(reinterpret_cast<char*>(&buf), 1);
            pos = 7;
            read_cnt++;
        }

        if (in) {
            value = (buf >> pos)&1;
            --pos;
        }

        return static_cast<bool>(in); 
    }
    
    uint64_t HuffmanBitReader::get_read_cnt() const {
        return read_cnt;
    }


    HuffmanTree::HuffmanTree(unsigned char byte, uint64_t frequency_val)
            : root(new HuffmanTreeNode(byte, frequency_val)) {
    }

    HuffmanTree::HuffmanTree(HuffmanTree&& left, HuffmanTree&& right)
            : root(new HuffmanTreeNode(left.root, right.root)) {
        left.root = nullptr;
        right.root = nullptr;
    }

    HuffmanTree::HuffmanTree(const HuffmanArchiver::Codes& codes) 
            :root(new HuffmanTreeNode(nullptr, nullptr)) {
        HuffmanTreeNode* cur;
        for (int i = 0; i < 256; ++i) {
            cur = root;          
            for (bool bit: codes[i]) {
                HuffmanTreeNode*& dest = bit ? cur->right : cur->left;
                if (dest == nullptr) {
                    dest = new HuffmanTreeNode(nullptr, nullptr);
                }
                cur = dest;
            }
            cur->c = i; 
        }
    }

    HuffmanTree::~HuffmanTree() {
        if (root != nullptr) {
            root->recursive_delete();
        }
    }
    
    HuffmanTree::HuffmanTree(HuffmanTree&& other)
            : root(other.root) {
        other.root = nullptr;
    }

    HuffmanTree& HuffmanTree::operator=(HuffmanTree other) {
        std::swap(root, other.root);
        return *this;
    }

    uint64_t HuffmanTree::get_frequency() const {
        return root->frequency;
    }

    void HuffmanTree::compute_codes(HuffmanArchiver::Codes& codes) const {
        std::vector<bool> vec;
        root->compute_codes(codes, vec);
    }
    

    HuffmanTree::HuffmanTreeWalker::HuffmanTreeWalker(const HuffmanTree& tree) 
            : root(tree.root), cur(root) {
    }

    void HuffmanTree::HuffmanTreeWalker::go(bool to) {
        if (is_leaf()) {
            cur = root;
        }
        cur = to ? cur->right : cur->left;
    }

    bool HuffmanTree::HuffmanTreeWalker::is_leaf() const {
        return (cur->left == nullptr && cur->right == nullptr);
    }

    unsigned char HuffmanTree::HuffmanTreeWalker::get_byte() const {
        return cur->c;
    }



    HuffmanTree::HuffmanTreeNode::HuffmanTreeNode(HuffmanTreeNode* left_child, 
                                                  HuffmanTreeNode* right_child)
            : left(left_child), right(right_child), frequency(0) {
        if (left != nullptr) {
            frequency += left->frequency;
        } 
        if (right != nullptr) {
            frequency += right->frequency;
        }
    }

    HuffmanTree::HuffmanTreeNode::HuffmanTreeNode(unsigned char byte, 
                                                  uint64_t frequency_val)
            : left(nullptr), right(nullptr), c(byte), frequency(frequency_val) {
    }
        
    void HuffmanTree::HuffmanTreeNode::recursive_delete() {
        if (left != nullptr) {
            left->recursive_delete();
        }
        if (right != nullptr) {
            right->recursive_delete();
        }
        delete this;
    }

    void HuffmanTree::HuffmanTreeNode::compute_codes(
        HuffmanArchiver::Codes& codes, std::vector<bool>& vec) const {
        if (left == nullptr && right == nullptr) {
            codes[c] = vec;
            return;
        }
        
        vec.push_back(0);
        left->compute_codes(codes, vec);
        vec.pop_back();

        vec.push_back(1);
        right->compute_codes(codes, vec);
        vec.pop_back(); 
    }
}

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
        out_size = writer.get_written_cnt();
    }     

    void decode(const Codes& codes, std::istream& in, 
                std::ostream& out, uint64_t bytes_encoded, 
                uint64_t& in_size, uint64_t& out_size) {
        HuffmanBitReader reader(in);
        HuffmanTree tree(codes);
        HuffmanTree::HuffmanTreeWalker walker(tree);

        out_size = bytes_encoded;

        bool bit;
        while (reader.read(bit) && bytes_encoded) {
            walker.go(bit);
            if (walker.is_leaf()) {
                unsigned char byte = walker.get_byte();
                out.write(reinterpret_cast<char*>(&byte), 1);
                --bytes_encoded;
            }
        }
        in_size = reader.get_read_cnt();
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

        decode(codes, in, out, size);
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
    }
    
    void Frequencies::load_saved(std::istream& in) {
        for (int i = 0; i < 256; ++i) {
            in.read(reinterpret_cast<char*>(&arr[i]), 8);
        }
    }

    Codes::Codes(const Frequencies& frequencies) {
        using tree_greater = HuffmanTree::HuffmanTreeGreater;

        std::vector<HuffmanTree> priority_q;
        for (int i = 0; i < 256; ++i) {
            priority_q.push_back(HuffmanTree(i, frequencies[i]));
        }
        std::make_heap(priority_q.begin(), priority_q.end(), tree_greater());

        while (priority_q.size() != 1) {
            pop_heap(priority_q.begin(), priority_q.end(), tree_greater());
            HuffmanTree first = std::move(priority_q.back());
            priority_q.pop_back();

            pop_heap(priority_q.begin(), priority_q.end(), tree_greater());
            HuffmanTree second = std::move(priority_q.back());
            priority_q.pop_back();

            priority_q.push_back(HuffmanTree(std::move(first), std::move(second)));
            push_heap(priority_q.begin(), priority_q.end(), tree_greater());
        }

        HuffmanTree tree = std::move(priority_q.front());
        tree.compute_codes(*this);
    }

    Codeword Codes::operator[](size_t ind) const {
        return arr[ind];
    }

    Codeword& Codes::operator[](size_t ind) {
        return arr[ind];
    }
}
