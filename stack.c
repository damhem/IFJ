#include "stack.h"


int STACK_SIZE = MAX_STACK;

ERROR_CODE stackInit(tStack *s)
{
/*
** Provede inicializaci zásobníku - nastaví vrchol zásobníku.
*/

// V případě, že funkce dostane jako parametr NULL, vrací se ERROR_CODE_INTERNAL
// V opačném případě je zásobník inicializován s hodnotou top == -1.
    if (s == NULL) {
        return ERROR_CODE_INTERNAL;
    }
    s->top = -1;
		return ERROR_CODE_OK;;
}


int stackEmpty(const tStack *s)
{
/*
** Vrací nenulovou hodnotu, pokud je zásobník prázdný, jinak vrací hodnotu 0.
*/

//Prázdný zásobník je značen hodnotou top == -1.
	return s->top == -1;
}


int stackFull(const tStack *s)
{
/*
** Vrací nenulovou hodnotu, je-li zásobník plný, jinak vrací hodnotu 0.
*/

//První prvek v zásobníku má index 0, a tudíž je maximální velikost zásobníku STACK_SIZE-1.
	return s->top == STACK_SIZE - 1;
}


void stackTop(const tStack *s, char *c)
{
/*
** Vrací znak z vrcholu zásobníku prostřednictvím parametru c.
*/

// V případě že je zásobník prázdný a vrací nulu, vrací se ERROR_CODE_INTERNAL
  if (stackEmpty(s) != 0) {
		return;
  }

// Do ukazatele c se přiřadí hodnota z vrcholu zásobníku.
  *c = s->arr[s->top];
}


void stackPop(tStack *s)
{
/*
** Odstraní prvek z vrcholu zásobníku.
*/

//Pokud je zásobník prázdný, funkce nic nedělá, v opačném případě je z jeho vrcholu odstraněn jeden prvek.
	if (stackEmpty(s) != 0) {
		return;
	}

	s->top--;

}


ERROR_CODE stackPush(tStack *s, char c)
{
/*
** Vloží znak na vrchol zásobníku.
*/

//Pokud je zásobník plný, vrací se ERROR_CODE_INTERNAL
	if (stackFull(s) != 0) {
			return ERROR_CODE_INTERNAL;
	}
//Když je v zásobníku místo, zvýšíme index jeho vrcholu o jedničku a uložíme na něj hodnotu c.
	s->top++;
	s->arr[s->top] = c;
  return ERROR_CODE_OK;
}

void stackClear(tStack *s) {
	while (stackEmpty(s) == 0) {
		stackPop(s);
	}
}
