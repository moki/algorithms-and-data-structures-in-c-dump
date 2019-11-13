# 2.1 Basic Binary Search Tree

## Description:
Model used here to implement trees is leaf trees, which basicaly means that object values that placed in the tree stored in the left pointer at the leaf node(external node), not in each node(sometimes, internal), how it is done in the node trees.

## Operations
   | Time Complexity | Return Type    | Operation name         | Arguments 		       	     |
   |:----------------|:---------------|:-----------------------|:------------------------------------|
   | O(1)	     | queue_t *      | bst\_constructor       | ()				     |
   |   	 	     | void           | bst\_destructor        | (bst\_t *bst)		             |
   | O(log n)	     | void	      | bst\_insert            | (bst\_t *bst, void *key, void *val) |
   | O(log n)        | void * 	      | bst\_find              | (bst\_t *bst, void *key)	     |
   | O(log n)        | void * 	      | bst\_delete            | (bst\_t *bst, void *key)	     |