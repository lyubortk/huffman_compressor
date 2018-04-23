#include <priority_queue>
#include <algorithm>
#include "huffman.h"

using std::uint64_t;

namespace {

    class HuffmanBitWriter {
    public:
        HuffmanBitWriter(std::ostream& out);
        HuffmanBitWriter(const HuffmanBitWriter&) = delete;
        HuffmanBitWriter& operator=(const HuffmanBitWriter&) = delete;
        ~HuffmanBitWriter();

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

        bool read(bool& value);
    private:
        std::istream& in;
        unsigned char buf;
        int pos;
    }

    class HuffmanTree {
    public:
        HuffmanTree(unsigned char c, uint64_t frequency);
        HuffmanTree(HuffmanTree& left, HuffmanTree& right);
        HuffmanTree(const Codes& codes);
        ~HuffmanTree();
        HuffmanTree(const HuffmanTree&);
        HuffmanTree& operator=(const HuffmanTree&);

        uint64_t get_frequency() const;
        void compute_codes(Codes& codes) const;

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
        }

    private:

        class HuffmanTreeNode {
            friend class HuffmanTreeWalker;
            friend class HuffmanTree;
        public:
            HuffmanTreeNode(HuffmanTreeNode* left, HuffmanTreeNode* right);
            HuffmanTreeNode(unsigned char c, uint64_t frequency);
            ~HuffamanTreeNode() = default;
            HuffmanTreeNode(const HuffmanTreeNode&) = delete;
            HuffmanTreeNode& operator=(const HuffmanTreeNode&) = delete;
            
            void recursive_delete();
            void compute_codes(Codes& codes, std::vector<bool>& vec) const;
        private:
            HuffmanTreeNode* left;
            HuffmanTreeNode* right;
            unsigned char c;
            uint64_t frequency;
        }

        HuffamnTreeNode* root;
    }

    class HuffmanTreeCMP {
    public:
        bool operator()(const HuffmanTree& a, const HuffmanTree& b) {
            return a < b;
        }
    }

    
    HuffmanBitWriter::HuffmanBitWriter(std::ostream& out) 
        : out(out), buf(0), pos(-1) {}
    
    void HuffmanBitWriter::write(const Codeword& codeword) {
        for (bool bool_val: codeword) {
            unsigned char bit = bool_val? 1 : 0;
            if (pos == -1) {
                out.write(&buf, 1);
                pos = 7;
                buf = 0;
            }
            buf ^= (bit << pos);
            --pos;
        }
    }

    HuffmanBitWriter::~HuffmanBitWriter() {
        out.write(&buf, 1);
    }

    HuffmanBitReader::HuffmanBitReader(std::istream& in) 
        : in(in), buf(0), pos(-1) {}
    
    bool HuffmanBitReader::read(bool& value) {
        if (pos == -1) {
            in.read(&buf, 1);
            pos = 7;
        }
        if (in == 0) return in;
        value = ((buf >> pos) & 1 ? true : false);
        return in; 
    }

    HuffmanTree::HuffmanTree(unsigned char c, uint64_t frequency)
        : root(new HuffmanTreeNode(c, frequency)) {}

    HuffmanTree::HuffmanTree(HuffmanTree& left, HuffmanTree& right)
        : root(new HuffmanTreeNode(left.root, right.root)) {
        
        left.root = nullptr;
        right.root = nullptr;
    }

    HuffmanTree::HuffmanTree(Codes& codes) 
        :root(new HuffmanTreeNode(nullptr, nullptr)) {
        
        HuffmanTreeNode* cur;
        for (unsigned char i = 0; i < 256; ++i) {
            cur = root;          
            for (bool bit: codes[i]) {
                HuffmanTreeNode* dest = bit ? cur->right : cur->left;
                if (dest == nullptr) {
                    dest = new HuffmanTreeNode(nullptr, nullptr);
                }
                cur = dest;
            }
            cur.c = i; 
        }
    }

    HuffmanTree::~HuffmanTree() {
        recursive_delete(root);    
    }

    uint64_t HuffmanTree::get_frequency() const {
        return root->frequency;
    }

    void HuffmanTree::compute_codes(Codes& codes) const {
        std::vector<bool> vec;
        root->compute_codes(codes, vec);
    }

    HuffmanTreeWalker(const HuffmanTree& tree) 
        : root(tree->root), cur(root) {}

    void HuffmanTreeWalker::go(bool to) {
        if (is_leaf()) {
            cur = root;
        }
        cur = to ? cur->right : cur->left;
    }

    bool HuffmanTreeWalker::is_leaf() {
        return (cur->left == nullptr && cur->right == nullptr);
    }

    unsigned char HuffmanTreeWalker::get_byte() {
        return cur->c;
    }

    HuffmanTreeNode::HuffmanTreeNode(HuffmanTreeNode* left, 
                                     HuffmanTreeNode* right)
        : left(left), right(right), frequency(0) {
        if (left != nullptr) {
            frequency += left->frequency;
        } 
        if (right != nullptr) {
            frequency += right->frequency;
        }
    }

    HuffmanTreeNode::HuffmanTreeNode(unsigned char c, uint64_t frequency)
        : left(nullptr), right(nullptr), c(c), frequency(frequency) {}
        
    void HuffmanTreeNode::recursive_delete() {
        if (left != nullptr) {
            recursive_delete(left);
        }
        if (right != nullptr) {
            recursive_delete(right);
        }
        delete this;
    }

    void HuffmanTreeNode::compute_codes(Codes& codes, 
                                        std::vector<bool>& vec) const {
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

    void encode(std::istream& in, std::ostream& out) {
        Frequencies frequencies;
        frequencies.add(in);
        Codes codes(frequencies);
        in.clear();
        in.seekg(0);
        encode(codes, in, out);
    }

    void decode(std::istream& in, std::ostream& out) {
        Frequencies frequencies(in);
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
        HuffmanBitReader reader(in)
        HuffmanTree tree(codes);
        HuffmanTreeWalker walker(tree);

        bool temp;
        while (reader.read(temp)) {
            walker.go(temp);
            if (walker.is_leaf()) {
                out.write(&walker.get_byte(), 1);
            }
        }

    }
    
    Frequencies::Frequencies()
        : cnt() {}
    
    Frequencies::Frequencies(std::istream& in)
        : cnt() {
        add(in); 
    }

    uint64_t Frequencies::operator[](size_t ind) const {
        return arr[ind];
    }

    uint64_t& Frequencies::operator[](size_t ind) {
        return arr[ind];
    }

    void Frequencies::add(std::istream& in) {
        unsigned char c;
        while (in.read(&c, 1)) {
            cnt[c]++;
        }
    }

    Codes::Codes(const Frequencies& frequencies) {
        std::priority_queue<HuffmanTree, vector<HuffmanTree>, HuffmanTreeCMP> 
            tree_priority_q;

        for (unsigned char i = 0; i < 256; ++i) {
            tree_priority_q.push(HuffmanTree(i, frequencies[i]));
        }

        while (tree_priority_q.size() != 1) {
            HuffmanTree first = tree_priority_q.top();
            tree_priority_q.pop();
            HuffmanTree second = tree_priority_q.top();
            tree_pritoriy_q.pop();
            tree_priority_q.push(HuffmanTree(first, second));
        }

        HuffmanTree tree = tree_priority_q.top();
        tree.compute_codes(*this);
    }

    Codes::Codeword operator[](size_t ind) const {
        return arr[ind];
    }

    Codes::COdeword& operator[](size_t ind) {
        return arr[ind];
    }
}
