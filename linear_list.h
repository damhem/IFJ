
#ifndef IFJ_LINEAR_LIST_H
#define IFJ_LINEAR_LIST_H
#include<stdio.h>
#include<stdlib.h>

#include "generator.h"

void DLInsertLast(instruction_list *, instruction);
void DLCopy (instruction_list *, instruction *);
void DLSucc (instruction_list *);
void DLFirst (instruction_list *);
int DLActive (instruction_list *);
void DLInitList (instruction_list *);
void DLDisposeList (instruction_list *);
/*


void DLInsertFirst (instruction_list *, int);


void DLLast (instruction_list *);
void DLCopyFirst (instruction_list *, int *);
void DLCopyLast (instruction_list *, int *);
void DLDeleteFirst (instruction_list *);
void DLDeleteLast (instruction_list *);
void DLPostDelete (instruction_list *);
void DLPreDelete (instruction_list *);
void DLPostInsert (instruction_list *, int);
void DLPreInsert (instruction_list *, int);

void DLActualize (instruction_list *, int);

void DLPred (instruction_list *);

*/
#endif
