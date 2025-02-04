/**
* Projekt IFJ/IAL 2019 - Překladač imperativního jazyka IFJ19
*
* @file stack.h
*
* @author Zdeněk Kroča (xkroca02)
*/


#ifndef IFJ_STACK_H
#define IFJ_STACK_H

#include <stdio.h>
#include "error_code.h"


#define MAX_STACK 100

int STACK_SIZE;

typedef struct {
	char arr[MAX_STACK];  // array for storing elements
	int top;              // index of the element on top
} tStack;



// stack functions
void stackError(int error_code);
ERROR_CODE stackInit(tStack* s);
int stackEmpty(const tStack* s);
int stackFull(const tStack* s);
void stackTop(const tStack* s, char* c);
void stackPop(tStack* s);
ERROR_CODE stackPush(tStack* s, char c);
void stackClear(tStack* s);

#endif
