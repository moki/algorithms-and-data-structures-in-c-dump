#include "patricia.h"

static patricia_node_t patricia_create_node(patricia_t patricia, const void *key, const void *value);
static patricia_node_t patricia_lookup_node(patricia_node_t cursor, const void *key, size_t key_len);
static void 	       patricia_clear_node(patricia_t patricia, patricia_node_t node);

static int keydiffbit(const unsigned char *key1, size_t key1len, const unsigned char *key2, size_t key2len);
static int iszero(const void *key, size_t key_len);

struct patricia {
	patricia_destroy_value_fn_t *destroy_value_fn;
	patricia_destroy_key_fn_t   *destroy_key_fn;
	patricia_length_key_fn_t    *length_key_fn;
	patricia_node_t 	     root;
	uint32_t 		     size;
};

struct patricia_node {
	patricia_node_t edge[2];
	item_t		item;
	int		bit;
};

patricia_t patricia_constructor(
	const void *default_value,
	patricia_destroy_key_fn_t destroy_key_fn,
	patricia_destroy_value_fn_t destroy_value_fn,
	patricia_length_key_fn_t length_key_fn
) {
	patricia_t patricia = malloc(sizeof(*patricia));
	if (!patricia) return NULL;

	patricia->root = patricia_create_node(patricia, NULL, NULL);
	if (!patricia->root) return NULL;

	patricia->root->edge[0] = patricia->root;
	patricia->root->bit = -1;

	patricia->destroy_value_fn = destroy_value_fn;
	patricia->destroy_key_fn   = destroy_key_fn;
	patricia->length_key_fn	   = length_key_fn;
	patricia->size		   = 0;

	return patricia;
}

void patricia_destructor(patricia_t patricia);

void patricia_insert(patricia_t patricia, const void *key, const void *value) {
	size_t 		     key_len = patricia->length_key_fn(key);
	const unsigned char *u_key_end;
	const unsigned char *u_key;
	int 		     bit;

	patricia_node_t      q;
	const unsigned char *q_u_key;
	const unsigned char *q_u_key_end;

	patricia_node_t node;
	patricia_node_t cursor;
	int		nbit;
	int		parbit;
	patricia_node_t trailing_cursor;
	int		dir;
	int		_dir;

	q = patricia->root;
	if (!q) return;

	if (iszero(key, patricia->length_key_fn(key))) {
		if (q->item.key == key) {
			q->item.value = value;
		} else {
			patricia_clear_node(patricia, q);

			q->item.key_len = patricia->length_key_fn(key);
			q->item.value	= value;
			q->item.key	= key;
		}

		return;
	}

	q = patricia_lookup_node(q, key, key_len);
	if (!q) return;

	if (q->item.key_len == key_len && memcmp(q->item.key, key, key_len) == 0) {
		if (q->item.key == key) {
			q->item.value = value;
		} else {
			patricia_clear_node(patricia, q);

			q->item.key_len = patricia->length_key_fn(key);
			q->item.value	= value;
			q->item.key	= key;
		}

		return;
	}

	q_u_key     = q->item.key;
	q_u_key_end = q_u_key + q->item.key_len;

	u_key	    = key;
	u_key_end   = u_key + key_len;

	bit	    = keydiffbit(q_u_key, q_u_key_end - q_u_key, u_key, u_key_end - u_key);

	node	    = patricia_create_node(patricia, key, value);
	if (!node) return;

	trailing_cursor = patricia->root;
	nbit		= trailing_cursor->bit;

	do {
		parbit 		= nbit;
		_dir		= GET_DIR(u_key, u_key_end, nbit);
		cursor 		= trailing_cursor->edge[_dir];
		trailing_cursor = cursor;
		nbit		= trailing_cursor->bit;
	} while (nbit < bit && nbit > parbit);

	node->bit = bit;

	dir = GET_DIR(u_key, u_key_end, bit);
	assert(dir == 0 || dir == 1);

	node->edge[dir]  = node;
	node->edge[!dir] = trailing_cursor;

	trailing_cursor->edge[_dir] = node;
}

void patricia_delete();

void *patricia_lookup();

static patricia_node_t patricia_lookup_node(patricia_node_t cursor, const void *key, size_t key_len) {
	const unsigned char *u_key 	= key;
	const unsigned char *u_key_end	= u_key + key_len;
	int 		     i;

	do {
		i = cursor->bit;
		cursor = cursor->edge[GET_DIR(u_key, u_key_end, i)];
		if (!cursor) return NULL;
	} while (cursor->bit > i);

	return cursor;
}

static patricia_node_t patricia_create_node(patricia_t patricia, const void *key, const void *value) {
	patricia_node_t node = malloc(sizeof(*node));
	if (!node) return node;

	node->edge[0] = NULL;
	node->edge[1] = NULL;

	node->item.key_len = patricia->length_key_fn(key);
	node->item.value   = value;
	node->item.key 	   = key;

	patricia->size++;

	return node;
}

static void patricia_clear_node(patricia_t patricia, patricia_node_t node) {
  if (patricia->destroy_key_fn && node->item.key)
    patricia->destroy_key_fn((void *) node->item.key);
  if (patricia->destroy_value_fn && node->item.value)
    patricia->destroy_value_fn((void *) node->item.value);
}

static int iszero(const void *key, size_t key_len) {
  const unsigned char *u_key = key;
  const unsigned char *u_key_end   = u_key + key_len;

  while (u_key < u_key_end)
    if (*u_key++)
      return 0;

  return 1;
}

static int keydiffbit(const unsigned char *key1, size_t key1len,
		      const unsigned char *key2, size_t key2len) {
	const unsigned char *longest;
	const unsigned char *shortest;
	size_t               commonlen;
	const unsigned char *longest_base;
	int                  mask;
	int                  offset;

	/* sort out which keys are longest and shortest */
	if (key1len >= key2len) {
		longest   = key1;
		shortest  = key2;
		commonlen = key2len;
	} else {
		longest   = key2;
		shortest  = key1;
		commonlen = key1len;
	}

	longest_base = longest;

	/* find differing bytes in the common span of the keys */
	for (; commonlen; commonlen--) {
		if ((mask = *longest++ ^ *shortest++) != 0)
			break;
	}

	/* we've now found a difference, or run out of common bytes
	 * if we ran out of bytes to compare look at longest's suffix
	 */
	if (commonlen == 0) {
		const unsigned char *longest_end;

		if (key1len == key2len) 		/* (might not happen in practice) */
			return -1; 			/* keys are the same length - there can be no difference */

		/* we could arrive here if the shortest key was zero bytes long and the
		 * longest key was positive bytes long. 'mask' may be unset.
		 */
		if (key1len >= key2len)
			longest_end = key1 + key1len;
		else
			longest_end = key2 + key2len;

		assert(longest != longest_end);

		/* look for a non-zero byte
		 * note that this is a do-while loop to appease clang's analysis. it says
		 * that mask might be getting used uninitialised and draws many pretty
		 * arrows, but i don't follow its reasoning.
		 */
		do {
			if ((mask = *longest++) != 0)
				break;
		} while (longest < longest_end);

		if (longest == longest_end && mask == 0)
			return -1;			 /* we ran out of bytes */
	}

	offset = (longest - 1 - longest_base) * 8;

	/* 128, 64, 32, ... => 0, 1, 2, ... */
	if (mask & 128) return offset + 0;
	if (mask &  64) return offset + 1;
	if (mask &  32) return offset + 2;
	if (mask &  16) return offset + 3;
	if (mask &   8) return offset + 4;
	if (mask &   4) return offset + 5;
	if (mask &   2) return offset + 6;

	return offset + 7;
}