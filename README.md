
# 📚 linked_list

A linked list shared library writen in **C** and made in **TDD**
Most operations have a complexity of **O(1)**


## 📦 Building it and checking

```
make lib
make run-tests
```


## 🤔 How to use

Include `include/List.h` in your project and link the library
```
gcc [your sources].c -Llib/ -llist.so -o [your program]
LD_LIBRARY_PATH=lib/ ./[your program]
```


## Usage example, with OpenSSL to store random strings

```C
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/engine.h>

#include "include/List.h"

char * create_random_binary_string(size_t length)
{
	char * random_string = malloc(length + 1);
	if (random_string == NULL)
		return NULL;

	RAND_bytes(random_string, length);
	random_string[length] = '\0';

	/* remove non-terminating null bytes */
	for (int index = 0; index < length; index++)
	{
		if (random_string[index] == '\0')
			random_string[index] = 0xC0;
	}

	return random_string;
}

/* credits: https://gist.github.com/barrysteyn/4409525 */
int base64_encode(const char * message, char ** buffer)
{
	BIO *bio, *b64;
	FILE * stream;
	int encodedSize = 4 * ceil((double) strlen(message) / 3);
	* buffer = (char *) malloc(encodedSize + 1);

	stream = fmemopen(* buffer, encodedSize + 1, "w");
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new_fp(stream, BIO_NOCLOSE);
	bio = BIO_push(b64, bio);
	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
	BIO_write(bio, message, strlen(message));
	BIO_flush(bio);
	BIO_free_all(bio);
	fclose(stream);

	return EXIT_SUCCESS;
}

char * create_random_base64_string(size_t length)
{
	char * base64;
	char * binary_string = create_random_binary_string(16);

	if (binary_string == NULL)
		return NULL;

	base64_encode(binary_string, & base64);
	free(binary_string);

	return base64;
}

void delete_random_strings_list(linked_list ** list)
{
	char * string;

	if (list == NULL)
		return;

	while (* list != NULL)
	{
		string = list_content(* list);
		free(string); /* we're in charge of the memory */
		list_remove_node(list);
	}
}

linked_list * create_random_strings_list(size_t size)
{
	char * random_base64;

	linked_list * strings = list_create();

	while (size--)
	{
		random_base64 = create_random_base64_string(16);
		if (random_base64 == NULL)
		{
			fprintf(stderr, "Memory allocation failed! :(\n");
			delete_random_strings_list(& strings);
			return NULL;
		}
		list_append(& strings, random_base64);
	}

	return strings;
}



void print_random_strings_list(linked_list const * list)
{
	int index = 0;

	while (list != NULL)
	{
		printf("[%d] -> %s\n", index++, (char *) list_content(list));
		list = list_next(list);
	}
}

void init_openssl(void)
{
	unsigned char seed[16];

	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();

	if (RAND_status() != 1)
		RAND_seed(seed, sizeof(seed));
}

int main(void)
{
	linked_list * random_strings;

	init_openssl();

	random_strings = create_random_strings_list(10);
	if (random_strings == NULL)
		exit(EXIT_FAILURE);

	print_random_strings_list(random_strings);
	delete_random_strings_list(& random_strings);

	return EXIT_SUCCESS;
}
```
```bash
gcc example.c -o example -lm -lcrypto -Llib/ -llist && LD_LIBRARY_PATH=lib/ ./example
```


## 🔮 Functions to come

- list_prepend
- list_find_first
- list_find_last
- list_sort
- list_map
- list_filter


## 😨 Found a bug ?

Create a pull request with a failing test, I'll make it pass
