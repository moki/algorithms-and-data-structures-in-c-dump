# 3.1 Height Balanced Search Trees

## Description:
A tree is height-balanced if, in each interior node, the height of the right subtree and height of the left subtree differ by at most 1.

To keep track of this we need to store additional information in nodes.

```
typedef struct tree_node {
	tree_node_t  *edge[2];
	unsigned char height;
} tree_node_t;
```

Height of the node defined recursively by following rules:

1. If n is a leaf, then n->height is 0.
2. Otherwise n->height is one larger than the maximum of the height of the left right subtrees.

	`1 + Max(n->edge[0]->height, n->edge[1]->height);`

Height information must be corrected every time tree changes and must be used to keep the tree height balanced.

Tree can change only by insert and delete operations, and by any such operation the height can change only for the nodes that contain the changed leaf in their subtree.

## Rebalancing Cases:

If **n** is the current node

1. |n->edge[0]->height - n->edge[1]->height| <= 1

	In this case, no rebalancing is necessary in this node. If the height also did not change, then from this node upward nothing changed and we can finish rebalancing; otherwise, we need to correct the height of *n and go up to the next node.

2. |n->edge[0]->height - n->edge[1]->height| == 2

	In this case, we need to rebalance *n. This is done using the rotations(left/right), rules goes as follows:

	1. (n->edge[0]->height - n->edge[1]->height) == 2 &&

		(n->edge[0]->height - n->edge[1]->height) == 1

		Perform right rotation around n, followed by recomputation of the height in n->edge[1] and n.
	2. (n->edge[0]->height - n->edge[1]->height) == 2 &&

		(n->edge[0]->edge[0]->height - n->edge[1]->height) == 0

		Perform left rotation around n->edge[0], followed by a right rotation around n, followed by recomputing the height in n->edge[1], n->edge[0], and n.
	3. (n->edge[1]->height - n->edge[0]->height) == 2 &&

		(n->edge[1]->edge[1]->height - n->edge[0]->height) == 1

		Perform left rotation around n, followed by recomputing the height in n->edge[0] and n.

	4. (n->edge[1]->height - n->edge[0]->height) == 2 &&

		(n->edge[1]->edge[1]->height - n->edge[0]->height) == 0

		Perform right rotation around n->edge[1], followed by a left rotation around n, followed by a recomputation of the height in n->edge[1], n->edge[0], and n.

After performing these rotations, we check wether the height of n changed by this: if not, we can finish rebalancing; otherwise we continue with the next node up, till we reach the root.

Rotations are O(1) , the path log n => at most rebalancing takes O(log n)

## Operations
   | Time Complexity | Return Type    | Operation name         | Arguments 		       	     |
   |:----------------|:---------------|:-----------------------|:------------------------------------|
   | O(1)	     | hbt_t *        | hbt\_constructor       | ()				     |
   |   	 	     | void           | hbt\_destructor        | (hbt\_t *hbt)		             |
   | O(log n)	     | void	      | hbt\_insert            | (hbt\_t *hbt, void *key, void *val) |
   | O(log n)        | void * 	      | hbt\_find              | (hbt\_t *hbt, void *key)	     |
   | O(log n)        | void * 	      | hbt\_delete            | (hbt\_t *hbt, void *key)	     |