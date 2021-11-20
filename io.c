#include "io.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#define STR2(X) #X
#define STR(X) STR2(X)

void clearLine(FILE* f)
{
  int c, inQuote = 0;
  while (((c = getc(f)) != EOF && c != '\n') || inQuote)
    {
      if (c == '"')
	{
	  inQuote = !inQuote;
	}
    }
}

time_t toTime(char* dateStr)
{
  struct tm time;
  memset(&time, 0, sizeof(time));
  if (strptime(dateStr, "%Y-%m-%d", &time) == NULL)
    {
      fprintf(stderr, "Invalid date format: %s\n", dateStr);
      exit(302);
    }
  return mktime(&time);
}

int parseExpense(Expense* res, FILE* in)
{
  const static char* format = "%"STR(DATE_LENGTH)"[^,],\"%"STR(CAT_LENGTH)"[^\"]\",\"%"STR(CAT_LENGTH)"[^\"]\",\" %f\",";
  char dateStr[DATE_LENGTH];
  int i;
  dateStr[0] = '\0';
  if ((i = fscanf(in, format, dateStr, res->catagory, res->subcatagory, &res->amount)) == EOF)
    {
      return 0;
    }
  else if (i != 4)
    {
      fprintf(stderr, "Invalid input format\n");
      exit(301);
    }
  res->date = toTime(dateStr);
  clearLine(in);
  return 1;
}

void removeHeader(FILE* in)
{
  if (fscanf(in, "Date,Category,SubCategory,Expense Amount") == -1)
    {
      fprintf(stderr, "Proper header not present");
      exit(302);
    }
  clearLine(in);
}

void printUsage(char* prgm)
{
  fprintf(stderr, "Usage: %s [-s YYYY-MM-DD] [-e YYYY-MM-DD] [-o (c,a,f)] [-t]\n", prgm);
  exit(EXIT_FAILURE);
}

Arguments parseArgs(int argn, char** args)
{
  Arguments res;
  int opt;
  res.start = 0;
  res.end = LONG_MAX;
  res.today = 0;
  res.order = CATAGORY;
  while ((opt = getopt(argn, args, "s:e:o:t")) != -1)
    {
      switch (opt)
	{
	case 'e':
	  res.end = toTime(optarg);
	  break;
	case 's':
	  res.start = toTime(optarg);
	  break;
	case 'o':
	  res.order = optarg[0];
	  if (res.order != CATAGORY && res.order != AMOUNT && res.order != FREQUENCY)
	    {
	      printUsage(args[0]);
	    }
	  break;
	case 't':
	  res.today = 1;
	  break;
	default:
	    printUsage(args[0]);
	}
    }
  return res;
}
