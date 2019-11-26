
#include "symtable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void BSTInit (tBSTNodePtr *RootPtr) {
	*RootPtr = NULL;
}
///////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////
ERROR_CODE BSTInsert (tBSTNodePtr* RootPtr, char* K, NodeType type, int vartype, bool declared, int paramnum) {
	ERROR_CODE result = ERROR_CODE_OK;
	if (*RootPtr == NULL) {
	  	if((((*RootPtr) = (tBSTNodePtr) malloc(sizeof(struct tBSTNode))) == NULL)) {
	    	result = ERROR_CODE_INTERNAL; //malloc nesehnal dost paměti
			return result;
		}
	  	(*RootPtr)->lPtr = NULL;
	  	(*RootPtr)->rPtr = NULL;
		stringAddChars(&(*RootPtr)->Key, K);
			(*RootPtr)->DataType = type;
	  	(*RootPtr)->VarType = vartype;
			(*RootPtr)->declared = declared;
			(*RootPtr)->parametrs = paramnum;
		return result;
	}

	else if ((*RootPtr)->Key.value == K) {
		(*RootPtr)->DataType = type;
		(*RootPtr)->VarType = vartype;
		(*RootPtr)->declared = declared;
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
	return ERROR_CODE_SEM_OTHER;
}
///////////////////////////////////////////////////////////////////////////
void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
	if (RootPtr == NULL)
        return;

	else if ((*RootPtr)->rPtr == NULL) {
    	PtrReplaced->DataType = (*RootPtr)->DataType;
		PtrReplaced->VarType = (*RootPtr)->VarType;
		PtrReplaced->declared = (*RootPtr)->declared;
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
///////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////
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
























/////        FUNKCE PRO SYMTABLE         //////////////////////////////

void symTableInit(symtable* symtable) {
  BSTInit(&(symtable->root));
}

ERROR_CODE symTableInsert(symtable* Table, string Key, bool isfunction) {
	ERROR_CODE result;
	//pro teďkonc, type 1 je funkce a 0 je proměnná
	if (isfunction) {
	result = BSTInsert(&(Table->root), Key.value, Function, -1, false, -1);
	return result;
	}
	result = BSTInsert(&(Table->root), Key.value, Variable, -1, false, -1);
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
