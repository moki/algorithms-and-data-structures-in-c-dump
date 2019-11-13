# 3.2 Weight Balanced Search Trees

## Description:
One natural alternative balance criteria to height is to balance weight, the number of leaves, instead of the height of the subtree.

**Weight Balanced Trees** first were introduced as *trees of bounded balance*, or *BB[α]-trees*

The *Weight* of a tree is the number of its leaves, so in a weight-balanced tree, the weight of the left and right subtrees in each node should be *balanced*.

The Balanced is defined as the difference by 1 at most, but we can't keep this balance condition with O(log n) rebalancing work during insertions and deletions.

**Instead** of bounding **Difference**, the correct choice is to bound **Ratio** of the weights. This gives an entire family of balance conditions:

The **α-weight balanced trees**, where for each subtree the left and right sub-subtrees have each **at least a fraction of α of the total weight of the subtree** and **at most a fraction of (1 - α)**. An α-weight balanced tree has necessarily small height.

α < 1 − 1/√2 is necessarly condition *Nievergelt and Reingold (1973)*

also can't be choosen too small

2/11 < α as lower bound *Blum and Mehlhorn 1980*

In our model we restrict ourselves with

**α ∈ [ 2/7 , 1 − 1/√2 ] ⊃ [0.286, 0.292],**

second paramenter **ε** subject to **ε ≤ α^2 − 2α + 1/2**

```
typedef struct wb_node {
	wb_node_t    *edge[2];
	uint32_t      weight;
} wb_node_t;
```

The weight of a node *n is defined recursively as follows:
1. If *n is a leaf (!n->edge[1]), then n->weight == 1.
2. Else n->weight is the sum of the weight of the left and right subtrees.

	`n->edge[0]->weight + n->edge[1]->weight`

The node n is α-weight balanced if

`n->edge[0]->weight >= α n->weight` and `n->edge[1]->weight >= α n->weight`

or equivalently

`α n->edge[0]->weight <= (1 - α) n->edge[1]->weight` and `(1 - α) n->edge[0]->weight >= α n->edge[1]->weight`

The weight information must be corrected whenever the tree is changed. Weight changes only by delete and insert operations, and only in the nodes on the path from the changed leaf to the root, and only by at most 1. So we follow path up to the root and restore balance in each node, and know by induction that subtrees below that node are balanced.

If *n is the current node, and we corrected the weight of *n, there are the following cases of rebalancing:

1. n->edge[0]->weight >= α n->weight and

	n->edge[1]->weight >= α n->weight

	In this case, no rebalancing is necessary in this node;

	we go up the tree to the next node.

2. n->edge[1]->weight < α n->weight

	1. If n->edge[0]->edge[0]->weight > (α + ε) n->weight

		Perform right rotation around n, followed by recomputing the weight in n->edge[1]

	2. If n->edge[0]->edge[1]->weight > (α + ε) n->weight

		Perform left rotation around n->edge[0], followed by right rotation around n, followed by recomputing the weight in n->edge[0] and n->edge[1]

3. n->edge[0]->weight < α n->weight

	1. If n->edge[1]->edge[1]->weight > (α + ε) n->weight

		Perform left rotation around n, followed by recomputing the weight in n->edge[0]

	2. If n->edge[1]->edge[0]->weight > (a + ε) n->weight

		Perform right rotation around n->edge[1], followed by a left rotation around n,followed by recomputing the weight in n->edge[0] and n->edge[1]

Unlike height balanced trees when restoring balance we can't stop early and have to follow whole path to the root which is log n, rotations and height recomputation is constant so rebalance takes O(log n).

## Operations
   | Time Complexity | Return Type    | Operation name         | Arguments 		       	     |
   |:----------------|:---------------|:-----------------------|:------------------------------------|
   | O(1)	     | wbt_t *        | wbt\_constructor       | ()				     |
   |   	 	     | void           | wbt\_destructor        | (wbt\_t *wbt)		             |
   | O(log n)	     | void	      | wbt\_insert            | (wbt\_t *wbt, void *key, void *val) |
   | O(log n)        | void * 	      | wbt\_find              | (wbt\_t *wbt, void *key)	     |
   | O(log n)        | void * 	      | wbt\_delete            | (wbt\_t *wbt, void *key)	     |