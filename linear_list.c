/**
* Projekt IFJ/IAL 2019 - Překladač imperativního jazyka IFJ19
*
* @file main.c
*
* @author Daniel Pátek (xpatek08)
*/

#include "linear_list.h"

void DLInitList(instruction_list *List) {
    //init linear list
    List->Actual = NULL;
    List->First = NULL;
    List->Last = NULL;

    //generate start of the code
    addInstruction(List, START, NULL, NULL, NULL);
    addInstruction(List, COMMENT, "-------------------start------------------", NULL, NULL);
    addInstruction(List, DEFVAR, "GF@tmp", NULL, NULL);
    addInstruction(List, DEFVAR, "GF@tmp1", NULL, NULL);
    addInstruction(List, DEFVAR, "GF@tmp2", NULL, NULL);
    addInstruction(List, JUMP, "$$start", NULL, NULL);
    addInstruction(List, COMMENT, "-------------------len()------------------", NULL, NULL);
    lenFunction();
    addInstruction(List, COMMENT, "-------------------ord()------------------", NULL, NULL);
    ordFunction();
    addInstruction(List, COMMENT, "-------------------chr()------------------", NULL, NULL);
    chrFunction();
    addInstruction(List, COMMENT, "-------------------inputs()---------------", NULL, NULL);
    inputSFunction();
    addInstruction(List, COMMENT, "-------------------inputi()---------------", NULL, NULL);
    inputIFunction();    
    addInstruction(List, COMMENT, "-------------------inputf()---------------", NULL, NULL);
    inputFFunction();
    addInstruction(List, COMMENT, "-------------------print()----------------", NULL, NULL);
    printFunction();
    addInstruction(List, COMMENT, "------------------------------------------", NULL, NULL);
    addInstruction(List, COMMENT, "----konec generovani vestavench funkci----", NULL, NULL);
    addInstruction(List, COMMENT, "------------------------------------------", NULL, NULL);
    addInstruction(List, LABEL, "$$start", NULL, NULL);
}

void DLDisposeList (instruction_list *L) {
    //variable to store ptr to next element
    instruction_node next = NULL;

    //starting from first element
    L->Actual = L->First;

    //free element to element till the end
    while (L->Actual != NULL) {
        next = L->Actual->rptr;
        free(L->Actual);
        L->Actual = next;
    }

    //initialize again
    L->Actual = NULL;
    L->First = NULL;
    L->Last = NULL;

}

//insert last member to the list
void DLInsertLast(instruction_list *L, instruction val) {
    //allocate memory and check if it was succesfull
    struct instruction_node *newElmPtr = (struct instruction_node*) malloc(sizeof(struct instruction_node));
    if (newElmPtr == NULL) {
        printf("ERROR");
    } else {
        newElmPtr->inst = val; //add inst to new item
        newElmPtr->lptr = L->Last; //connect new one to the previous one
        newElmPtr->rptr = NULL; //it has not next item so right side will be NULL

        //check if the list was empty
        if (L->Last != NULL) {
            L->Last->rptr = newElmPtr; //connect previous one to the new one
        } else {
            L->First = newElmPtr; //make it one and only item (first item)
        }
        L->Last = newElmPtr; //make it last item
    }
}

//add active status to first member
void DLFirst (instruction_list *L) {
    L->Actual = L->First;
}

//copy actual member
void DLCopy (instruction_list *L, instruction *val) {
    if (L->Actual == NULL) {
        ;
    } else {
        *val = L->Actual->inst;
    }
}

//move one member ahead
void DLSucc (instruction_list *L) {
    if (L->Actual != NULL) {
        L->Actual = L->Actual->rptr;
    }
}

//check if some is active
int DLActive (instruction_list *L) {
    return L->Actual != NULL;
}