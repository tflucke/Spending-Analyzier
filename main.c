#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "io.h"
#include "hashmap.h"
#include "heap.h"

typedef struct
{
  time_t date;
  float amount;
} Exp;

Exp* toExp(Expense e)
{
  Exp* res = malloc(sizeof(Exp));
  res->amount = e.amount;
  res->date = e.date;
  return res;
}

int dateCmp(const void* _a, const void* _b)
{
  const Exp* a = _a;
  const Exp* b = _b;
  return b->date - a->date;
}

unsigned long hashCatagory(const void* key)
{
  const Catagory* cata = key;
  return strHash(cata->catagory) ^ strHash(cata->subcatagory);
}

int cmpCatagory(const void* _a, const void* _b)
{
  const Catagory* a = _a;
  const Catagory* b = _b;
  int r = strcmp(b->catagory, a->catagory);
  return r? r : strcmp(b->subcatagory, a->subcatagory);
}

int cmpCatagoryAmount(const void* _a, const void* _b)
{
  const Catagory* a = _a;
  const Catagory* b = _b;
  return a->amount - b->amount;
}

int cmpCatagoryFrequency(const void* _a, const void* _b)
{
  const Catagory* a = _a;
  const Catagory* b = _b;
  return a->days != 0 && b->days != 0? b->days - a->days : a->days - b->days;
}

static Heap* tree;
static int today;
static time_t now;

void calcExpense(void* key, void* value)
{
  Catagory* cata = key;
  Heap* heap = value;
  Exp* last = heapDeleteMax(heap);
  unsigned int n = 1;
  unsigned long totalDateDiff = 0;
  double totalAmount = last->amount;
  Exp* cur;
  while ((cur = heapDeleteMax(heap)) != NULL)
    {
      ++n;
      totalAmount += cur->amount;
      totalDateDiff += cur->date - last->date;
      free(last);
      last = cur;
    }
  if (today)
    {
      totalDateDiff += now - last->date;
      ++n;
    }
  cata->days = n > 1? round(totalDateDiff/(n - 1.0)/(60*60*24)) : 0;
  cata->amount = totalAmount / n;
  free(last);
  deleteHeap(heap);
  heapAdd(tree, cata);
}

int main(int argn, char** argc)
{
  Expense exp;
  Hashmap* map = newHashmap(20, hashCatagory, cmpCatagory);
  Catagory* cata;
  Arguments arg = parseArgs(argn, argc);
  now = time(NULL);
  removeHeader(stdin);
  while (parseExpense(&exp, stdin))
    {
      if (exp.date < arg.start)
	{
	  continue;
	}
      else if (exp.date > arg.end)
	{
	  break;
	}
      cata = malloc(sizeof(Catagory));
      strcpy(cata->catagory, exp.catagory);
      strcpy(cata->subcatagory, exp.subcatagory);
      if ((tree = mapGet(map, cata)) == NULL)
	{
	  tree = newHeap(20, dateCmp);
	  mapAdd(map, cata, tree);
	}
      else
	{
	  free(cata);
	}
      heapAdd(tree, toExp(exp));
    }
  tree = newHeap(mapSize(map), arg.order == CATAGORY? cmpCatagory : (arg.order == AMOUNT? cmpCatagoryAmount : cmpCatagoryFrequency));
  today = arg.today;
  mapForEach(map, calcExpense);
  deleteMap(map);
  printf("%*s,%*s,%*s,%*s", CAT_LENGTH, "Catagory", CAT_LENGTH, "Subcatagory", 13, "Average Days", 20, "Average Amount\n");
  while ((cata = heapDeleteMax(tree)) != NULL)
    {
      printf("%*s,%*s,%*u,\t%*.2f\n", CAT_LENGTH, cata->catagory, CAT_LENGTH, cata->subcatagory, 13, cata->days, 15, cata->amount);
      free(cata);
    }
  deleteHeap(tree);
  return 0;
}
