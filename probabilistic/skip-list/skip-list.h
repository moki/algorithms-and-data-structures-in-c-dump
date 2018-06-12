/**
 * Generic Skip List
 *
 * Jun 2018 Kirill Morozov <morozov.kirill.moki@gmail.com>
 */

#include <stdlib.h>

#define SL_MAXLEVEL 32
#define SL_P 0.25

typedef struct _skip_list      *skip_list;
typedef struct _skip_list_node *skip_list_node;
typedef struct _sln_level     	sln_level;

skip_list sl_constructor(int (*)(const void *, const void *));

void sl_destructor(skip_list sl);

void sl_insert(skip_list, void *);

void sl_delete(skip_list, void *);

void sl_traverse_apply_key(skip_list sl, void (*)(void *));

void *sl_find_k_smallest(skip_list, unsigned int);