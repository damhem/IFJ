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

	else if (RootPtr->Key == K) {
		return RootPtr;
	}

	else if (RootPtr->Key < K) {
		return BSTSearch(RootPtr->rPtr, K);
	}

	return BSTSearch(RootPtr->lPtr, K);
}
///////////////////////////////////////////////////////////////////////////
void BSTInsert (tBSTNodePtr* RootPtr, char* K, NodeType type, int vartype, bool declared, int paramnum) {
	if (*RootPtr == NULL) {
		*RootPtr = malloc(sizeof( **RootPtr));
	  	if(*RootPtr == NULL) {
	    	exit(1); //malloc nesehnal dost paměti
		}
	  	(*RootPtr)->lPtr = NULL;
	  	(*RootPtr)->rPtr = NULL;
		  (*RootPtr)->Key = K;
			(*RootPtr)->DataType = type;
	  	(*RootPtr)->VarType = vartype;
			(*RootPtr)->declared = declared;
			(*RootPtr)->parametrs = paramnum;
		return;
	}

	else if ((*RootPtr)->Key == K) {
		(*RootPtr)->DataType = type;
		(*RootPtr)->VarType = vartype;
		(*RootPtr)->declared = declared;
		(*RootPtr)->parametrs = paramnum;
		return;
	}

	else if ((*RootPtr)->Key < K) {
		BSTInsert(&(*RootPtr)->rPtr, K, type, vartype, declared, paramnum);
		return;
	}

	else if ((*RootPtr)->Key > K) {
		BSTInsert(&(*RootPtr)->lPtr, K, type, vartype, declared, paramnum);
		return;
	}
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

	else if ((*RootPtr)->Key == K) {
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

	else if ((*RootPtr)->Key < K) {
		BSTDelete(&(*RootPtr)->rPtr, K);
	}

	else if ((*RootPtr)->Key > K) {
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

void symTableInsert(symtable* Table, string Key, bool isfunction) {
//pro teďkonc, type 1 je funkce a 0 je proměnná
if (isfunction) {
	BSTInsert(&(Table->root), Key.value, Function, -1, false, -1);
	return;
	}
BSTInsert(&(Table->root), Key.value, Variable, -1, false, -1);

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
