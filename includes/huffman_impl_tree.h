#pragma once

#include <memory>
#include "huffman.h"

namespace HuffmanImpl {

    class HuffmanTree {
    public:
        HuffmanTree(unsigned char byte, uint64_t frequency_val);
        HuffmanTree(const HuffmanTree& left, const HuffmanTree& right);
        HuffmanTree(const HuffmanArchiver::Codes& codes);
        ~HuffmanTree() = default;
        HuffmanTree(const HuffmanTree& other) = default;
        HuffmanTree(HuffmanTree&& other) = default;
        HuffmanTree& operator=(const HuffmanTree& other) = default;

        uint64_t get_frequency() const;
        void compute_codes(HuffmanArchiver::Codes& codes) const;

    private:
        class Node {
            friend class TreeWalker;
            friend class HuffmanTree;
        public:
            Node(std::shared_ptr<Node> left_child, 
                 std::shared_ptr<Node> right_child);
            Node(unsigned char byte, uint64_t frequency_val);
            ~Node() = default;
            Node(const Node&) = delete;
            Node(Node&&) = default;
            Node& operator=(const Node&) = delete;
            
            void recursive_delete();
            void compute_codes(HuffmanArchiver::Codes& codes, 
                               std::vector<bool>& vec) const;
        private:
            std::shared_ptr<Node> left;
            std::shared_ptr<Node> right;
            unsigned char c;
            uint64_t frequency;
        };

        std::shared_ptr<Node> root;

    public:
        class TreeWalker {
        public:
            TreeWalker(const HuffmanTree& tree);
            ~TreeWalker() = default;
            TreeWalker(const TreeWalker&) = default;
            TreeWalker(TreeWalker&&) = default;
            TreeWalker& operator=(const TreeWalker&) = default;
            
            void go(bool to);
            bool is_leaf() const;
            unsigned char get_byte() const;
        private:
            std::shared_ptr<Node> root;
            std::shared_ptr<Node> cur;
        };

        class Greater {
        public:
            bool operator()(const HuffmanTree& a, const HuffmanTree& b) const;
        };
    };
}
