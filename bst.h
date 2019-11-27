#ifndef IFJ_BINARY_SEARCH_TREE
#define IFJ_BINARY_SEARCH_TREE

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "string.h"
#include "error_code.h"

//type of node in binary seach tree
typedef enum {
    Variable,
    Function,
} NodeType;

typedef enum {
    typeinteger,
    typedouble,
    typestring,
    undefined,
} VarType;

//one node in binary seach tree
typedef struct tBSTNode {
    string Key;                 //unique key
    NodeType DataType;          //function or variable (0 or 1)
    struct tBSTNode * lPtr;     //left subtree
    struct tBSTNode * rPtr;     //right subtree

    //variable
    VarType Vartype;            //type of variable (-1 means undefined or unknown)

    //function
    bool defined;               // if the function has been defined;
    int parametrs;              //number of function parametres
    string paramName[40];       //names of function parametres
} *tBSTNodePtr;

void BSTInit   (tBSTNodePtr *);
tBSTNodePtr BSTSearch (tBSTNodePtr, char*);
ERROR_CODE BSTInsert (tBSTNodePtr *, char*, NodeType, VarType, bool, int);
void BSTDelete (tBSTNodePtr *, char*);
void BSTDispose(tBSTNodePtr *);

#endif
