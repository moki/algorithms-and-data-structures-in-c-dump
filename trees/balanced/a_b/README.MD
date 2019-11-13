# 3.3 (a, b)- and B-Trees

## Description:
A different method to keep the height of the tree small is to allow tree nodes of higher degree. This idea was introduced as B-Trees by Bayer and McCreight 1972 and turned out to be very fruitful, first as external memory data structure, but also has interesting uses as normal memory data structure.

The characteristic of external memory is that access to it is very slow, compared to main memory, and is done in block, units much larger than single main memory locations, which are simultaniously transferred into main memory.

```
struct abt_node {
	int 		 degree;
	int 		 height;
	void   		*key[b];
	struct abt_node *next[b];
}
```