#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

typedef enum {
    Variable,     
    Function,     
} DataType;

typedef struct tBSTNode {
    char* Key;			                             
    DataType nodeDataType;                      
    void* Data;                                     
    struct tBSTNode * lPtr;                          
    struct tBSTNode * rPtr;                          
} *tBSTNodePtr;

void BSTInit   (tBSTNodePtr *);
tBSTNodePtr BSTSearch (tBSTNodePtr, char*);
void BSTInsert (tBSTNodePtr *, char*, void*, DataType);
void BSTDelete (tBSTNodePtr *, char*);
void BSTDispose(tBSTNodePtr *);

#endif