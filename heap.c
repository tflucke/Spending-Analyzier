#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

struct heap
{
   void** arr;
   unsigned int length;
   unsigned int maxLength;
   int (*cmpfn)(const void*, const void*);
};

Heap* newHeap(unsigned int maxSize, int (*cmpfn)(const void*, const void*))
{
  Heap* res = malloc(sizeof(Heap));
  if (res == NULL)
    {
      return NULL;
    }
  res->arr = malloc(sizeof(void*)*maxSize);
  if (res->arr == NULL)
    {
      free(res);
      return NULL;
    }
  res->maxLength = maxSize;
  res->length = 0;
  res->cmpfn = cmpfn;
  return res;
}

void bubbleUp(Heap* heap, unsigned int index, void* item)
{
   int pIndex = (index - 1) / 2;
   if (pIndex < heap->length && heap->cmpfn(item, heap->arr[pIndex]) > 0)
   {
      heap->arr[index] = heap->arr[pIndex];
      bubbleUp(heap, pIndex, item);
   }
   else
   {
      heap->arr[index] = item;
   }
}

unsigned int heapSize(Heap* heap)
{
  return heap->length;
}

void heapAdd(Heap* heap, void* elm)
{
   if (heap->length == heap->maxLength)
   {
      heap->maxLength <<= 1;
      heap->arr = realloc(heap->arr, heap->maxLength*sizeof(void*));
      if (heap->arr == NULL)
      {
         perror("wf");
         exit(EXIT_FAILURE);
      }
   }
   bubbleUp(heap, heap->length++, elm);
}

int findHole(Heap* heap, int parent, void* val)
{
   int left = 2 * parent + 1, right = 2 * parent + 2;
   if (left >= heap->length)
   {
      return -1;
   }
   if (heap->cmpfn(heap->arr[left], val) > 0 &&
      (right >= heap->length ||
      heap->cmpfn(heap->arr[left], heap->arr[right]) > 0))
   {
      return left;
   }
   else if (right < heap->length && heap->cmpfn(heap->arr[right], val) > 0)
   {
      return right;
   }
   return -1;
}

void bubbleDown(Heap* heap, int index, void* val)
{
   int hole = findHole(heap, index, val);
   if (hole < 0)
   {
      heap->arr[index] = val;
   }
   else
   {
      heap->arr[index] = heap->arr[hole];
      bubbleDown(heap, hole, val);
   }
}

void* heapDeleteMax(Heap* heap)
{
   if (heap->length)
   {
      void* res = heap->arr[0];
      bubbleDown(heap, 0, heap->arr[--(heap->length)]);
      return res;
   }
   return NULL;
}

void deleteHeap(Heap* heap)
{
  free(heap->arr);
  free(heap);
}
