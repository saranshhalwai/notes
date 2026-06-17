#include <vector>
#include <functional>
#include <iostream>

/**
 * @brief A standard Segment Tree for point updates and range queries.
 * 0-indexed, represented as a single flat array of size 4*N.
 * Can be customized by changing the data type `T`, the identity value, and the merge operation.
 */
template <typename T>
class SegmentTree {
private:
    int n;
    std::vector<T> tree;
    T identity;
    std::function<T(const T&, const T&)> merge_op;

    void build(const std::vector<T>& arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
            return;
        }
        int mid = start + (end - start) / 2;
        build(arr, 2 * node, start, mid);
        build(arr, 2 * node + 1, mid + 1, end);
        tree[node] = merge_op(tree[2 * node], tree[2 * node + 1]);
    }

    void update(int node, int start, int end, int idx, const T& val) {
        if (start == end) {
            tree[node] = val; // Or tree[node] += val depending on requirements
            return;
        }
        int mid = start + (end - start) / 2;
        if (idx <= mid) {
            update(2 * node, start, mid, idx, val);
        } else {
            update(2 * node + 1, mid + 1, end, idx, val);
        }
        tree[node] = merge_op(tree[2 * node], tree[2 * node + 1]);
    }

    T query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) {
            return identity;
        }
        if (l <= start && end <= r) {
            return tree[node];
        }
        int mid = start + (end - start) / 2;
        T left_res = query(2 * node, start, mid, l, r);
        T right_res = query(2 * node + 1, mid + 1, end, l, r);
        return merge_op(left_res, right_res);
    }

public:
    /**
     * @brief Construct a Segment Tree with a specified identity element and merging operation.
     * @param size Size of the array/range.
     * @param id Identity element (e.g., 0 for sum, INF for min, -INF for max).
     * @param op Merging function combining two nodes.
     */
    SegmentTree(int size, T id, std::function<T(const T&, const T&)> op) 
        : n(size), tree(4 * size, id), identity(id), merge_op(op) {}

    /**
     * @brief Construct and build a Segment Tree from an initial array.
     * @param arr The source array.
     * @param id Identity element.
     * @param op Merging function combining two nodes.
     */
    SegmentTree(const std::vector<T>& arr, T id, std::function<T(const T&, const T&)> op)
        : n(arr.size()), tree(4 * arr.size(), id), identity(id), merge_op(op) {
        if (n > 0) {
            build(arr, 1, 0, n - 1);
        }
    }

    /**
     * @brief Update element at index `idx` to value `val`.
     * @param idx The index (0-indexed).
     * @param val The new value.
     */
    void update(int idx, const T& val) {
        if (idx >= 0 && idx < n) {
            update(1, 0, n - 1, idx, val);
        }
    }

    /**
     * @brief Query the range [l, r] (inclusive).
     * @param l Left bound (0-indexed).
     * @param r Right bound (0-indexed).
     * @return The combined result of the range.
     */
    T query(int l, int r) {
        if (l < 0 || r >= n || l > r) {
            return identity;
        }
        return query(1, 0, n - 1, l, r);
    }
};
