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
