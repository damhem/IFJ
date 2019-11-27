#include "symtable.h"
#include "bst.h"

ERROR_CODE symTableInit(symtable* symtable) {
  BSTInit(&(symtable->root));
  tBSTNodePtr FunctionNode;
  ERROR_CODE result;
  // ERROR_CODE result; //A vsechno kolem

  BSTInsert(&(symtable->root), "inputs", Function, undefined, true, 0);
  BSTInsert(&(symtable->root), "inputi", Function, undefined, true, 0);
  BSTInsert(&(symtable->root), "inputd", Function, undefined, true, 0);
  //BSTInsert(&(Table->root), "print", Function, undefined, true, 41? lol);

  BSTInsert(&(symtable->root), "len", Function, undefined, true, 1);
  FunctionNode = BSTSearch(symtable->root, "len");
  stringInit(&FunctionNode->paramName[0]);
  stringAddChar(&(FunctionNode->paramName[0]), 's');

  BSTInsert(&(symtable->root), "substr", Function, undefined, true, 3);
  FunctionNode = BSTSearch(symtable->root, "substr");
  stringInit(&FunctionNode->paramName[0]);
  stringInit(&FunctionNode->paramName[1]);
  stringInit(&FunctionNode->paramName[2]);
  stringAddChar(&(FunctionNode->paramName[0]), 's');
  stringAddChar(&(FunctionNode->paramName[1]), 'i');
  stringAddChar(&(FunctionNode->paramName[2]), 'n');

  BSTInsert(&(symtable->root), "ord", Function, undefined, true, 2);
  FunctionNode = BSTSearch(symtable->root, "ord");
  stringInit(&FunctionNode->paramName[0]);
  stringInit(&FunctionNode->paramName[1]);
  stringAddChar(&(FunctionNode->paramName[0]), 's');
  stringAddChar(&(FunctionNode->paramName[1]), 'i');

  result = BSTInsert(&(symtable->root), "chr", Function, undefined, true, 1);
  FunctionNode = BSTSearch(symtable->root, "chr");
  stringInit(&FunctionNode->paramName[0]);
  stringAddChar(&(FunctionNode->paramName[0]), 'i');
  return result;
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
