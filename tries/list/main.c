#include "trie.h"

int main(int argc, char **argv) {
	printf("hello\n");

	trie_node_t *trie = trie_constructor();

	char *k_a = "hello";
	char *v_a = "world";

	char *k_b = "foo";
	char *v_b = "bar";

	char *k_c = "world is";
	char *v_c = "cruel";

	trie_insert(trie, k_a, v_a);
	trie_insert(trie, k_b, v_b);
	trie_insert(trie, k_c, v_c);

	char *r;
	r = trie_find(trie, k_a);
	printf("%s:%s\n", k_a, r);

	r = trie_find(trie, k_b);
	printf("%s:%s\n", k_b, r);

	r = trie_find(trie, k_c);
	printf("%s:%s\n", k_c, r);


	r = trie_find(trie, k_a);
	printf("%s:%s\n", k_a, r ? r : "");

	r = trie_find(trie, k_b);
	printf("%s:%s\n", k_b, r ? r : "");

	trie_destructor(trie);
}