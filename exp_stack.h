#ifndef EXP_STACK_H
#define EXP_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error_code.h"
#include "scanner.h"

// Struktura prvku na zásobníku
typedef struct ptstack_structure{
    struct ptstack_structure *left;
    void *value;
} ptStack;

// Struktura zásboníku
typedef struct {
    ptStack *top_of_stack;
} ptrStack;

// Deklarace funkcí pro zásobník
void exp_stackInit(ptrStack *);
bool exp_stackPush(ptrStack *, void *);
bool exp_stackPop(ptrStack *);
Token exp_stackTop(ptrStack *);
bool exp_stackEmpty(ptrStack *);
bool exp_stackClear(ptrStack *);

#endif //EXP_STACK_H
