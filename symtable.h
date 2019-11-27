#ifndef SYMTABLE
#define SYMTABLE

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "string.h"
#include "error_code.h"
#include "bst.h"

//binary seach tree itself
typedef struct symtable {
    tBSTNodePtr root;           //this is pointer to the root node
} symtable;

void symTableInit(symtable* symtable);
ERROR_CODE symTableInsert(symtable* Table, string Key, bool isfunction);
tBSTNodePtr symTableSearch(symtable* Table, string Key);
void symTableDelete(symtable* Table, string Key);
void symTableDispose(symtable* Table);

#endif
