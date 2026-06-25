# Database and Information Systems notes

> [!NOTE]
> This is not intended to be comprehensive. Also, it often quotes various [source material](#references) verbatim and at other times, is just my interpretation of a long block of text.  

> [!IMPORTANT]
> For SQL refer to [the SQL notes](./sql.md)

<!--
## Intro

1. Levels of abstraction
   * Physical level: Describes how a record (e.g., instructor) is stored
     * Location, name, size of Database in memory, indexing
   * Logical level: Describes data stored in database, and the relationships among the data.
     * It is a logical blueprint of Database

     ```
     type instructor = record
     ID : string;
     name : string;
     dept_name : string;
     salary : integer;
     end;
     ```
   * View level: Application programs hide details of data types. Views can also hide information (such as an employee’s salary) for privacy/security purposes
     * Only a part of the actual database is viewed by the users

2. Schema: Framework to describe the structure of a specific Database System.
   Three schema architecture: 
-->

## ER and relational model

* Strong entity sets have sufficient attributes to form a key.
* Weak entity sets: no key can be formed.

## Normalisation

### First Normal Form

All attributes must be atomic, i.e., no multi-valued attribute.

### Functional Dependency

Require that the value for a certain set of attributes determines uniquely the value for another set of attributes.

x->y
x uniquely determines y. Essentially, if two records have the same entry for x, they have to have the same entry for y.

#### Properties of Functional Dependency

* Reflexive rule: if 'b' is a subset of 'a', then a->b (trivial and valid)
* Augmentation rule: if a->b, then ca->cb (valid)
* Transitivity rule: if a->b, and b->c, then a->c (valid)
* Union rule: If a->b holds and b->c holds, then a->bc holds(valid)
* Decomposition rule: If a->bc holds, then a->b holds and b->c holds (valid)
* Pseudotransitivity rule: If a->b holds and cb->d holds, then ac->d holds (valid)
* Composition rule: If a->b holds and d->c holds, then ad->bc holds (valid)

#### Closure of a Set of Functional Dependencies

The set of all functional dependencies logically implied by F is the
closure of F.
We denote the closure of F by F+

* PA = All the attributes that are present in the set of CK
* NPA = {All the attributes of Relation} - PA

#### Canonical Cover

A canonical cover for F is a set of dependencies Fc such that

* F logically implies all dependencies in Fc, and
* Fc logically implies all dependencies in F, and
* No functional dependency in Fc contains an extraneous attribute
  * An attribute of a functional dependency is said to be extraneous if we can remove it without changing the closure of the set of functional dependencies
* Each left side of functional dependency in Fc is unique. That is, there are no two dependencies in Fc
  * a1->b1 and a2 -> b2 such that
  * a1 = a2

##### To compute a canonical cover for F

repeat
  Use the union rule to replace any dependencies in F of the form

    a1->b1 and a1->b2 with a1 ->b1 b2

  Find a functional dependency a->b in Fc with an extraneous attribute either in a or in b
  If an extraneous attribute is found, delete it from a->b
until (Fc not change)

> [!NOTE]
> Union rule may become applicable after some extraneous attributes have been deleted, so it has to be re-applied

#### Minimal Cover

A minimal cover cannot allow more than one attribute on the right hand side.

##### Method to find Minimal Cover

* First, make RHS of each FD as a single attribute or decompose RHS
  * Use decomposition rule
* Second, pick each functional dependency (FD) and check if other functional decencies (excluding current FD) can generate the same results/dependencies
  * If other FDs can generate, remove the current FD
    * Find the closure of LHS of each FD from other FDs (excluding current FD) and check if other dependencies can generate RHS of current FD. If so, remove current FD
  * This step is called removal of redundant FD.
* Third, pick those FDs that have at least 2 attributes at LHS and try to reduce it (preferably make it one attribute on the LHS). E.g. xy->z
  * Check if closure of one attribute (x) at LHS can derive other attribute (y) of LHS. If so, remove other attribute (y)
    * Similarly we can check if x can be removed
  * This step is called removal of extraneous attribute

### Second Normal Form

Conditions:

* Should be in 1st normal form.
* All NPA should be **fully** functionally dependent on CK.
  * i.e., there should not be any partial dependency.
    * Partial Dependency occurs when a non-prime attribute is functionally dependent on part of a candidate key

![Partial Dependency Illustration](./assets/partial_dep.png)

### Third Normal Form

Conditions:

* Should be in 2nd normal form.
* No transitive dependency.

### Boyce Codd Normal Form(BCNF)

* Should be in 3rd normal form
* All attributes should be functionally dependent on CK.

### Equivalence of Functional Dependency

Two FDs, F and G are equivalent if

* F covers G; and,
* G covers F

To check G is subset of F(or F covers G)

* For each FD in G, pick it’s LHS and take closure from F
* If all closure from F are able to determine FD in G then F covers G.

## Transaction and Concurrency Control

A transaction is a unit of program execution that accesses and possibly updates various data items.
OR
Set of operations used to perform a logical unit of work.

**Atomicity**: Either all operations or no operations.(No partial execution)
**Consistency**: A database must be consistent before and after a transaction(middle exempted). Consistency here implies non-violation of integrity constants and the like.
**Isolation**: Running of one transaction should not affect another. (Running transactions should give the same results as running them serially, and no transaction should be able to access the intermediate state of another.)
**Durability**: Permanent changes in database after a transaction is completed successfully.(Even in case of system failures)

### Transaction state

* **Active** – the initial state; a transaction stays in this state while it is executing transaction is main memory and it is getting executed
* **Partially committed** – after the final statement has been executed transaction has completed all the operations except commit
* **Failed** – after the discovery that normal execution can no longer proceed
* **Aborted** – after the transaction has been rolled back and the database restored to its state prior to the start of the transaction. Two options after it has been aborted:
  * restart the transaction(can be done only if no internal logical error)
  * kill the transaction
* **Committed** – after successful completion
* **Terminated** – free the resources that were are used by transaction

![Pathway of a Transaction](./assets/txn_path.png)

### Schedules

**Schedule**:a sequences of instructions that specify the chronological order in which instructions of concurrent transactions are executed
Two types:

1. Serial Schedule
2. Parallel Schedule

| Serial Schedule | Parallel Schedule |
| --------------- | ----------------- |
| Transactions execute after one another | Transactions execute concurrently |
| Consistent | Can be inconsistent |
| High waiting time | Less waiting time |
| Low throughput | High throughput |
| Low performance | High performance |

### Conflicts

Two operations in a schedule conflict if they belong to different transactions, access the same data item, and at least one is a write (i.e., Read-Write, Write-Read, and Write-Write conflicts).

* **Recoverable schedule**: if a transaction Tj reads a data item previously written by a transaction Ti, then the commit operation of Ti appears before the commit operation of Tj.
* **Cascading rollback**: a single transaction failure leads to a series of transaction rollbacks.
* **Cascadeless schedules**: cascading rollbacks cannot occur. For each pair of transactions Ti and Tj such that Tj reads a data item previously written by Ti, the commit operation of Ti appears before the read operation of Tj.
* **Strict recoverable**: If Ti writes a data item, any subsequent read or write of that item by Tj must be delayed until Ti commits or aborts.

> [!IMPORTANT]
> **Recoverable <= Cascadeless <= Strict <= Serial schedule** (left is less strict and thus a superset)

### Serialisability

Two types:

1. Conflict Serialisable
2. View Serialisable

#### Conflict Serialisability

If a schedule S can be transformed into a schedule S’ by a series of swaps of non-conflicting instructions, we say that S and S’ are **conflict equivalent**.
A schedule S is **conflict serialisable** if it is conflict equivalent to a serial schedule.

##### Precedence graph

Txns are nodes. Make an edge when Ti and Tj conflict originating from the txn that accessed the conflicting data first.
If there is a cycle, this schedule is not conflict serialisable.
If no cycle, then serialisability order is obtained by a topological sorting of the graph.

#### View Serialisability

Three Conditions:

1. If in schedule S, transaction Ti reads the initial value of Q, then in schedule S’ also transaction Ti must read the initial value of Q. (Initial read)
2. If in schedule S transaction Ti executes read(Q), and that value was produced by transaction Tj(if any), then in schedule S’ also transaction Ti must read the value of Q that was produced by the same write(Q) operation of transaction Tj (WR Sequence).
3. The transaction (if any) that performs the final write(Q) operation in schedule S must also perform the final write(Q) operation in schedule S’ (Final Write).

A schedule S is view serializable if it is view equivalent to a serial schedule.

> [!IMPORTANT]
> **All Schedules <= View Serialisable <= Conflict Serialisable <= Serial Schedule

### Concurrency Control Protocols

#### Shared-Exclusive locking

Exclusive lock(X) and shared locks(S) obtainable on data items. Exclusive can read and write. Shared can only read. Requests made to concurrency control manager.
A locking protocol is a set of rules followed by all transactions while requesting and releasing locks.

**Deadlocks** possible when using locks.
**Starvation** is when a transaction requesting an X-lock waits endlessly while multiple transactions request and are granted an S-lock.

#### Two-Phase Locking(2PL)

Phase 1: Transactions obtain locks.(And do not release)
Phase 2: Transactions release locks. (No new requests)

* **Strict 2PL**: Basic 2PL and all X-locks hold until commit.
* **Rigorous 2PL**: Basic 2PL and all locks hold until commit.
* **Conservative 2PL**: Lock all the items it will access before the Transaction begins.

#### Timestamp-Based protocols

Timestamp assigned may not be real time. Timestamp order = serialisability order

##### Timestamp-Ordering Protocol

Maintains for each data Q two timestamp values:

* W-timestamp(Q) is the largest time-stamp of any transaction that executed write(Q) successfully.
* R-timestamp(Q) is the largest time-stamp of any transaction that executed read(Q) successfully.

For reads, compare with write timestamp
For writes, compare with both read and write timestamps.

* Always ensures serialisability
* Free from deadlock.
* Does not ensure recoverable or cascadeless schedules.

###### Strict Timestamp-Ordering Protocol

A Transaction T that issues a R_item(X) or W_item(X) such that TS(T) > W_TS(X) has its read or write operation delayed until the Transaction T‘ that wrote the values of X has committed or aborted

* Ensures recoverable and cascadeless schedules
* Free from deadlock
* Ensures serializability

#### Multi Version Concurrency Control

With MVCC, the DBMS maintains multiple *physical* versions of a single *logical* object in the database. When a transaction writes to an object, the DBMS creates a new version of that object. When a transaction reads an object, it reads the newest version that existed when the transaction started.

The fundamental concept/benefit of MVCC is that writers do not block readers and readers do not block writers.

One advantage of using MVCC is that read-only transactions can read a consistent snapshot of the database without using locks of any kind, and it naturally supports Snapshot Isolation (SI)

A typical MVCC-based database design will:

1. Have a versioned storage which stores different versions of the same logical object. (Note: Do not do this!)
2. Takes a snapshot of the database (by copying the transaction status table) when a transaction starts.
3. Use the snapshot to determine which versions of objects are visible to the transaction.

##### Snapshot Isolation

Snapshot Isolation involves providing a transaction with a consistent snapshot of the database when the transaction started. Data values from a snapshot consist of only values from committed transactions, and the transaction operates in complete isolation from other transactions until it finishes.

**Write Conflicts**: If two transactions update the same object, the first writer wins
**Write Skew Anomaly**  can occur in Snapshot Isolation when two concurrent transactions modify different objects resulting in non-serializable schedules. For example, if one transaction changes all white marbles to black and the other changes all black marbles to white, the outcome may not correspond to any serializable schedule.

##### Serialisable Snapshot Isolation

Here is an anti-dependency from a transaction T1 to a transaction T2 if T1 reads a version of item x, and T2 produces a version of x that is later in the version order (i.e. newer) than the version read by T1. If there is an anti-dependency from T1 to T2 and from T2 to T1, serializability is violated and at least one of T1 and T2 must be aborted and retried.

There are five important MVCC design considerations:

1. Concurrency Control Protocol
2. Version Storage
3. Garbage Collection
4. Index Management
5. Deletes

##### 1. Concurrency Control Protocol

The choice of concurrency protocol is between the approaches discussed above (two-phase locking, timestamp ordering, optimistic concurrency control).

##### 2. Version Storage

This determines how the DBMS stores the different physical versions of a logical object and how transactions find the newest version visible to them. The DBMS uses the tuple’s pointer field to create a **version chain** (a linked list of versions sorted by timestamp). Indexes always point to the head of the chain.

* **Append-Only Storage**: All physical versions of a logical tuple are stored in the same table space. Every update appends a new version to the table and updates the version chain.
  * *Oldest-to-Newest (O2N)*: Requires traversing the chain on lookups.
  * *Newest-to-Oldest (N2O)*: Must update index pointers for every new version, but avoids chain traversal on lookups. (Usually preferred as most transactions care about the newest version).
* **Time-Travel Storage**: The DBMS maintains a separate *time-travel table* for older versions. On update, the DBMS copies the old version to the time-travel table, overwrites the main table in-place with the new data, and points the main table tuple pointer to the time-travel table.
* **Delta Storage**: Like time-travel storage, but stores only *deltas* (changes/diffs) in a *delta storage segment* instead of full past tuples. Recreates older versions by iterating through deltas in reverse order. Results in faster writes but slower reads.

###### Versioning with Large Values (Overflow Pages)

If a transaction does not modify data in columns using overflow pages, the DBMS reuses overflow pointers instead of copying data.

* *Reference Counting*: Uses an internal metadata table to track the number of physical versions pointing to an overflow page.
* *Garbage Collection*: Uses the version chain to identify which physical tuples point to the same overflow page before reclaiming space (more common).

##### 3. Garbage Collection

The DBMS must remove *reclaimable* physical versions over time. A version is reclaimable if no active transaction can see it, or if it was created by an aborted transaction.

* **Tuple-level GC**: DBMS finds old versions by examining tuples directly.
  * *Background Vacuuming*: Separate threads periodically scan the table for reclaimable versions.
    * *Optimization*: A **dirty page bitmap** is maintained to skip unmodified pages.
  * *Cooperative Cleaning*: Worker threads identify and prune reclaimable versions while traversing the version chain (only works for O2N chains; un-accessed data is never cleaned).
* **Transaction-level GC**: Each transaction tracks its own old versions using its read/write sets. When a transaction completes, the garbage collector reclaims the associated tuples directly without scanning tables.

###### PostgreSQL Transaction ID Wraparound

PostgreSQL uses fixed-size 32-bit transaction IDs. Long-running clusters could suffer from transaction ID wraparound (counter resets to 0, making past transactions appear in the future).

* *Fix*: Background Vacuum scans every tuple and marks rows as **frozen** (meaning they were committed sufficiently far in the past).

###### Block Compaction

Coalescing/compacting less-than-full data blocks into fewer blocks and releasing empty blocks (e.g. `VACUUM FULL` in PostgreSQL). Compaction groups tuples likely to be accessed together:

* *Time Since Last Update*: Uses `BEGIN-TS` to group tuples modified around the same time.
* *Time Since Last Access*: Requires maintaining `READ-TS` on tuples (expensive).
* *Application-level Semantics*: Identifies higher-level relations among tuples from the same table.

##### 4. Index Management

All primary key (pkey) indexes point to the version chain head. If a transaction updates a pkey attribute, it is treated as a `DELETE` followed by an `INSERT`.
Secondary index management is more complex and has two main approaches:

* **Logical Pointers**: The DBMS uses a fixed tuple identifier that does not change. An extra indirection layer maps the logical ID to the physical location of the tuple. Updates to tuples only require updating the mapping in the indirection layer.
* **Physical Pointers**: Secondary indexes store the physical address of the version chain head. This requires updating every secondary index whenever the version chain head is updated (very expensive).

###### MVCC Duplicate Key Problem

Indexes usually do not store version information. They must support duplicate keys because different snapshots might point to different physical versions of the same logical key. Workers may get multiple entries on a single fetch and must follow pointers to find the correct physical version.

##### 5. Deletes

The DBMS physically deletes a tuple only when all versions of a logically deleted tuple are no longer visible. A logically deleted tuple cannot have new versions created after its deletion (first-writer wins, no write-write conflicts).
To denote logical deletion:

* **Deleted Flag**: A flag in the tuple header or a separate column indicates the tuple is logically deleted.
* **Tombstone Tuple**: An empty physical version indicates logical deletion. A special bit pattern in the version chain pointer is used to reduce storage overhead.

###### Representation of Deleted Tuples (Slot Management)

* *Reuse Slot*: Allow new tuples to be inserted back into vacated slots. Easy in append-only storage; hurts temporal locality in delta storage due to intermixing.
* *Leave Slot Unoccupied*: Workers cannot insert new tuples into previously occupied slots, keeping new versions physically close. Requires a separate compaction mechanism to reclaim empty slots.

## Indexing

**Search key**: Attribute to set of attributes used to look up records in a file.
**Index file**: An index file consists of records (called index entries) of the form

| Search key | pointer |
|------------|---------|

* Index table block sizes are same as block size of SM or PM.
* Keys in index table are always sorted and unique.

### Types of Indexing

* **Dense Index**: Index record appears for every search-key value in the file.
* **Sparse Index**: contains index records for only some search-key.
  * Applicable when records are sequentially ordered on search-key.
  * To locate a record with search-key value K we:
    * Find index record with largest search-key value < K
    * Search file sequentially starting at the record to which the index record points

### Types of Indexes

* Primary index
* Clustered index
* Secondary index

|                | Primary Index   | Secondary Index |
|----------------|-----------------|-----------------|
| Ordered File   | Primary Index   | Clustered Index |
| Unordered File | Secondary Index | Secondary Index |

#### Primary Index

* Applied when data is sorted & search key is unique or primary key.
* Follows sparse indexing
* Number of entries in Index Table = Number of blocks in HD
* Search Time in Primary Index = log(N) + 1; where N is the number of blocks in index table.

#### Clustered Index

* used when there is a ordered file and search key is a non key.
* follows sparse indexing.
* Best case search time = log(N) + 1; where N is the number of blocks in index table
  Worst case search time = log(N) + 1 + 1 (this can be more than 1, worst case no of blocks in HD)
* At most one clustering index for database table.

#### Secondary Index

* Secondary index is used when data is unordered.
* No of records in Index Table = No of records in HD.
* Secondary indices have to be dense.
* Search complexity = log(N) + 1, where N is number of blocks in Index table (where secondary search based on KEY).
  If secondary search is based on non-key, we maintain an intermediate layer of record pointers. Search complexity = log(N) + 1 + 1.

> [!NOTE]
> Dynamic Multilevel Index is just nested indexes. Can cause problems with inserting or deleting. Trees are used to solve this problem.

### B-Tree

* B-Tree is a balanced tree.

B-Tree node properties:

1. Block pointer/Tree pointer
2. Keys
3. Data pointer/Record Pointer.

**Order of B-Tree**: Max no of children a node can have.

| Children | Root | Intermediate or leaf node |
|----------|------|---------------------------|
| Max      | p    | p                         |
| Min      | 2    | ceil(p/2)                 |

Data is inserted in sorted order, like binary search tree.

### B+Tree

| B-Tree | B+Tree |
| ------ | ------ |
| Data is stored in leaf as well as internal nodes | Data is stored only in leaf nodes |
| Search time is variable (can be faster if key is in root/internal nodes) | Search time is constant (must traverse to leaf, but faster for range queries) |
| No redundant search key present | Redundant search keys are present |
| Leaf nodes are not linked together | Leaf nodes are linked together |

## Query Optimisation

### Query Processing Pipeline

1. **Scanning, Parsing, and Validation**: SQL query text is scanned into tokens, parsed for syntax, and validated against system catalog. Outputs logical query tree.
2. **Query Optimizer**: Evaluates equivalent relational algebra expressions to select the cheapest physical execution plan.
3. **Query Code Generator**: Translates physical plan into executable low-level code.
4. **Runtime Database Processor**: Executes code against physical database storage and returns results.

### Techniques of Query Optimisation

Two strategies: Heuristic Rules and Cost Estimation.

#### Heuristic Rules (Algebraic Optimization)

Rule-based transformations that rearrange the relational algebra tree to reduce the size of intermediate relation tables early.

* **Push Selections (σ) Down**: Perform WHERE filters as early as possible to reduce number of tuples processed in subsequent operations (e.g. joins).
* **Push Projections (π) Down**: Perform SELECT column filters early. Reduces memory footprint of intermediate blocks in the buffer pool.
* **Optimize Join Order**: Rearrange joins so that smaller tables are joined first, avoiding massive intermediate Cartesian products.

#### Cost-Based Optimization

Estimates computational resources required for different physical execution paths; selects plan with lowest cost.

##### Cost Metrics

* **Access Cost to Secondary Storage (C_IO)**: Number of physical disk block accesses (usually dominant cost factor).
* **Storage Cost (C_storage)**: Cost of writing intermediate scratchpads to disk if memory is exceeded.
* **Computation Cost (C_CPU)**: CPU cycles spent on sorting, hashing, or executing operations on tuples.
* **Memory Usage Cost (C_RAM)**: Number of page/frame allocations in buffer pool.
* **Communication Cost (C_network)**: Network latency/transfer in distributed databases.

##### Catalog Information (Data Dictionary Stats)

Optimizer relies on statistical snapshots in system catalog:

* r: Total number of records (tuples) in the relation.
* R: Average record size in bytes.
* b: Total number of storage blocks required for the relation.
* bfr: Blocking factor (number of records per block).
* d: Number of distinct values for a given attribute.
* s: Selectivity (fraction of tuples satisfying the condition).

##### Join Cost Calculations

###### Single Loop Join (Index Nested Loop Join)

Used when outer relation loop searches inner relation using a secondary index on the join attribute.
Let bE, rE be blocks and records of outer table (Emp), bD, rD be blocks and records of inner table (Dept), and x be the number of index levels.

* **Looping through Emp first (Emp as outer)**:
  Cost = bE + (rE *(x + 1))
  *Example*: bE = 2000, rE = 6000, x = 3:
  2000 + (6000* 3) = 20,000 block accesses.

* **Looping through Dept first (Dept as outer)**:
  Cost = bD + (rD *(x + 1))
  *Example*: bD = 10, rD = 50, x = 5:
  10 + (50* 5) = 260 block accesses.

###### Sort-Merge Join

For equality join conditions. Inputs are sorted by join key and scanned concurrently.

* **If tables are already sorted**:
  Cost = bD + bE
* **If tables are unsorted**:
  Cost = bE + bD + (bE *log2(bE)) + (bD* log2(bD))

### Key Concepts

* **EXPLAIN / EXPLAIN ANALYZE**: Command to view the execution tree generated by the optimizer. Shows whether engine uses sequential scan or index scan.
* **Sargable Queries (Search Argument Able)**: Query conditions in WHERE clause that allow the optimizer to use indexes.
  * *Non-Sargable*: `WHERE YEAR(join_date) = 2026` (forces full table scan because function runs on every row).
  * *Sargable*: `WHERE join_date >= '2026-01-01' AND join_date <= '2026-12-31'` (allows B+ tree index seek).

## Database Internals

### Database Storage

* **Directory Page**: The first page in database files, acting as a directory of pages.
* **Buffer Pool**: Memory region for temporarily storing pages; manages data movement between disk and memory.
* **Execution Engine**: Executes queries by requesting specific pages from the buffer pool and operating on the retrieved memory pointer.

#### Database Pages

* **Page**: Fixed-size block of data representing the unit of transfer between disk and memory.
* **Page ID**: Unique identifier per page (can be instance-wide, database-wide, or table-wide).
* **Page Types**:
  1. Hardware page (typically 4 KB, hardware guarantees atomic write at this size).
  2. OS page (typically 4 KB).
  3. Database page (typically 1–16 KB).
* **Database Heap (Heap File)**: Unordered collection of pages storing tuples in random order.

#### Page Layout

* **Page Header**: Contains page metadata (size, checksum, DBMS version, transaction visibility, self-containment info).
* **Layout Approaches**: Slotted pages (Tuple-Oriented), Log-Structured, and Index-Organised.

##### Slotted Pages (Tuple-Oriented)

The entire tuple is stored in the page. The page maps slots to offsets:

* **Header**: Tracks number of used slots, offset of last used slot, and a slot array mapping to each tuple's start.
* **Growth Direction**: Slot array grows from start of page to end; tuple data grows from end of page to start. Page is full when they meet.
* **Record ID (RID)**: Unique logical tuple identifier representing its physical location (e.g. `[file_id, page_id, slot_number]`). Size: 4–10 bytes.
* **Tuple Header**: Metadata (transaction visibility, NULL value bitmap). Does not store database schema.
* **Tuple Data**: Attribute values stored sequentially (usually word-aligned). Most DBMSs restrict tuple size to page size.

###### Operations in Slotted Pages

* **Retrieval**: Find page position via directory $\rightarrow$ fetch page to buffer pool $\rightarrow$ locate tuple offset via slot array.
* **Insertion**: Locate page with free slot $\rightarrow$ verify space via slot array $\rightarrow$ write tuple and update slot array.
* **Update**: Find tuple via Record ID. If new value fits in-place, overwrite. Otherwise, mark old value deleted and insert new value as a new tuple.

###### Limitations

* **Fragmentation**: Deletions leave empty slots/gaps, leading to under-utilized pages.
* **Useless Disk I/O**: Block-oriented transfers require fetching the entire page to update a single tuple.
* **Random Disk I/O**: Non-sequential page updates force slow, random disk head jumps.

#### Log-Structured Storage

Appends log records of tuple modifications sequentially, based on Log-Structured File Systems (LSFS) and LSM Trees, avoiding in-place updates.

* **MemTable**: In-memory data structure where modifications (PUT/DELETE logs) are written first.
* **SSTable (Sorted String Table)**: Immutable on-disk files written sequentially once the MemTable fills up. Tuples are stored sorted by key.
* **Read Path**: Check MemTable $\rightarrow$ scan SSTables from newest to oldest (using binary search).
  * *Optimisation*: Maintain an in-memory **SummaryTable** (min/max keys per SSTable) and a **Bloom Filter** per level to skip checking SSTables that do not contain the key.

##### Compaction

Periodically merges SSTables using a sort-merge algorithm to discard old versions/deleted tuples, reducing disk usage and read latency.

* **Universal Compaction**: SSTables are kept in a single level. Compaction triggers when file count or overlapping key ranges exceed thresholds. Best for write-heavy/time-series workloads.
* **Level Compaction**: SSTables are grouped into levels (Level 0, Level 1, etc.). Key ranges are sorted and non-overlapping within each level (except Level 0). Compacting Level L merges files into Level L+1. Best for read-heavy workloads.

##### Tradeoffs

* **Pros**: Fast sequential writes; matches append-only cloud storage; no random disk writes.
* **Cons**: Slower read times (potentially checking multiple SSTables); expensive compaction overhead.
* **Write Amplification**: Multiple physical disk writes (during compaction) for a single logical write.

#### Index-Organised Storage

DBMS directly stores a table's tuples as the values inside an index data structure (e.g., B+ Tree). Uses a slotted page layout; tuples are sorted by key.

#### Data Representation

* **Integers (INTEGER/BIGINT/SMALLINT)**: Stored in native C/C++ format.
* **Reals (FLOAT/REAL vs. NUMERIC/DECIMAL)**:
  * *Variable Precision (FLOAT/REAL)*: Inexact, stored using IEEE-754 standard. Fast due to hardware CPU support.
  * *Fixed Precision (NUMERIC/DECIMAL)*: Exact, stored as variable-length binary; supports arbitrary precision/scale.
* **Variable Length (VARCHAR/VARBINARY/TEXT/BLOB)**: Stored as a header (containing length) followed by data bytes, or a pointer to an overflow page.
  * *Overflow Pages*: Used when value exceeds page size. A Record ID points to the overflow page. Can store a prefix inline to avoid page fetching on scans.
  * *External Value Storage*: Stores massive values in external files (BLOBs). The DBMS does not provide transaction or durability guarantees for external files.
* **Temporal (TIME/DATE/TIMESTAMP)**: Stored as 32/64-bit integers representing micro/milliseconds since Unix epoch.
* **Null Values**:
  * *Null Column Bitmap*: A bitmap in the page header; a bit is set if the attribute is NULL (common in row-stores).
  * *Special Values*: Reserved values (e.g., `INT32_MIN`) designate NULL (common in column-stores).
  * *Per-Attribute Flag*: A flag stored alongside each attribute; bad for word alignment padding.

#### System Catalogs

Internal tables containing metadata used to decipher database contents:

* Database objects (tables, views, columns, indexes, procedures).
* User access controls and permissions.
* Table statistics (e.g., cardinality, min/max values).
* Catalog tables are stored inside the database and initialized via bootstrap code.

### Buffer Pool

In-memory cache representing an array of fixed-size frames; operates as a write-back cache for pages.

* **Page Directory**: On-disk mapping of page IDs to physical file locations. Changes must be flushed to disk immediately for crash recovery.
* **Buffer Pool Metadata**:
  * *Page Table*: In-memory hash table mapping page IDs to active buffer pool frame locations. Includes page metadata (dirty flag, pin counter, access tracking).
  * *Dirty Flag*: Set when a page is modified; alerts manager to write the page back to disk before eviction.
  * *Pin/Reference Counter*: Tracks active threads accessing the page. Pin count > 0 prevents page eviction.

#### Page Table vs. Page Directory

* **Page Directory**: Maps Page ID $\rightarrow$ Physical File location. Persisted on disk.
* **Page Table**: Maps Page ID $\rightarrow$ In-memory Buffer Pool frame. In-memory only.

#### Locks vs. Latches

| Feature | Locks | Latches |
| :--- | :--- | :--- |
| **Scope** | High-level logical primitives (database contents). | Low-level execution primitives (internal data structures). |
| **Duration** | Held for transaction duration. | Held for operation duration. |
| **Rollback** | Requires rollback capabilities. | No rollback capability required. |
| **Implementation** | Managed by lock manager. | Language primitives (mutexes, spinlocks). |

#### Buffer Replacement Policies

* **LRU (Least Recently Used)**: Evicts page that has not been accessed for the longest time.
* **CLOCK**: Approximates LRU using a reference bit per page. A sweeping hand checks pages: if bit is 1, set to 0; if bit is already 0, evict and replace with the new page (setting its bit to 1).
* **LFU (Least Frequently Used)**: Evicts page with the lowest access count.
* **ARC (Adaptive Replacement Cache)**: Dynamically balances recency and frequency by maintaining two lists (T1 for recency, T2 for frequency) and adjusting a target size parameter $p$.

#### Buffer Pool Optimisations

1. **Multiple Buffer Pools**: Reduces lock contention.
   * *Object IDs*: RIDs contain an object identifier to map objects to designated buffer pools.
   * *Hashing*: Hashes page IDs to distribute pages across buffer pools.
2. **Pre-fetching**: Retrieves pages sequentially before the execution engine requests them (e.g., table scans).
3. **Scan Sharing (Synchronized Scans)**: Allows multiple query cursors to attach to a single table scan to reuse pages in memory.
4. **Buffer Pool Bypass**: Bypasses the buffer pool (directly reading/writing to disk) for large sequential scans to avoid dirtying buffer frames.

### Query Processing

#### Query Plan

A DAG (typically a tree) of operators converting SQL to actions. Data flows from leaves to the root; root outputs the final query result.

* **Pipeline**: A sequence of operators where tuples flow continuously between operators without intermediate storage.
* **Pipeline Breaker**: An operator that must consume all input tuples from its child before emitting output (e.g., Joins [build side], Subqueries, Order By).

#### Processing Models

Defines the control flow (how operators are invoked) and data flow (how results are sent) for query plan execution. Output format can be whole tuples (NSM) or subsets of columns (DSM).

##### Iterator Model (Volcano / Pipeline)

Implements a `Next()` function on every operator:

* Nodes call `Next()` on their children recursively, pulling tuples one-by-one.
* Returns a single tuple or a null marker (if finished) per call.
* High virtual function call overhead and CPU branching.

##### Materialisation Model

Operators process inputs completely and emit output all at once:

* Implements an `Output()` function returning all tuples for the operator.
* Operator finishes completely; parents never have to call it again.
* Good for OLTP queries (small result sets); bad for OLAP (large intermediate tables).

##### Vectorisation Model

A hybrid of Iterator and Materialisation:

* Implements a `Next()` function returning a **batch (vector) of tuples** instead of a single tuple.
* Greatly reduces virtual function overhead while preserving pipelining.

#### Processing Direction

* **Top-to-Bottom (Pull)**: Begins at root and pulls data from children. Easy to limit results (e.g. `LIMIT`), but incurs virtual function overhead.
* **Bottom-to-Top (Push)**: Starts at leaves and pushes data up. Enhances cache/CPU register utilization, but harder to limit/control intermediate sizes.

## References

* Class PPT(Based on Silberschatz, Abraham, Henry F. Korth, and Shashank Sudarshan. Database system concepts. Vol. 6. New York: McGraw-Hill, 1997.)
* [Notes of CMU course on the same topic.](https://15445.courses.cs.cmu.edu/spring2026/schedule.html)
