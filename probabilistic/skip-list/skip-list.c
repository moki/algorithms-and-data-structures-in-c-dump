/**
 * Generic Skip List
 *
 * Jun 2018 Kirill Morozov <morozov.kirill.moki@gmail.com>
 *
 * Consider implementing oprimizations:
 * http://ticki.github.io/blog/skip-lists-done-right/
 * linked memory pools, unrolled lists
 */

#include <time.h>
#include <limits.h>
#include "skip-list.h"

static 			int seed_reset = 0;
static int		_random(void);
static int		_sl_random_level(void);
static skip_list_node	_sl_create_new_node(int, void *);
static void		_sl_traverse(skip_list, void (*)(void *));

struct _skip_list {
	skip_list_node 	header;
	unsigned int 	length;
	int 		level;
	int 		(*cmp)(const void *, const void *);
};

struct _sln_level {
	skip_list_node forward;
	unsigned int width;
};

struct _skip_list_node {
	void 		*key;
	sln_level	levels[];
};

static int _random(void) {
	if (!seed_reset) {
		srand(time(NULL));
		seed_reset = 1;
	}

	return (int)(rand() % RAND_MAX);
}

static int _sl_random_level(void) {
	int level = 1;

	while ((_random()&0xFFFF) < (SL_P * 0xFFFF))
		level += 1;

	return (level < SL_MAXLEVEL) ? level : SL_MAXLEVEL;
}

static skip_list_node _sl_create_new_node(int level, void *key) {
	skip_list_node sln = malloc(sizeof(*sln) + level * sizeof(sln_level));

	sln->key = key;

	return sln;
}

skip_list sl_constructor(int (*cmp)(const void *, const void *)) {
	skip_list sl = malloc(sizeof(*sl));

	sl->level  = 1;
	sl->length = 0;
	sl->cmp = cmp;
	sl->header = _sl_create_new_node(SL_MAXLEVEL, NULL);

	int i;
	for (i = 0; i < SL_MAXLEVEL; i++) {
		sl->header->levels[i].forward = NULL;
		sl->header->levels[i].width = 0;
	}

	return sl;
}

void sl_destructor(skip_list sl) {
	_sl_traverse(sl, free);

	free(sl);
}

void sl_traverse_apply_key(skip_list sl, void(*fn)(void *)) {
	skip_list_node cursor = sl->header->levels[0].forward;

	while (cursor) {
		fn(cursor->key);
		cursor = cursor->levels[0].forward;
	}
}

static void _sl_traverse(skip_list sl, void(*fn)(void *)) {
	skip_list_node cursor = sl->header;

	while (cursor) {
		skip_list_node x = cursor->levels[0].forward;
		fn(cursor);
		cursor = x;
	}
}

void *sl_find_k_smallest(skip_list sl, unsigned int k) {
	if (k <= 0 || k >= sl->length) return NULL;

	skip_list_node x = sl->header;
	unsigned int j = 0;
	int i;

	for (i = sl->level - 1; i >= 0; i--) {
		while (x->levels[i].forward && (j + x->levels[i].width) <= k) {
			j += x->levels[i].width;
			x = x->levels[i].forward;
		}

		if (j == k) return x->key;
	}

	return NULL;
}

void sl_insert(skip_list sl, void *key) {
	skip_list_node 	update[SL_MAXLEVEL];
	unsigned int 	rank[SL_MAXLEVEL];
	skip_list_node 	x = sl->header;

	/* finding previous nodes on each level */
	int i;
	for (i = sl->level - 1; i >= 0; i--) {
		rank[i] = i == (sl->level - 1) ? 0 : rank[i + 1];

		while (x->levels[i].forward &&
			sl->cmp(x->levels[i].forward->key, key) < 0)
		{
			rank[i] += x->levels[i].width;
			x = x->levels[i].forward;
		}

		update[i] = x;
	}

	/* potential successor */
	x = x->levels[0].forward;

	/* Key already present in the list, return prematurely. */
	if (x && sl->cmp(x->key, key) == 0)
		return;

	int level = _sl_random_level();

	/**
	 * Set dummy head pointer as a predecessor
	 * from old level up to the new one
	 */
	if (level > sl->level) {
		for (i = sl->level; i < level; i++) {
			rank[i] = 0;
			update[i] = sl->header;
			update[i]->levels[i].width = sl->length;

		}

		sl->level = level;
	}

	x = _sl_create_new_node(level, key);

	/**
	 * Set next pointers on each level for new node as predecessor->next.
	 * Set next pointers on each level for precedecessors as new node.
	 */
	for (i = 0; i < level; i++) {
		x->levels[i].forward = update[i]->levels[i].forward;
		update[i]->levels[i].forward = x;

		x->levels[i].width = update[i]->levels[i].width - (rank[0] - rank[i]);
		update[i]->levels[i].width = (rank[0] - rank[i]) + 1;
	}

	for (i = level; i < sl->level; i++)
		update[i]->levels[i].width++;

	sl->length++;
}

void sl_delete(skip_list sl, void *key) {
	skip_list_node update[SL_MAXLEVEL];
	skip_list_node x = sl->header;

	/* finding previous nodes on each level */
	int i;
	for (i = sl->level - 1; i >= 0; i--) {
		while (x->levels[i].forward && sl->cmp(x->levels[i].forward->key, key) < 0)
			x = x->levels[i].forward;

		update[i] = x;
	}

	/* successor */
	x = x->levels[0].forward;

	/* node with such key doesnt exists */
	if (x && sl->cmp(x->key, key) != 0)
		return;

	for (i = 0; i < sl->level; i++) {
		if (update[i]->levels[i].forward == x) {
			update[i]->levels[i].width += x->levels[i].width - 1;
			update[i]->levels[i].forward = x->levels[i].forward;
		} else {
			update[i]->levels[i].width -= 1;
		}
	}

	free(x);

	while (sl->level > 1 && !sl->header->levels[sl->level - 1].forward)
		sl->level--;

	sl->length--;
}