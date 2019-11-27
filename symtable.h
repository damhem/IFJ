#ifndef SYMTABLE
#define SYMTABLE

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "string.h"
#include "error_code.h"

//type of node in binary seach tree
typedef enum {
    Variable,
    Function
} NodeType;

//one node in binary seach tree
typedef struct tBSTNode {
    string Key;                 //unique key
    NodeType DataType;          //function or variable (0 or 1)
    struct tBSTNode * lPtr;     //left subtree
    struct tBSTNode * rPtr;     //right subtree

    //variable
    int VarType;                //type of variable (-1 means undefined or unknown)

    //function
    bool defined;               // if the function has been defined
    int parametrs;              //number of function parametres
    string paramName[40];       //names of function parametres
} *tBSTNodePtr;

//binary seach tree itself
typedef struct symtable {
    tBSTNodePtr root;           //this is pointer to the root node
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
