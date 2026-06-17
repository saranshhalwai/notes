#include <vector>
#include <iostream>

/**
 * @brief Fenwick Tree (Binary Indexed Tree) for point updates and prefix/range queries.
 * 0-indexed externally, 1-indexed internally.
 * Supports O(log N) point updates and range queries.
 */
template <typename T>
class FenwickTree {
private:
    int n;
    std::vector<T> tree;

public:
    /**
     * @brief Construct a Fenwick Tree of size `n` initialized with zeros.
     * @param n Size of the array.
     */
    explicit FenwickTree(int n) : n(n), tree(n + 1, 0) {}

    /**
     * @brief Construct a Fenwick Tree from an initial array.
     * @param arr The source array.
     */
    explicit FenwickTree(const std::vector<T>& arr) : n(arr.size()), tree(arr.size() + 1, 0) {
        for (int i = 0; i < n; ++i) {
            add(i, arr[i]);
        }
    }

    /**
     * @brief Add value `val` to the element at index `idx` (0-indexed).
     * @param idx The index to update.
     * @param val The value to add.
     */
    void add(int idx, T val) {
        for (++idx; idx <= n; idx += idx & -idx) {
            tree[idx] += val;
        }
    }

    /**
     * @brief Get the prefix sum from index 0 to `idx` (0-indexed) inclusive.
     * @param idx The end index of the prefix range.
     * @return Prefix sum.
     */
    T query(int idx) const {
        T sum = 0;
        for (++idx; idx > 0; idx -= idx & -idx) {
            sum += tree[idx];
        }
        return sum;
    }

    /**
     * @brief Query the range sum in [l, r] (0-indexed, inclusive).
     * @param l Left bound of the query range.
     * @param r Right bound of the query range.
     * @return Sum of elements in range [l, r].
     */
    T query(int l, int r) const {
        if (l < 0 || r >= n || l > r) {
            return 0;
        }
        return query(r) - (l > 0 ? query(l - 1) : 0);
    }
};
