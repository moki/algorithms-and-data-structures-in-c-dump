# 8.3 Reduced Alphabet Trie

## Description:
Another way to avoid the problem with alphabet size |A| is alphabet reduction.

We can represent the alphabet A as set of k-tuples from some direct product A_1 x ... x A_k;

by this each string gets longer by a factor of k, but the alphabet size can be reduced to round up of the |A|^1/k.

For standard ASCII codes, we can break each 8-bit character by two 4-bit characters, which reduces the node size from 256 pointers to 16 pointers, but doubles the length oof each search path.

At the extreme end, we could use a 1-bit alphabet, representing the strings as
sequences of single bits; this has been variously discussed in literature because
it seems a natural model, but the many necessary bit operations make it a bad
choice in real implementations.

We implement the trie structure, for strings of 8-bit characters broken in two 4-bit characters, with
no special role of the ’\0’-character but instead a given length of each string.
We also use reference counts in the nodes to speed up the deletion.

### Node structure:
```
typedef struct trie_node_t {
	struct trie_node_t *next[16];
	void *val;
	int refs;
} trie_node_t;
```

```
The trie structure with k-fold alphabet reduction stores a set of words over an alphabet A.
It supports find and delete operations on a query string q in time O(k length(q)) and
insert operations in time O(k|A|^1/k length(q)).
The space requirement to store n strings w_1,...,w_n is O(k|A|^1/k sum[i, n](length(w_i))).
```

Main improvement is the space complexity.