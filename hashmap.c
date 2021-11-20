#include "hashmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct entry
{
  char* key;
  void* value;
  struct entry* next;
} Node;

struct Hashmap
{
  Node** entries;
  unsigned int count;
  unsigned int filled;
  unsigned int size;
  unsigned long (*hash)(const void* key);
  int (*cmp)(const void* a, const void* b);
};

int contains_help(Hashmap* map, unsigned long h, char* key)
{
  Node* n = map->entries[h];
  while (n)
    {
      if (!map->cmp(key, n->key))
	{
	  return 1;
	}
      n = n->next;
    }
  return 0;
}

void put_help(Hashmap* map, unsigned long h, Node* node)
{
  node->next = map->entries[h];
  map->entries[h] = node;
  map->count++;
}

unsigned int isPrime(unsigned int num)
{
  unsigned int i;
  for (i = 2; i <= sqrt(num); ++i)
    {
      if (num % i)
	{
	  return 0;
	}
    }
  return 1;
}

unsigned int nextPrime(unsigned int num)
{
  unsigned int c;
  if ((num & 1))
    {
      --num;
    }
  if (num == 2)
    {
      c = 3;
    }
  else
    {
      c = isPrime(num += 2) ? nextPrime(num) : num;
    }
  return c;
}

void resize(Hashmap* map)
{
  unsigned int i, oldSize;
  Node** old;
  if (map->filled < map->size / 2)
    {
      return;
    }
  old = map->entries;
  oldSize = map->size;
  map->filled = 0;
  map->size = nextPrime(map->size * 2);
  map->entries = calloc(map->size, sizeof(Node*));
  for (i = 0; i < oldSize; i++)
    {
      Node* n = old[i];
      Node* next;
      while (n)
	{
	  unsigned long h = map->hash(n->key) % map->size;
	  next = n->next;
	  put_help(map, h, n);
	  n = next;
	}
    }
  free(old);
}

unsigned long strHash(const char* str)
{
  unsigned long hash = 5381;
  int c;
  while ((c = *str++))
    {
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
  return hash;
}

Hashmap* newHashmap(unsigned int expected, unsigned long (*hash)(const void* key), int (*cmp)(const void* a, const void* b))
{
  Hashmap* res = calloc(1, sizeof(Hashmap));
  if (!res)
    {
      return NULL;
    }
  res->size = nextPrime(expected << 1);
  res->entries = calloc(res->size, sizeof(Node*));
  if (!res->entries)
    {
      free(res);
      return NULL;
    }
  res->hash = hash;
  res->cmp = cmp;
  return res;
}

int mapAdd(Hashmap* map, void* key, void* value)
{
  unsigned long h = map->hash(key) % map->size;
  Node* node;
  if (contains_help(map, h, key))
    {
      return 0;
    }
  node = malloc(sizeof(Node));
  if (!node)
    {
      return -1;
    }
  node->key = key;
  node->value = value;
  put_help(map, h, node);
  if (!node->next)
    {
      map->filled++;
      resize(map);
    }
  return 1;
}

int mapContains(Hashmap* map, void* key)
{
  return contains_help(map, map->hash(key) % map->size, key);
}

void* mapGet(Hashmap* map, void* key)
{
  unsigned long h = map->hash(key) % map->size;
  Node* n = map->entries[h];
  while (n)
    {
      if (!map->cmp(key, n->key))
	{
	  return n->value;
	}
      n = n->next;
    }
  return NULL;
}

void mapForEach(Hashmap* map, void (*fn)(void* key, void* val))
{
  Node* n;
  unsigned int i;
  for (i = 0; i < map->size; ++i)
    {
      n = map->entries[i];
      while (n)
	{
	  fn(n->key, n->value);
	  n = n->next;
	}
    }
}

unsigned int mapSize(Hashmap* map)
{
  return map->count;
}

void deleteMap(Hashmap* map)
{
  int i;
  Node* n;
  Node* tmp;
  for (i = 0; i < map->size; ++i)
    {
      n = map->entries[i];
      while (n)
	{
	  tmp = n;
	  n = n->next;
	  free(tmp);
	}
    }
  free(map->entries);
  free(map);
}
