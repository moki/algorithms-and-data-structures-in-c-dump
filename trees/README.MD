# 2. Search Trees

Search Tree is a structure that stores objects, each object identified by a key value, in a tree structure.

The keys form a linearly ordered set, two keys can be compared in constant time, and these comparisons guide the access to a specific object in the structure.

Tree has a root, where any search starts, and then contains in each node key value to guide the search comparing it with query key, so one can go left if query key smaller than key in node one looks at and right otherwise until one finds node contains the right key.

Search tree is one of the ways to implement dictionaries, which is structure supporting at least these operations: find, insert, and delete. There are also other ways to implement dictionaries and there other applications of search tree but implementing the dictionaries.

## Implementations
1. [Basic Binary Search Tree](./binary/README.MD)