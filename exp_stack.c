#include "exp_stack.h"

unsigned int deletedItems = 0;
unsigned int stackSize = 0;

// Inicializuje zásobník
void exp_stackInit (ptrStack* stack) {
    stack->top_of_stack = NULL;
}

// vrátí true pokud je zásobník prázdný
bool exp_stackEmpty (ptrStack* stack ) {
    return (stack->top_of_stack == NULL);
}

// Uvolní vrchní prvek ze zásobníku
bool exp_stackPop (ptrStack* stack ) {
    ptStack *tmp = NULL;
    if(stack->top_of_stack != NULL){
        tmp = stack->top_of_stack;
        stack->top_of_stack = stack->top_of_stack->left;
        free(tmp);
        stackSize--;
        return true;
    }
    return false;
}

// vytvoří nové paměťové místo na zásobníku a vloží do něj data
bool exp_stackPush (ptrStack* stack, void * value) {
    ptStack *tmp = malloc(sizeof(struct ptstack_structure));
    if(tmp != NULL) {
        tmp->value = value;
        tmp->left = stack->top_of_stack;
        stack->top_of_stack = tmp;
        stackSize++;
        return true;
    }
    return false;
}

// uvolnění postupně celý zásobník
bool exp_stackClear(ptrStack *stack){
    while(stack->top_of_stack != NULL){
        exp_stackPop(stack);
        deletedItems++;
    }
    return true;
}
