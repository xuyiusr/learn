#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <pthread.h>
#include <assert.h>

typedef int (*callback)(void*, int, char**, char**);

int sqlite3_open(
  const char *filename,   /* Database filename (UTF-8) */
  sqlite3 **ppDb          /* OUT: SQLite db handle */
);

int sqlite3_exec(
  sqlite3*,                                  /* An open database */
  const char *sql,                           /* SQL to be evaluated */
  int (*callback)(void*,int,char**,char**),  /* Callback function */
  void *,                                    /* 1st argument to callback */
  char **errmsg                              /* Error msg written here */
);

int sqlite3_close(sqlite3 *);
