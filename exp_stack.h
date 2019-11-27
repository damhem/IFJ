#ifndef EXP_STACK_H
#define EXP_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error_code.h"
#include "scanner.h"
#include "expression.h"



// Struktura prvku na zásobníku
typedef struct ptstack_structure{
    struct ptstack_structure *left;
    void *value;
} ptStack;

// Struktura zásboníku
typedef struct ptrStack{
    ptStack *top_of_stack;
} ptrStack;

// Deklarace funkcí pro zásobník
ERROR_CODE exp_stackInit(ptrStack *);
ERROR_CODE exp_stackPush(ptrStack *, void *);
void exp_stackPop(ptrStack *);
//tToken STop(ptrStack *, void *);
void exp_stackTop(ptrStack *);
bool exp_stackEmpty(ptrStack *);
bool exp_stackClear(ptrStack *);

#endif //EXP_STACK_H
