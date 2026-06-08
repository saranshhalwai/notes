# Repository Index & Notes

Welcome to the notes and algorithms repository. This repository contains implementations of fundamental C++ data structures and a comprehensive SQL reference.

---

## Repository Structure

| File | Type | Description | Key Features |
| :--- | :--- | :--- | :--- |
| [dsu.cpp](file:///home/saransh/code/notes/dsu.cpp) | C++ Source | Disjoint Set Union (DSU) / Union-Find | Path compression, union by size, component counting. |
| [segtree.cpp](file:///home/saransh/code/notes/segtree.cpp) | C++ Source | Segment Tree template implementation | Point updates, range queries, custom merge operations. |
| [sql.md](file:///home/saransh/code/notes/sql.md) | Markdown | Comprehensive SQL reference & cheatsheet | LeetCode SQL 50 patterns, window functions, regex, CTEs. |
| [.markdownlint.json](file:///home/saransh/code/notes/.markdownlint.json) | JSON | Markdown lint configuration | Disables `MD013` (line length rule). |

---

## File Details

### 1. Disjoint Set Union ([dsu.cpp](file:///home/saransh/code/notes/dsu.cpp))

An efficient implementation of the Disjoint Set Union / Union-Find data structure.

* **Class**: [DSU](file:///home/saransh/code/notes/dsu.cpp#L10)
* **Key Methods**:
  * [DSU(int n)](file:///home/saransh/code/notes/dsu.cpp#L21) - Constructor initializing `n` elements (0 to n-1).
  * [find(int v)](file:///home/saransh/code/notes/dsu.cpp#L31) - Find the representative of the set containing `v` with **path compression** ($O(\log N)$ worst-case, amortized $O(\alpha(N))$).
  * [unite(int a, int b)](file:///home/saransh/code/notes/dsu.cpp#L45) - Merges two sets using **union by size**.
  * [same(int a, int b)](file:///home/saransh/code/notes/dsu.cpp#L66) - Checks if `a` and `b` belong to the same set.
  * [size(int v)](file:///home/saransh/code/notes/dsu.cpp#L75) - Returns the size of the set containing `v`.
  * [count_components()](file:///home/saransh/code/notes/dsu.cpp#L83) - Returns the total number of disjoint components.

> [!TIP]
> Path compression and union by size yield a nearly-constant amortized time complexity of $O(\alpha(N))$ per operation, where $\alpha$ is the Inverse Ackermann function.

---

### 2. Segment Tree ([segtree.cpp](file:///home/saransh/code/notes/segtree.cpp))

A standard 0-indexed Segment Tree represented as a flat array of size $4N$. It supports point updates and range queries, and can be customized with generic types and custom merge operations.

* **Class Template**: [SegmentTree<T>](file:///home/saransh/code/notes/segtree.cpp#L10)
* **Key Methods**:
  * [SegmentTree(int size, T id, std::function<T(const T&, const T&)> op)](file:///home/saransh/code/notes/segtree.cpp#L63) - Constructor specifying the tree size, identity element, and merge function.
  * [SegmentTree(const std::vector<T>& arr, T id, std::function<T(const T&, const T&)> op)](file:///home/saransh/code/notes/segtree.cpp#L72) - Constructor that builds the Segment Tree from an initial vector `arr`.
  * [update(int idx, const T& val)](file:///home/saransh/code/notes/segtree.cpp#L84) - Performs a point update at `idx`.
  * [query(int l, int r)](file:///home/saransh/code/notes/segtree.cpp#L96) - Queries the combined value of the range `[l, r]` (inclusive) in $O(\log N)$ time.

> [!NOTE]
> The identity element `id` varies by operation: use `0` for sums, `1` for products, `INF` for minimums, and `-INF` for maximums.

---

### 3. SQL Reference ([sql.md](file:///home/saransh/code/notes/sql.md))

My personal SQL reference guide covering LeetCode top SQL 50 patterns, date arithmetic, aggregation strategies, window functions, and regular expressions.

* **Sections**:
  * [At a glance](file:///home/saransh/code/notes/sql.md#L5) - Quick recipes for complex SQL questions (e.g., Rising Temperature, Average Process Time, Group Sold Products).
  * [Syntax Reference](file:///home/saransh/code/notes/sql.md#L148) - Deep-dives into core SQL syntax, execution order, window functions (`ROW_NUMBER()`, `RANK()`, `DENSE_RANK()`, `LAG()`, `LEAD()`), and conditional aggregation using `CASE WHEN`.
  * [Regex](file:///home/saransh/code/notes/sql.md#L447) - PostgreSQL native regex operators, character classes, quantifiers, and differences compared to MySQL syntax.

> [!IMPORTANT]
> **Logical Execution Order in SQL**:
>
> 1. `FROM` / `JOIN`
> 2. `WHERE` (filters rows)
> 3. `GROUP BY` (groups rows into buckets)
> 4. `HAVING` (filters aggregate buckets)
> 5. Window Functions
> 6. `SELECT` (picks output columns)
> 7. `ORDER BY` (sorts final output)
