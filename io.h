#ifndef IO_H
#define IO_H

#include <time.h>
#include <stdio.h>

#define DATE_LENGTH 11 /* sizeof("YYYY-MM-DD") */
#define CAT_LENGTH 22

typedef struct
{
  time_t date;
  float amount;
  char catagory[CAT_LENGTH];
  char subcatagory[CAT_LENGTH];
} Expense;

typedef struct
{
  unsigned int days;
  float amount;
  char catagory[CAT_LENGTH];
  char subcatagory[CAT_LENGTH];
} Catagory;

typedef struct
{
  time_t start;
  time_t end;
  int today;
  enum {CATAGORY = 'c', AMOUNT = 'a', FREQUENCY = 'f'} order;
} Arguments;

int parseExpense(Expense* res, FILE* in);

void removeHeader(FILE* in);

Arguments parseArgs(int argn, char** argc);

#endif
