/**
* Projekt IFJ/IAL 2019 - Překladač imperativního jazyka IFJ19
*
* @file exp_stack.c
* @brief Pomocný stack pro práci s výrazy
*
* @author Daniel Čechák (xcecha06)
*/

#include "exp_stack.h"

unsigned int deletedItems = 0;
unsigned int stackSize = 0;

// Inicializuje zásobník
ERROR_CODE exp_stackInit (ptrStack* stack) {
    stack->top_of_stack = NULL;
    int type = TOKEN_UNDEF;
    Exp_element *new_element = newElement(type,false);
    return exp_stackPush(stack, new_element);
}

// vrátí true pokud je zásobník prázdný
bool exp_stackEmpty (ptrStack* stack ) {
    if (stack->top_of_stack != NULL) {
        return (stack->top_of_stack->value->type == TOKEN_UNDEF);
    }
    else {
        return 1;
    }
    
}

// Uvolní vrchní prvek ze zásobníku
void exp_stackPop (ptrStack* stack ) {
    ptStack *tmp = NULL;
    if (stack->top_of_stack != NULL) {
        tmp = stack->top_of_stack;
        stack->top_of_stack = stack->top_of_stack->left;
        free(tmp);
        stackSize--;
    }
}

// vytvoří nové paměťové místo na zásobníku a vloží do něj data
ERROR_CODE exp_stackPush (ptrStack* stack, void * value) {
    ptStack *tmp = malloc(sizeof(struct ptstack_structure));
    if(tmp != NULL) {
        tmp->value = value;
        tmp->left = stack->top_of_stack;
        stack->top_of_stack = tmp;
        stackSize++;
        return ERROR_CODE_OK;
    }
    return ERROR_CODE_INTERNAL;
}

// uvolnění postupně celý zásobník
bool exp_stackClear(ptrStack *stack){
    while(stack->top_of_stack != NULL){
        exp_stackPop(stack);
        deletedItems++;
    }
    return true;
}
