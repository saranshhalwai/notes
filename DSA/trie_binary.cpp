#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>

/**
 * @brief Binary Trie (Bitwise Trie) implementation for integers.
 * Useful for bitwise queries such as maximum XOR or minimum XOR with a query integer.
 * Supports insertion, deletion (via reference counting), and XOR optimization queries.
 */
class BinaryTrie {
private:
    struct TrieNode {
        std::unique_ptr<TrieNode> children[2];
        int count; // Number of numbers sharing this node (for deletion)

        TrieNode() : count(0) {
            children[0] = nullptr;
            children[1] = nullptr;
        }
    };

    std::unique_ptr<TrieNode> root;
    int max_bit; // Number of bits to represent the numbers (e.g., 30 for numbers up to ~10^9)

public:
    /**
     * @brief Construct a Binary Trie.
     * @param bits The maximum number of bits to consider (default 30, suitable for values < 2^30).
     */
    explicit BinaryTrie(int bits = 30) : root(std::make_unique<TrieNode>()), max_bit(bits) {}

    /**
     * @brief Insert an integer into the trie.
     * @param num The integer to insert.
     */
    void insert(int num) {
        TrieNode* curr = root.get();
        for (int i = max_bit; i >= 0; --i) {
            int bit = (num >> i) & 1;
            if (!curr->children[bit]) {
                curr->children[bit] = std::make_unique<TrieNode>();
            }
            curr = curr->children[bit].get();
            curr->count++;
        }
    }

    /**
     * @brief Remove one occurrence of an integer from the trie.
     * @param num The integer to remove.
     * @return true if the integer was successfully removed, false if it wasn't in the trie.
     */
    bool remove(int num) {
        // First, verify if the number exists
        if (count(num) == 0) {
            return false;
        }

        TrieNode* curr = root.get();
        std::vector<TrieNode*> path;
        path.push_back(curr);

        for (int i = max_bit; i >= 0; --i) {
            int bit = (num >> i) & 1;
            curr = curr->children[bit].get();
            path.push_back(curr);
        }

        // Decrement counts and clean up nodes
        for (int i = path.size() - 1; i > 0; --i) {
            TrieNode* child = path[i];
            TrieNode* parent = path[i - 1];
            child->count--;
            if (child->count == 0) {
                // If count becomes 0, remove the child node
                int bit = (num >> (max_bit - (i - 1))) & 1;
                parent->children[bit].reset();
            }
        }
        return true;
    }

    /**
     * @brief Count occurrences of `num` in the trie.
     * @param num The integer to check.
     * @return The number of times `num` has been inserted and not yet removed.
     */
    int count(int num) const {
        const TrieNode* curr = root.get();
        for (int i = max_bit; i >= 0; --i) {
            int bit = (num >> i) & 1;
            if (!curr->children[bit]) {
                return 0;
            }
            curr = curr->children[bit].get();
        }
        return curr->count;
    }

    /**
     * @brief Find the maximum XOR value possible between `num` and any number in the trie.
     * @param num The query integer.
     * @return The maximum XOR value.
     * @throws std::runtime_error If the trie is empty.
     */
    int max_xor(int num) const {
        if (!root->children[0] && !root->children[1]) {
            throw std::runtime_error("Cannot query maximum XOR on an empty BinaryTrie");
        }

        const TrieNode* curr = root.get();
        int best_xor = 0;

        for (int i = max_bit; i >= 0; --i) {
            int bit = (num >> i) & 1;
            int desired_bit = 1 - bit; // Try to go the opposite path for max XOR

            if (curr->children[desired_bit]) {
                best_xor |= (1 << i);
                curr = curr->children[desired_bit].get();
            } else {
                curr = curr->children[bit].get();
            }
        }
        return best_xor;
    }

    /**
     * @brief Find the minimum XOR value possible between `num` and any number in the trie.
     * @param num The query integer.
     * @return The minimum XOR value.
     * @throws std::runtime_error If the trie is empty.
     */
    int min_xor(int num) const {
        if (!root->children[0] && !root->children[1]) {
            throw std::runtime_error("Cannot query minimum XOR on an empty BinaryTrie");
        }

        const TrieNode* curr = root.get();
        int min_xor_val = 0;

        for (int i = max_bit; i >= 0; --i) {
            int bit = (num >> i) & 1;
            // Try to go the same path for min XOR (resulting in 0 at this bit)
            if (curr->children[bit]) {
                curr = curr->children[bit].get();
            } else {
                min_xor_val |= (1 << i);
                curr = curr->children[1 - bit].get();
            }
        }
        return min_xor_val;
    }
};
