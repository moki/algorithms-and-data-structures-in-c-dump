#include "trie.h"

int main(int argc, char **argv) {
	printf("hello\n");

	trie_node_t *trie = trie_constructor();

	char *k_a = "hello";
	char *v_a = "world";

	trie_insert(trie, k_a, v_a);

	char *k_b = "bye";
	char *v_b = "cruel";

	trie_insert(trie, k_b, v_b);

	char *k_c = "ehh";
	char *v_c = "wehh";

	trie_insert(trie, k_c, v_c);

	char *r;

	r = trie_find(trie, k_a);
	printf("%s\n", r ? r : "");

	r = trie_find(trie, k_b);
	printf("%s\n", r ? r : "");

	r = trie_find(trie, k_c);
	printf("%s\n", r ? r : "");

	trie_delete(trie, k_a);
	trie_delete(trie, k_b);

	r = trie_find(trie, k_c);
	printf("%s\n", r ? r : "");

	r = trie_find(trie, k_b);
	printf("%s\n", r ? r : "");

	trie_destructor(trie);
}