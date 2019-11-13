# 8.1 Chaining Hash Table

## Description:
We partition the universe U by h^−1(S) and store those x ∈
X ⊂ U that are in the same partition class in the same secondary structure. If we can insert and delete in the secondary structure, we can do so in the main structure. The function h just directs us to the right secondary structure. If partition induced on x is fine, with only at most a few elements in each bucket, this is especially good, but if there are many elements in the same, it degrades no worse that the secondary structure we are using.

We could use a balanced search tree as secondary structure and get a worst-time O(log n) bound in addition to an O(1) time for all those elements whose bucket contains few elements. But we will show that with proper choice of the hash function and a not-too-small set S, most buckets are expected to be almost empty. So the choice of a linked list as a secondary structure is enough.

## Data Strucutre

```
typedef struct hash_table_t {
	hash_bucket_t **table;
	key_len_fn_t	key_len_fn;
	hash_fn_t	hash_fn;
	uint32_t 	size;
} hash_table_t;
```

```
typedef struct bucket_node_t {
	bucket_node_t *next;
	void          *key;
	void 	      *val;
} bucket_node_t;
```

```
typedef uint32_t (*hash_fn_t)(void *key, size_t key_len);
typedef size_t 	 (*key_len_fn_t)(void *key);
```
