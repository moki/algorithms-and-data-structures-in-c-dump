#include "trie.h"

int main(int argc, char **argv) {
	printf("hello\n");

	trie_node_t *trie = trie_constructor();

	char *k_a = "hello, ";
	char *v_a = "world!";

	trie_insert(trie, k_a, v_a, (key_length_fn_t) strlen);

	char *k_b = "not found";

	char *k_c = "foo";
	char *v_c = "bar";

	trie_insert(trie, k_c, v_c, (key_length_fn_t) strlen);

	char *k_d = "hey there";
	char *v_d = "hello!";
	trie_insert(trie, k_d, v_d, (key_length_fn_t) strlen);

	char *k_e = "Really long key goes here";
	char *v_e = "And here goes really long value!";
	trie_insert(trie, k_e, v_e, (key_length_fn_t) strlen);

	trie_delete(trie, k_d, (key_length_fn_t) strlen);

	char *r;

	r = trie_find(trie, k_a, (key_length_fn_t) strlen);
	printf("%s:%s\n", k_a, r);

	r = trie_find(trie, k_b, (key_length_fn_t) strlen);
	printf("%s:%s\n", k_b, r);

	r = trie_find(trie, k_d, (key_length_fn_t) strlen);
	printf("%s:%s\n", k_d, r);

	r = trie_find(trie, k_c, (key_length_fn_t) strlen);
	printf("%s:%s\n", k_c, r);

	r = trie_find(trie, k_e, (key_length_fn_t) strlen);
	printf("%s:%s\n", k_e, r);

	trie_destructor(trie);
}