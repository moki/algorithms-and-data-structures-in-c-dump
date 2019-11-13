# 3 Balanced Search Trees

Tree operations have complexity bounded by O(h), where h is the height of the tree.
But the height can be as large as n; in fact linear list can be a correct search tree, which is really inefficient.

The key to usefulness of the search trees is to keep them balanced, that is, to keep height bounded by O(log2 n), instead of O(n).

The first method that achieved it is height-balanced tree, now frequently referred to as AVL Tree.

There are many other methods that achieve similar bounds.

1. [Height Balanced Search Trees](./height/README.MD)
2. [Weight Balanced Search Trees](./weight/README.MD)
3. [(a, b)- and B-Trees](./a_b/README.MD)