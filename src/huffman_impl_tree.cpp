#include "huffman_impl_tree.h"

namespace HuffmanImpl {

    bool HuffmanTree::Greater::operator()(
            const HuffmanTree& a, const HuffmanTree& b) const {
        return a.get_frequency() > b.get_frequency();
    }

    HuffmanTree::HuffmanTree(unsigned char byte, uint64_t frequency_val)
            : root(std::make_shared<Node>(byte, frequency_val)) {
    }

    HuffmanTree::HuffmanTree(const HuffmanTree& left, const HuffmanTree& right)
            : root(std::make_shared<Node>(left.root, right.root)) {
    }

    HuffmanTree::HuffmanTree(const HuffmanArchiver::Codes& codes) 
            :root(std::make_shared<Node>(nullptr, nullptr)) {
        for (std::uint_fast16_t i = 0; i < HuffmanArchiver::NUM_OF_BYTES; ++i) {
            std::shared_ptr<Node> cur = root;          
            for (bool bit: codes[i]) {

                std::shared_ptr<Node>& dest = 
                    bit ? cur->right : cur->left;

                if (dest == nullptr) {
                    dest = std::make_shared<Node>
                            (Node(nullptr, nullptr));
                }
                cur = dest;
            }
            cur->c = i; 
        }
    }

    uint64_t HuffmanTree::get_frequency() const {
        return root->frequency;
    }

    void HuffmanTree::compute_codes(HuffmanArchiver::Codes& codes) const {
        std::vector<bool> vec;
        root->compute_codes(codes, vec);
    }


    HuffmanTree::TreeWalker::TreeWalker(const HuffmanTree& tree) 
            : root(tree.root), cur(root) {
    }

    void HuffmanTree::TreeWalker::go(bool to) {
        if (is_leaf()) {
            cur = root;
        }
        cur = to ? cur->right : cur->left;
    }

    bool HuffmanTree::TreeWalker::is_leaf() const {
        return (cur->left == nullptr && cur->right == nullptr);
    }

    unsigned char HuffmanTree::TreeWalker::get_byte() const {
        return cur->c;
    }


    HuffmanTree::Node::Node(
            std::shared_ptr<Node> left_child, 
            std::shared_ptr<Node> right_child)
            : left(left_child), right(right_child), frequency(0) {
        if (left != nullptr) {
            frequency += left->frequency;
        } 
        if (right != nullptr) {
            frequency += right->frequency;
        }
    }

    HuffmanTree::Node::Node(unsigned char byte, 
                                                  uint64_t frequency_val)
            : left(nullptr), right(nullptr), c(byte), frequency(frequency_val) {
    }
    

    void HuffmanTree::Node::compute_codes(
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
