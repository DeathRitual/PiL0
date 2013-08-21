#include<stdlib.h>
#include<stdio.h>
#include<string.h>

struct hash_table {
	const size_t HASH_SIZE;
	size_t used;
	void **table;
};

typedef struct hash_table *HASHTABLE;

HASHTABLE init_hash(void *data_type, size_t size, void *(*cast)(void *)) {
	HASHTABLE new_hash = NULL;
	size_t i;

	if ((new_hash = malloc(sizeof(*new_hash))) == NULL)
		printf("fehler\n!");
	if ((new_hash->table = malloc(sizeof((cast)(data_type)) * size)) == NULL)
		printf("fehler\n!");

	*(size_t *)&new_hash->HASH_SIZE = size;
	new_hash->used = 0;

	for (i = 0; i < new_hash->HASH_SIZE; ++i)
		new_hash->table[i] = NULL;

	return new_hash;
}

int full_hash(HASHTABLE hash) {
	return (hash->used == hash->HASH_SIZE);
}

static unsigned genHashKey(HASHTABLE hash, char *s) {
	unsigned hashval1, hashval2, hashkey;
	unsigned int i;
	char *name = s;

	for (hashval1 = hashval2 = 0; *name != '\0'; name++) {
		hashval1 = ((hashval1 << 7) + *name) % hash->HASH_SIZE;
		hashval2 = ((hashval2 << 7) + *name) % (hash->HASH_SIZE - 2);
	}

	hashkey = hashval1;
	for (i = 0; (hash->table[hashkey] == NULL || strcmp(hash->table[hashkey], s)) && i < hash->HASH_SIZE; i++)
		hashkey = (hashval1 + i * hashval2) % hash->HASH_SIZE;

	hash->used++;
	return hashkey;
}

void insertHash(HASHTABLE hash, char *s, void *element) {

	unsigned key = genHashKey(hash, s);

	if (hash == NULL)
		printf("error");

	if (!full_hash(hash)) {
		printf("insert: %d\n", key); 
		hash->table[key] = element;
	} else
		printf("error");;
}

void *getHash(HASHTABLE hash, char *s) {
	void *element;

	unsigned key = genHashKey(hash, s);

	if (hash == NULL)
		printf("error");
	printf("get: %d\n", key);
	return ((element = hash->table[key]) == NULL) ? NULL : element;
}

struct test *casting(void *element) {
	return (struct test *) element;
}

struct test {
	char name[30];
	int x;
	int y;
	} point_one, point_sec, point_thi;

int main() {

	struct test *dummy;
	size_t size = 13;
	HASHTABLE init = init_hash(dummy, size, (void *)casting);


	strcpy(point_one.name, "entry\0");
	strcpy(point_sec.name, "hello\0");
	strcpy(point_thi.name, "12345\0");

	point_one.x = 5;
	point_one.y = 20;
	
	point_sec.x = 17;
	point_sec.y = 30;

	point_thi.x = 11;
	point_thi.y = 2350;

	insertHash(init, point_one.name, &point_one);				
	insertHash(init, point_sec.name, &point_sec);				
	insertHash(init, point_thi.name, &point_thi);				

	dummy = getHash(init, point_sec.name);
	printf("Output Name: %s, x: %d, y: %d\n", dummy->name, dummy->x, dummy->y);


	return 1;
}
