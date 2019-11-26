#ifndef SYMTABLE
#define SYMTABLE

#include <stdbool.h>
#include "string.h"
#include "error_code.h"

typedef enum {
    Variable,
    Function,
} NodeType;

typedef struct tBSTNode {
    string Key;
    NodeType DataType;
    int VarType;
    bool declared;
    int parametrs;
    string paramName[40];
    struct tBSTNode * lPtr;
    struct tBSTNode * rPtr;
} *tBSTNodePtr;

typedef struct symtable {
    tBSTNodePtr root;
} symtable;


void BSTInit   (tBSTNodePtr *);
tBSTNodePtr BSTSearch (tBSTNodePtr, char*);
ERROR_CODE BSTInsert (tBSTNodePtr *, char*, NodeType, int, bool, int);
void BSTDelete (tBSTNodePtr *, char*);
void BSTDispose(tBSTNodePtr *);

void symTableInit(symtable* symtable);
ERROR_CODE symTableInsert(symtable* Table, string Key, bool isfunction);
tBSTNodePtr symTableSearch(symtable* Table, string Key);
void symTableDelete(symtable* Table, string Key);
void symTableDispose(symtable* Table);

#endif
