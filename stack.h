#ifndef IFJ_STACK_H
#define IFJ_STACK_H

#include <stdio.h>


/*
 * Hodnota MAX_STACK udává skutečnou velikost statického pole pro uložení
 * hodnot zásobníku. Při implementaci operací nad ADT zásobník však
 * předpokládejte, že velikost tohoto pole je pouze STACK_SIZE. Umožní to
 * jednoduše měnit velikost zásobníku v průběhu testování. Při implementaci
 * tedy hodnotu MAX_STACK vůbec nepoužívejte.
 */
#define MAX_STACK 20

extern int STACK_SIZE;
extern int solved; /* Indikuje, zda byla operace řešena. */
extern int err_flag; /* Indikuje, zda operace volala chybu. */

/* Chybové kódy pro funkci stackError */
#define MAX_SERR 3 /* počet možných chyb */
#define SERR_INIT 1 /* chyba při stackInit */
#define SERR_PUSH 2 /* chyba při stackPush */
#define SERR_TOP 3 /* chyba při stackTop */


/* ADT zásobník implementovaný ve statickém poli */
typedef struct
{
	char arr[MAX_STACK]; /* pole pro uložení hodnot */
	int top; /* index prvku na vrcholu zásobníku */
} tStack;


/* Hlavičky funkcí pro práci se zásobníkem. */

void stackError(int error_code);

void stackInit(tStack *s);

int stackEmpty(const tStack *s);

int stackFull(const tStack *s);

void stackTop(const tStack *s, char *c);

void stackPop(tStack *s);

void stackPush(tStack *s, char c);

void stackClear(tStack *s);

#endif
