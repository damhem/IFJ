#ifndef SYMTABLE
#define SYMTABLE

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "string.h"
#include "error_code.h"

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

//binary seach tree itself
typedef struct symtable {
    tBSTNodePtr root;           //this is pointer to the root node
} symtable;

//binary tree functions
void BSTInit   (tBSTNodePtr *);
tBSTNodePtr BSTSearch (tBSTNodePtr, char*);
ERROR_CODE BSTInsert (tBSTNodePtr *, char*, NodeType, VarType, bool, int);
void BSTDelete (tBSTNodePtr *, char*);
void BSTDispose(tBSTNodePtr *);

//symtable tree functions
ERROR_CODE SYMInit(symtable* symtable);
ERROR_CODE SYMInsert(symtable* Table, string Key, bool isfunction);
tBSTNodePtr SYMSearch(symtable* Table, string Key);
void SYMDelete(symtable* Table, string Key);
void SYMDispose(symtable* Table);

#endif
