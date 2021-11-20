#ifndef HASHMAP_H
#define HASHMAP_H

struct Hashmap;

typedef struct Hashmap Hashmap;

unsigned long strHash(const char* str);

Hashmap* newHashmap(unsigned int expected, unsigned long (*hash)(const void* key), int (*cmp)(const void* a, const void* b));

int mapAdd(Hashmap* map, void* key, void* value);

int mapContains(Hashmap* map, void* key);

void* mapGet(Hashmap* map, void* key);

void mapForEach(Hashmap* map, void (*fn)(void* key, void* value));

unsigned int mapSize(Hashmap* map);

void deleteMap(Hashmap* map);

#endif
