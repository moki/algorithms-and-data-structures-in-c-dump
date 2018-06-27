#include <stdio.h>
#include "digital-search-tree.h"

int main(int argc, char **argv) {
	// printf("digital search tree\n");

	dst_t tree = dst_constructor(NULL, NULL, NULL, stringkv_len);
	// dst_t tree = dst_constructor(NULL, stringkv_destroy, stringkv_destroy, stringkv_len);

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

	char non_existing_key[2] = "\\";

	// char *key = malloc(sizeof(char) * 2);
	// char *value = malloc(sizeof(char) * 2);

	// strcpy(key, "h");
	// strcpy(value, "y");

	// char *_key = malloc(sizeof(char) * 2);
	// char *_value = malloc(sizeof(char) * 2);

	// strcpy(_key, "y");
	// strcpy(_value, "e");

	dst_insert(tree, key_1, value_1);
	dst_insert(tree, key_2, value_2);
	dst_insert(tree, key_3, value_3);
	dst_insert(tree, key_4, value_4);
	dst_insert(tree, key_5, value_5);
	dst_insert(tree, key_6, value_6);

	dst_delete(tree, key_1);
	dst_delete(tree, key_2);
	dst_delete(tree, key_3);
	dst_delete(tree, key_4);
	dst_delete(tree, key_5);
	dst_delete(tree, non_existing_key);

	printf("%d\n", dst_lookup(tree, key_1) == value_1);
	printf("%d\n", dst_lookup(tree, key_2) == value_2);
	printf("%d\n", dst_lookup(tree, key_3) == value_3);
	printf("%d\n", dst_lookup(tree, key_4) == value_4);
	printf("%d\n", dst_lookup(tree, key_5) == value_5);
	printf("%d\n", dst_lookup(tree, key_6) == value_6);

	dst_destructor(tree);
}