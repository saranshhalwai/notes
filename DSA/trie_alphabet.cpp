#include <string>
#include <vector>
#include <memory>

/**
 * @brief Trie (Prefix Tree) implementation for lowercase English letters ('a' - 'z').
 * Supports insertion, search, prefix matching, and deletion of words.
 */
class AlphabetTrie {
private:
    struct TrieNode {
        std::vector<std::unique_ptr<TrieNode>> children;
        bool is_end_of_word;

        TrieNode() : children(26), is_end_of_word(false) {}
    };

    std::unique_ptr<TrieNode> root;

    bool remove_helper(TrieNode* curr, const std::string& word, int depth) {
        if (!curr) {
            return false;
        }

        if (depth == word.length()) {
            if (!curr->is_end_of_word) {
                return false; // Word does not exist
            }
            curr->is_end_of_word = false;
            // If node has no children, it can be deleted
            return isEmpty(curr);
        }

        int idx = word[depth] - 'a';
        if (idx < 0 || idx >= 26) {
            return false; // Invalid character
        }

        if (!curr->children[idx]) {
            return false; // Word does not exist
        }

        bool should_delete_child = remove_helper(curr->children[idx].get(), word, depth + 1);

        if (should_delete_child) {
            curr->children[idx].reset(); // Delete the child node
            // Return true if current node has no other children and is not end of another word
            return !curr->is_end_of_word && isEmpty(curr);
        }

        return false;
    }

    bool isEmpty(const TrieNode* node) const {
        for (const auto& child : node->children) {
            if (child) {
                return false;
            }
        }
        return true;
    }

public:
    AlphabetTrie() : root(std::make_unique<TrieNode>()) {}

    /**
     * @brief Insert a word into the trie.
     * @param word The word to insert (lowercase English letters only).
     */
    void insert(const std::string& word) {
        TrieNode* curr = root.get();
        for (char ch : word) {
            int idx = ch - 'a';
            if (idx < 0 || idx >= 26) {
                continue; // Ignore invalid characters
            }
            if (!curr->children[idx]) {
                curr->children[idx] = std::make_unique<TrieNode>();
            }
            curr = curr->children[idx].get();
        }
        curr->is_end_of_word = true;
    }

    /**
     * @brief Search if a word exists in the trie.
     * @param word The word to search for.
     * @return true if the word is in the trie, false otherwise.
     */
    bool search(const std::string& word) const {
        const TrieNode* curr = root.get();
        for (char ch : word) {
            int idx = ch - 'a';
            if (idx < 0 || idx >= 26 || !curr->children[idx]) {
                return false;
            }
            curr = curr->children[idx].get();
        }
        return curr->is_end_of_word;
    }

    /**
     * @brief Check if any word in the trie starts with the given prefix.
     * @param prefix The prefix to search for.
     * @return true if there is any word starting with `prefix`, false otherwise.
     */
    bool startsWith(const std::string& prefix) const {
        const TrieNode* curr = root.get();
        for (char ch : prefix) {
            int idx = ch - 'a';
            if (idx < 0 || idx >= 26 || !curr->children[idx]) {
                return false;
            }
            curr = curr->children[idx].get();
        }
        return true;
    }

    /**
     * @brief Remove a word from the trie.
     * @param word The word to remove.
     * @return true if the word was successfully removed, false if the word was not found.
     */
    bool remove(const std::string& word) {
        return remove_helper(root.get(), word, 0);
    }
};
