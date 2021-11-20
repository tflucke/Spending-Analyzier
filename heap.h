#ifndef HEAP_H
#define HEAP_H

struct heap;

typedef struct heap Heap;

Heap* newHeap(unsigned int maxSize, int (*cmpfn)(const void*, const void*));

void heapAdd(Heap* heap, void* elm);

unsigned int heapSize(Heap* heap);

void* heapDeleteMax(Heap* heap);

void deleteHeap(Heap* heap);

#endif
