#include <vector>
#include <numeric>
#include <iostream>

/**
 * @brief Disjoint Set Union (DSU) / Union-Find structure.
 * Supports finding the representative of a set, union of two sets, and checking connectivity.
 * Uses path compression and union by size for O(alpha(N)) amortized operations.
 */
class DSU {
private:
    std::vector<int> parent;
    std::vector<int> sz;
    int num_components;

public:
    /**
     * @brief Construct a new DSU object with n elements (0 to n-1).
     * @param n Number of elements.
     */
    explicit DSU(int n) : parent(n), sz(n, 1), num_components(n) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    /**
     * @brief Find the representative element of the set containing `v`.
     * Applies path compression.
     * @param v The element to search for.
     * @return The representative element.
     */
    int find(int v) {
        if (v == parent[v]) {
            return v;
        }
        return parent[v] = find(parent[v]);
    }

    /**
     * @brief Union the set containing `a` and the set containing `b`.
     * Applies union by size.
     * @param a First element.
     * @param b Second element.
     * @return true if two disjoint sets were merged, false if they were already in the same set.
     */
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a != b) {
            if (sz[a] < sz[b]) {
                std::swap(a, b);
            }
            parent[b] = a;
            sz[a] += sz[b];
            num_components--;
            return true;
        }
        return false;
    }

    /**
     * @brief Check if element `a` and element `b` belong to the same set.
     * @param a First element.
     * @param b Second element.
     * @return true if connected, false otherwise.
     */
    bool same(int a, int b) {
        return find(a) == find(b);
    }

    /**
     * @brief Get the size of the set containing `v`.
     * @param v The element.
     * @return Size of the set.
     */
    int size(int v) {
        return sz[find(v)];
    }

    /**
     * @brief Get the total number of connected components.
     * @return Number of components.
     */
    int count_components() const {
        return num_components;
    }
};
