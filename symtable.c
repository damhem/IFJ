#include "symtable.h"

// BINARY TREE FUNCTIONS ///////////////////////////////////////////////////////

void BSTInit (tBSTNodePtr *RootPtr) {
	*RootPtr = NULL;
}

tBSTNodePtr BSTSearch (tBSTNodePtr RootPtr, char* K)	{
	if (RootPtr == NULL) {
		return NULL;
	}
	else if (strcmp(K, RootPtr->Key.value) > 0) {
        return BSTSearch(RootPtr->rPtr, K);
	}
	else if (strcmp(K, RootPtr->Key.value) < 0) {
		return BSTSearch(RootPtr->lPtr, K);
	}
	else {
		return RootPtr;
	}
}

ERROR_CODE BSTInsert (tBSTNodePtr* RootPtr, char* K, NodeType type, VarType vartype, bool declared, int paramnum) {
	ERROR_CODE result = ERROR_CODE_OK;
	if (*RootPtr == NULL) {
		//jeste neni root -> musi se vytvorit
		if((((*RootPtr) = (tBSTNodePtr) malloc(sizeof(struct tBSTNode))) == NULL)) {
	    	result = ERROR_CODE_INTERNAL; //malloc nesehnal dost paměti
			return result;
		}
		(*RootPtr)->lPtr = NULL;
		(*RootPtr)->rPtr = NULL;
		stringInit(&(*RootPtr)->Key);
		stringAddChars(&(*RootPtr)->Key, K);
		(*RootPtr)->DataType = type;
		(*RootPtr)->Vartype = vartype;
		(*RootPtr)->defined = declared;
		(*RootPtr)->parametrs = paramnum;
		return result;
	}
	else if (strcmp((*RootPtr)->Key.value, K) == 0) {
		(*RootPtr)->DataType = type;
		(*RootPtr)->Vartype = vartype;
		(*RootPtr)->defined = declared;
		(*RootPtr)->parametrs = paramnum;
		return result;
	}

	else if (strcmp(K, (*RootPtr)->Key.value) < 0) {
		result = BSTInsert(&(*RootPtr)->lPtr, K, type, vartype, declared, paramnum);
		return result;
	}

	else if (strcmp(K, (*RootPtr)->Key.value) > 0) {
		result = BSTInsert(&(*RootPtr)->rPtr, K, type, vartype, declared, paramnum);
		return result;
	}
	//printf("Ssss");
	return ERROR_CODE_SEM_OTHER;

}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
	if (RootPtr == NULL)
    return;

	else if ((*RootPtr)->rPtr == NULL) {
    PtrReplaced->DataType = (*RootPtr)->DataType;
		PtrReplaced->Vartype = (*RootPtr)->Vartype;
		PtrReplaced->defined = (*RootPtr)->defined;
		PtrReplaced->parametrs = (*RootPtr)->parametrs;

    PtrReplaced->Key = (*RootPtr)->Key;

		tBSTNodePtr reserve = *RootPtr;
		*RootPtr = (*RootPtr)->lPtr;
	  free(reserve);
  }

	else {
    ReplaceByRightmost(PtrReplaced, &(*RootPtr)->rPtr);
  }
}

void BSTDelete (tBSTNodePtr *RootPtr, char* K) {
	if (*RootPtr == NULL) {
        return ;
	}

	else if ((*RootPtr)->Key.value == K) {
		tBSTNodePtr reserve = *RootPtr;
		if ((*RootPtr)->rPtr == NULL) {
			*RootPtr = reserve->lPtr;
			free(reserve);
		}
		else if ((*RootPtr)->lPtr == NULL) {
			*RootPtr = reserve->rPtr;
			free(reserve);

		}
		else {
			ReplaceByRightmost(*RootPtr, &(*RootPtr)->lPtr);
		}
	}

	else if (strcmp(K, (*RootPtr)->Key.value) > 0) {
		BSTDelete(&(*RootPtr)->rPtr, K);
	}

	else if (strcmp(K, (*RootPtr)->Key.value) < 0) {
		BSTDelete(&(*RootPtr)->lPtr, K);
	}
}

void BSTDispose (tBSTNodePtr *RootPtr) {
	if (*RootPtr == NULL)
    	return ;

	else {
		BSTDispose(&(*RootPtr)->rPtr);
		BSTDispose(&(*RootPtr)->lPtr);
		free(*RootPtr);
		*RootPtr = NULL;
	}
}

// SYMTABLE FUNCTIONS //////////////////////////////////////////////////////////

ERROR_CODE SYMInit(symtable* symtable) {
  BSTInit(&(symtable->root));
  tBSTNodePtr FunctionNode;
  ERROR_CODE result;

  BSTInsert(&(symtable->root), "inputs", Function, undefined, true, 0);
  BSTInsert(&(symtable->root), "inputi", Function, undefined, true, 0);
  BSTInsert(&(symtable->root), "inputf", Function, undefined, true, 0);
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

ERROR_CODE SYMInsert(symtable* Table, string Key, bool isfunction) {
	ERROR_CODE result;
	//když je isfunction true, je to funkce a když false tak je to proměnná
	if (isfunction) {
	result = BSTInsert(&(Table->root), Key.value, Function, undefined, false, -1);
	return result;
	}
	result = BSTInsert(&(Table->root), Key.value, Variable, undefined, false, -1);
	return result;
}

tBSTNodePtr SYMSearch(symtable* Table, string Key) { // vraci ukazatel na hledany uzel, pokud nenajde vraci NULL
    return BSTSearch(Table->root, Key.value);
}


void SYMDelete(symtable* Table, string Key) {
    BSTDelete(&(Table->root), Key.value);
}


void SYMDispose(symtable* Table) {
    BSTDispose(&(Table->root));
}
