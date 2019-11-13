# 8.2 List Trie

## Description:
Replacing the big nodes by linked lists of all the entries that are really used.
Is one of the ways to solve problem of the alphabet size.
Empty string '\0' already presented in the created trie when we create it, we are using it as an entry point to the structure.

### Node structure:
```
typedef struct trie_node_t {
	struct trie_node_t *next;
	struct trie_node_t *list;
	char ch;
} trie_node_t;
```

```
The trie structure with nodes realized as lists stores a set of words over an alphabet A.
It supports a find operation on a query string q in time O(|A| length(q))
Insert and Delete operations in time O(|A| length(q)).
The space requirement to store n strings w_1,..., w_n is O(sum[i, n](length(w_i))
```

Main improvement is the space complexity.