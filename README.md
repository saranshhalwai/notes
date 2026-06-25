# Repository Index & Notes

Welcome to the notes and algorithms repository. This repository contains implementations of fundamental C++ data structures and a comprehensive SQL reference.

---

## Repository Structure

| File | Type | Description | Key Features |
| :--- | :--- | :--- | :--- |
| [DSA/dsu.cpp](DSA/dsu.cpp) | C++ Source | Disjoint Set Union (DSU) / Union-Find | Path compression, union by size, component counting. |
| [DSA/segtree.cpp](DSA/segtree.cpp) | C++ Source | Segment Tree template implementation | Point updates, range queries, custom merge operations. |
| [DSA/fenwick_tree.cpp](DSA/fenwick_tree.cpp) | C++ Source | Fenwick Tree (Binary Indexed Tree) | 0-indexed interface, prefix & range sum queries, $O(\log N)$ updates. |
| [DSA/trie_alphabet.cpp](DSA/trie_alphabet.cpp) | C++ Source | Trie (Prefix Tree) for strings | Lowercase English letters, insertion, search, prefix match, recursion removal. |
| [DSA/trie_binary.cpp](DSA/trie_binary.cpp) | C++ Source | Binary Trie (Bitwise Trie) | Integer insertion, reference-counted removal, max/min XOR query. |
| [OOPS.md](OOPS.md) | Markdown | OOPS quick reference & cheatsheet | Pillars, virtual functions/destructors, vtable/vptr, diamond problem, object slicing. |
| [DBIS/DBIS.md](DBIS/DBIS.md) | Markdown | Database and Information Systems notes | Normalisation, Transaction & Concurrency control (MVCC), Indexing, Query Optimisation, and Storage/Buffer Pool Internals. |
| [DBIS/sql.md](DBIS/sql.md) | Markdown | Comprehensive SQL reference & cheatsheet | LeetCode SQL 50 patterns, window functions, regex, CTEs. |
| [.markdownlint.json](.markdownlint.json) | JSON | Markdown lint configuration | Disables `MD013` (line length rule). |

---

## File Details

### 1. Disjoint Set Union ([DSA/dsu.cpp](DSA/dsu.cpp))

An efficient implementation of the Disjoint Set Union / Union-Find data structure.

* **Class**: [DSU](DSA/dsu.cpp#L10)
* **Key Methods**:
  * [DSU(int n)](DSA/dsu.cpp#L21) - Constructor initializing `n` elements (0 to n-1).
  * [find(int v)](DSA/dsu.cpp#L31) - Find the representative of the set containing `v` with **path compression** ($O(\log N)$ worst-case, amortized $O(\alpha(N))$).
  * [unite(int a, int b)](DSA/dsu.cpp#L45) - Merges two sets using **union by size**.
  * [same(int a, int b)](DSA/dsu.cpp#L67) - Checks if `a` and `b` belong to the same set.
  * [size(int v)](DSA/dsu.cpp#L76) - Returns the size of the set containing `v`.
  * [count_components()](DSA/dsu.cpp#L83) - Returns the total number of disjoint components.

> [!TIP]
> Path compression and union by size yield a nearly-constant amortized time complexity of $O(\alpha(N))$ per operation, where $\alpha$ is the Inverse Ackermann function.

---

### 2. Segment Tree ([DSA/segtree.cpp](DSA/segtree.cpp))

A standard 0-indexed Segment Tree represented as a flat array of size $4N$. It supports point updates and range queries, and can be customized with generic types and custom merge operations.

* **Class Template**: [SegmentTree<T>](DSA/segtree.cpp#L11)
* **Key Methods**:
  * [SegmentTree(int size, T id, std::function<T(const T&, const T&)> op)](DSA/segtree.cpp#L63) - Constructor specifying the tree size, identity element, and merge function.
  * [SegmentTree(const std::vector<T>& arr, T id, std::function<T(const T&, const T&)> op)](DSA/segtree.cpp#L72) - Constructor that builds the Segment Tree from an initial vector `arr`.
  * [update(int idx, const T& val)](DSA/segtree.cpp#L84) - Performs a point update at `idx`.
  * [query(int l, int r)](DSA/segtree.cpp#L96) - Queries the combined value of the range `[l, r]` (inclusive) in $O(\log N)$ time.

> [!NOTE]
> The identity element `id` varies by operation: use `0` for sums, `1` for products, `INF` for minimums, and `-INF` for maximums.

---

### 3. Fenwick Tree ([DSA/fenwick_tree.cpp](DSA/fenwick_tree.cpp))

A template implementation of a Fenwick Tree (Binary Indexed Tree) for point updates and prefix/range queries.

* **Class Template**: [FenwickTree<T>](DSA/fenwick_tree.cpp#L10)
* **Key Methods**:
  * [FenwickTree(int n)](DSA/fenwick_tree.cpp#L20) - Constructor initializing a Fenwick Tree of size `n` with zeros.
  * [FenwickTree(const std::vector<T>& arr)](DSA/fenwick_tree.cpp#L26) - Constructor building the Fenwick Tree from an initial vector `arr`.
  * [add(int idx, T val)](DSA/fenwick_tree.cpp#L38) - Adds `val` to the element at `idx` (0-indexed).
  * [query(int idx)](DSA/fenwick_tree.cpp#L49) - Computes the prefix sum up to `idx` (0-indexed).
  * [query(int l, int r)](DSA/fenwick_tree.cpp#L62) - Queries the sum in the range `[l, r]` (inclusive) in $O(\log N)$ time.

---

### 4. Alphabet Trie ([DSA/trie_alphabet.cpp](DSA/trie_alphabet.cpp))

A Prefix Tree (Trie) for standard lowercase English words ('a'-'z'), supporting retrieval, prefix querying, and safe recursive word deletion.

* **Class**: [AlphabetTrie](DSA/trie_alphabet.cpp#L8)
* **Key Methods**:
  * [insert(const std::string& word)](DSA/trie_alphabet.cpp#L54) - Inserts a lowercase word into the Trie.
  * [search(const std::string& word)](DSA/trie_alphabet.cpp#L73) - Searches for a full word in the Trie.
  * [startsWith(const std::string& prefix)](DSA/trie_alphabet.cpp#L89) - Checks if any word starts with the given prefix.
  * [remove(const std::string& word)](DSA/trie_alphabet.cpp#L105) - Removes a word from the Trie and cleans up unused nodes recursively.

---

### 5. Binary Trie ([DSA/trie_binary.cpp](DSA/trie_binary.cpp))

A Bitwise Trie (Binary Trie) for storing integers, supporting point additions, safe reference-counted deletions, and bitwise optimization queries.

* **Class**: [BinaryTrie](DSA/trie_binary.cpp#L9)
* **Key Methods**:
  * [BinaryTrie(int bits)](DSA/trie_binary.cpp#L28) - Constructor specifying the maximum bits (default 30).
  * [insert(int num)](DSA/trie_binary.cpp#L34) - Inserts an integer into the Trie.
  * [remove(int num)](DSA/trie_binary.cpp#L49) - Safely deletes one instance of an integer from the Trie.
  * [count(int num)](DSA/trie_binary.cpp#L83) - Counts occurrences of `num` in the Trie.
  * [max_xor(int num)](DSA/trie_binary.cpp#L99) - Finds the maximum XOR sum with `num` in $O(\text{bits})$ time.
  * [min_xor(int num)](DSA/trie_binary.cpp#L124) - Finds the minimum XOR sum with `num` in $O(\text{bits})$ time.

---

### 6. OOPS Quick Reference ([OOPS.md](OOPS.md))

A quick reference and cheatsheet for Object-Oriented Programming (OOP) concepts, mechanisms, and common C++ specific implementations.

* **Key Topics**:
  * [Four pillars of OOPS](OOPS.md#L5) - Abstraction, Encapsulation, Inheritance, and Polymorphism.
  * [Polymorphism & Vtable/Vptr](OOPS.md#L11) - Difference between overloading and overriding; how virtual functions use vtables and vptrs under the hood.
  * [Virtual Destructor](OOPS.md#L39) - Ensuring correct cleanup when deleting derived objects via base pointers.
  * [Virtual Inheritance](OOPS.md#L52) - Fixing the diamond problem in C++.
  * [Object Slicing](OOPS.md#L59) - Understanding why derived parts are discarded when assigning to base objects, and how to avoid it.
  * [C++ Specifiers](OOPS.md#L76) - Using `override` and `final` (for classes and virtual functions).

> [!TIP]
> Virtual function calls resolve at runtime through dynamic/late binding via a compiler-generated **Vtable** (Virtual Table) lookup, referenced by an object's hidden **vptr** (virtual pointer).

---

### 7. DBIS Notes ([DBIS/DBIS.md](DBIS/DBIS.md))

A quick reference for Database and Information Systems concepts.

* **Sections**:
  * [Normalisation](DBIS/DBIS.md#L38) - Functional dependency properties, closures, canonical and minimal covers, 1NF, 2NF, 3NF, BCNF.
  * [Transaction and Concurrency Control](DBIS/DBIS.md#L148) - ACID properties, transaction states, serialisability, and protocols (2PL, Timestamp-ordering, MVCC).
  * [Indexing](DBIS/DBIS.md#L369) - Dense, sparse, primary, clustered, and secondary indexes; B-Trees and B+ Trees.
  * [Query Optimisation](DBIS/DBIS.md#L454) - Parsing, algebraic/heuristic rules, cost-based optimizer calculations (nested loops, sort-merge), and sargability.
  * [Database Internals](DBIS/DBIS.md#L531) - Page layout (slotted pages), log-structured storage, buffer pool replacement policies (LRU, Clock, LFU, ARC), and query processing models.

---

### 8. SQL Reference ([DBIS/sql.md](DBIS/sql.md))

My personal SQL reference guide covering LeetCode top SQL 50 patterns, date arithmetic, aggregation strategies, window functions, and regular expressions.

* **Sections**:
  * [At a glance](DBIS/sql.md#L5) - Quick recipes for complex SQL questions (e.g., Rising Temperature, Average Process Time, Group Sold Products).
  * [Syntax Reference](DBIS/sql.md#L148) - Deep-dives into core SQL syntax, execution order, window functions (`ROW_NUMBER()`, `RANK()`, `DENSE_RANK()`, `LAG()`, `LEAD()`), and conditional aggregation using `CASE WHEN`.
  * [Regex](DBIS/sql.md#L447) - PostgreSQL native regex operators, character classes, quantifiers, and differences compared to MySQL syntax.

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

