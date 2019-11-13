#include "trie.h"
#include <stdio.h>

int main(int argc, char **argv) {
	printf("hello trie\n");
	trie_t trie = trie_constructor(NULL, NULL, NULL, stringkv_len);

	char key_1[2] = "a";
	char value_1[3] = "av";

	char key_2[2] = "b";
	char value_2[3] = "bv";

	char key_3[2] = "c";
	char value_3[3] = "cv";

	char key_4[2] = "d";
	char value_4[3] = "dv";

	char key_5[2] = "e";
	char value_5[3] = "ev";

	char key_6[2] = "f";
	char value_6[3] = "fv";

	// char non_existing_key[2] = "\\";

	// char *key = malloc(sizeof(char) * 2);
	// char *value = malloc(sizeof(char) * 2);

	// strcpy(key, "h");
	// strcpy(value, "y");

	// char *_key = malloc(sizeof(char) * 2);
	// char *_value = malloc(sizeof(char) * 2);

	// strcpy(_key, "y");
	// strcpy(_value, "e");

	trie_insert(trie, key_1, value_1);
	trie_insert(trie, key_2, value_2);
	trie_insert(trie, key_3, value_3);
	trie_insert(trie, key_4, value_4);
	trie_insert(trie, key_5, value_5);
	trie_insert(trie, key_6, value_6);

	// trie_delete(trie, key_1);
	// trie_delete(trie, key_2);
	// trie_delete(trie, key_3);
	// trie_delete(trie, key_4);
	// trie_delete(trie, key_5);
	// trie_delete(trie, non_existing_key);

	// printf("%d\n", trie_lookup(trie, key_1) == value_1);
	// printf("%d\n", trie_lookup(trie, key_2) == value_2);
	// printf("%d\n", trie_lookup(trie, key_3) == value_3);
	// printf("%d\n", trie_lookup(trie, key_4) == value_4);
	// printf("%d\n", trie_lookup(trie, key_5) == value_5);
	// printf("%d\n", trie_lookup(trie, key_6) == value_6);

	trie_destructor(trie);
}