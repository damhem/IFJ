#ifndef SYMTABLE
#define SYMTABLE

#include <stdbool.h>
#include "string.h"

typedef enum {
    Variable,
    Function,
} NodeType;

typedef struct tBSTNode {
    char* Key;
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
void BSTInsert (tBSTNodePtr *, char*, NodeType, int, bool, int);
void BSTDelete (tBSTNodePtr *, char*);
void BSTDispose(tBSTNodePtr *);

#endif
