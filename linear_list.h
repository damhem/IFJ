/**
* Projekt IFJ/IAL 2019 - Překladač imperativního jazyka IFJ19
*
* @file linear_list.h
*
* @author Daniel Pátek (xpatek08)
*/

#ifndef IFJ_LINEAR_LIST_H
#define IFJ_LINEAR_LIST_H

#include<stdio.h>
#include<stdlib.h>

#include "generator.h"

//function declaration
void DLInsertLast(instruction_list *, instruction);
void DLCopy (instruction_list *, instruction *);
void DLSucc (instruction_list *);
void DLFirst (instruction_list *);
int DLActive (instruction_list *);
void DLInitList (instruction_list *);
void DLDisposeList (instruction_list *);

#endif
