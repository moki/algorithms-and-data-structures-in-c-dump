# 8.1 Array Trie

## Description:
edge[(int)’\0’] hold pointer to the object stored in the trie, for ’\0’ terminated strings we will never need that field to point to another node. If we need a string model without a specific termination character, we must use an extra field in the node for that.

### Node structure:

```
typedef struct trie_node_t {
	struct trie_ndoe_t *edge[256];
} trie_node_t;
```

Structure looks simple and efficient but the problem is dependence on the size of the alphabet that determines the size of the nodes.

Each node *256 pointers*, poointer *4-8 bytes* => **size** of each **node** atleast **1kb**.

So unless string to be stored often overlap, we need approx. as many nodes as the total length of all strings together => space req. is enormous.
```
Basic trie structure stores a set of words over an alphabet A.
It supports a find operation on a query string q in time O(length(q)).
Insert and Delete operations in time O(|A| length(q)).
Requirement to store n strings w1, ..., wn is O(|A| sum[i, n](length(w i ))).
```
|A|-dependence eleminated in the delete operation by using reference counts.
There are several ways to reduce or avoid the problem of the alphabet size,
and the mostly empty nodes.

In each method, we trade some loss in the query time against an improvement in space and update time.

refer to [list optimization](../list/README.MD) as one of the way to improve.