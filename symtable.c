#include "symtable.h"
#include "bst.h"

void symTableInit(symtable* symtable) {
  BSTInit(&(symtable->root));
}

ERROR_CODE symTableInsert(symtable* Table, string Key, bool isfunction) {
	ERROR_CODE result;
	//když je isfunction true, je to funkce a když false tak je to proměnná
	if (isfunction) {
	result = BSTInsert(&(Table->root), Key.value, Function, undefined, false, -1);
	return result;
	}
	result = BSTInsert(&(Table->root), Key.value, Variable, undefined, false, -1);
	return result;
}

tBSTNodePtr symTableSearch(symtable* Table, string Key) { // vraci ukazatel na hledany uzel, pokud nenajde vraci NULL
    return BSTSearch(Table->root, Key.value);
}


void symTableDelete(symtable* Table, string Key) {
    BSTDelete(&(Table->root), Key.value);
}


void symTableDispose(symtable* Table) {
    BSTDispose(&(Table->root));
}
