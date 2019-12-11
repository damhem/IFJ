#include "stack.h"


int STACK_SIZE = MAX_STACK;


// initialization of the stack with top having the value of -1
// in case of parameter null, ERROR_CODE_INTERNAL is returned
ERROR_CODE stackInit(tStack *s) {

    if (s == NULL) {
        return ERROR_CODE_INTERNAL;
    }
    
    s->top = -1;
		return ERROR_CODE_OK;;
}

// returns zero only when the stack is not empty
int stackEmpty(const tStack *s) {

	return s->top == -1;
}

// returns zero only when the stack is not full
int stackFull(const tStack *s) {

	return s->top == STACK_SIZE - 1;
}

// returns element from the top of the stack through parameter c
void stackTop(const tStack *s, char *c) {

  // in the case of empty stack, return ERROR_CODE_INTERNAL
  if (stackEmpty(s) != 0) {
	  *c = 0;
		return;
  }

  *c = s->arr[s->top];
}

// deletes one element from the top of the stack
void stackPop(tStack *s) {
  // if the stack is full, nothing happens
	if (stackEmpty(s) != 0) {
		return;
	}

	s->top--;
}

// inserts char on the top of the stack
ERROR_CODE stackPush(tStack *s, char c) {

  // if the stack is full, returns ERROR_CODE_INTERNAL
	if (stackFull(s) != 0) {
			return ERROR_CODE_INTERNAL;
	}

  // if there is enough space, index is incremented and char is saved on the top of the stack
	s->top++;
	s->arr[s->top] = c;
  return ERROR_CODE_OK;
}

// empties the whole stack
void stackClear(tStack *s) {

	while (stackEmpty(s) == 0) {
		stackPop(s);
	}

}
