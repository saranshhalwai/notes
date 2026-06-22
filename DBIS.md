# Database and Information Systems notes

> [!NOTE]
> This is not intended to be comprehensive.

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

### First Normal form

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
* Atmost one clustering index for database table.

#### Secondary Index

* Secondary indexed is used when data is unordered.
* No of records in Index Table = No of records in HD.
* Secondary indices have to be dense
* Search complexity = log(N) + 1, where N is number of blocks in Index table (where secondary search based on KEY)
  If secondary search is based on non key then we need to maintain intermediate layer that is block of record pointers. Search complexity = log(N) + 1 + 1

> [!NOTE]
> Dynamic Mulitlevel Index is just nested indexes. Can cause problems with inserting or deleting. Trees are used to solve this problem.

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
|--------|--------|
|Data is stored in leaf as well as internal nodes | Data is stored only in leaf nodes |
| Searching is slower | Searching is faster |
| No redundant search key present | Redundant keys would present |
| Leaf nodes are not linked together | Leaf nodes are linked together |
