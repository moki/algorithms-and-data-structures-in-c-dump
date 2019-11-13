#include "patricia.h"
#include <stdio.h>

int main(int argc, char **argv) {
	printf("hello patricia\n");
	patricia_t patricia = patricia_constructor(NULL, NULL, NULL, stringkv_len);

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

	patricia_insert(patricia, key_1, value_1);
	patricia_insert(patricia, key_2, value_2);
	patricia_insert(patricia, key_3, value_3);
	patricia_insert(patricia, key_4, value_4);
	patricia_insert(patricia, key_5, value_5);
	patricia_insert(patricia, key_6, value_6);

	// patricia_delete(patricia, key_1);
	// patricia_delete(patricia, key_2);
	// patricia_delete(patricia, key_3);
	// patricia_delete(patricia, key_4);
	// patricia_delete(patricia, key_5);
	// patricia_delete(patricia, non_existing_key);

	// printf("%d\n", patricia_lookup(patricia, key_1) == value_1);
	// printf("%d\n", patricia_lookup(patricia, key_2) == value_2);
	// printf("%d\n", patricia_lookup(patricia, key_3) == value_3);
	// printf("%d\n", patricia_lookup(patricia, key_4) == value_4);
	// printf("%d\n", patricia_lookup(patricia, key_5) == value_5);
	// printf("%d\n", patricia_lookup(patricia, key_6) == value_6);

	// patricia_destructor(patricia);
}