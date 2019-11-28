

#include "linear_list.h"

void DLInitList(instruction_list *List) {
    List->Actual = NULL;
    List->First = NULL;
    List->Last = NULL;
    addInstruction(List, START, NULL, NULL, NULL);
    addInstruction(List, COMMENT, "-------------------start------------------", NULL, NULL);
    addInstruction(List, DEFVAR, "GF@tmp", NULL, NULL);
    addInstruction(List, DEFVAR, "GF@tmp1", NULL, NULL);
    addInstruction(List, DEFVAR, "GF@tmp2", NULL, NULL);
    addInstruction(List, JUMP, "$$start", NULL, NULL);
    
    //todo vestavene funkce
    //writeLength();
    //addInstruction(L, I_COMMENT, "-------------------ASC--------------------", NULL, NULL);
    //writeAsc();
    //addInstruction(L, I_COMMENT, "-------------------CHR--------------------", NULL, NULL);
    //writeChr();
    //addInstruction(L, I_COMMENT, "------------------SUBSTR------------------", NULL, NULL);
    //writeSubstr();
    addInstruction(List, COMMENT, "konec generovani vestavench funkci", NULL, NULL);
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

void DLInsertFirst (instruction_list *L, instruction val) {
    //allocate memory and check if it was succesfull
    struct instruction_node *newElmPtr = (struct instruction_node*) malloc(sizeof(struct instruction_node));
    if (newElmPtr == NULL) {
        printf("ERROR\n");
    } else {
        newElmPtr->inst = val; //add inst to new item
        newElmPtr->rptr = L->First; //connect new one to the previous one
        newElmPtr->lptr = NULL; //it has not previous item so left side will be NULL

        //check if the list was empty
        if (L->First != NULL) {
            L->First->lptr = newElmPtr; //connect previous one to the new one
        } else {
            L->Last = newElmPtr; //make it one and only item (last item)
        }
        L->First = newElmPtr; //make it first item
    }
}

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

void DLFirst (instruction_list *L) {
    L->Actual = L->First;
}

void DLLast (instruction_list *L) {
    L->Actual = L->Last;
}

void DLCopyFirst (instruction_list *L, instruction *val) {
    if (L->First == NULL) {
        printf("ddd");
    } else {
        *val = L->First->inst;
    }
}

void DLCopyLast (instruction_list *L, instruction *val) {
    if (L->First == NULL) {
        printf("sdss");
    } else {
        *val = L->Last->inst;
    }
}

void DLDeleteFirst (instruction_list *L) {
    //create new null pointer
    instruction_node itemPtr = NULL;

    //work only with not empty list
    if (L != NULL){
        itemPtr = L->First; //assign first item to help pointer
        if (L->First == L->Actual) { //if it was Active item
            L->Actual = NULL; //it loses Active status
        }
        if (L->First == L->Last) { //if it was the only item
            L->First = NULL;
            L->Last = NULL; //list will be empty
        }
        else {
            L->First = L->First->rptr; //shift list to start at next item
            L->First->lptr = NULL; //unconnect previusly first item
        }
        free(itemPtr); //free memory
    }
}

void DLDeleteLast (instruction_list *L) {
    //create new null pointer
    instruction_node itemPtr = NULL;

    //work only with not empty list
    if (L != NULL){
        itemPtr = L->Last; //assign last item to help pointer
        if (L->Last == L->Actual) { //if it was Active item
            L->Actual = NULL; //it loses Active status
        }
        if (L->First == L->Last) { //if it was the only item
            L->First = NULL;
            L->Last = NULL; //list will be empty
        }
        else {
            L->Last = L->Last->lptr; //shift list to end at previous item
            L->Last->rptr = NULL; //unconnect previusly last item
        }
        free(itemPtr); //free memory
    }
}

/*void DLPostDelete (instruction_list *L) {
    //create new null pointer
    instruction_node itemPtr = NULL;

    //delete only when list Active and last is not Active
    if (L->Actual != NULL && L->Actual != L->Last && L->Actual->rptr != NULL){
        itemPtr = L->Actual->rptr; //assing to the new pointer
        L->Actual->rptr = itemPtr->rptr; //connect the two items around the deleting one
        if (itemPtr == L->Last) { //if its the last item
            L->Last = L->Actual; //Active is last
        } else {
            itemPtr->rptr->lptr = L->Actual; //connect the next to the Active one
        }
        free(itemPtr); //free memory
    }
}*/
/*
void DLPreDelete (instruction_list *L) {

** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.

    //create new null pointer
    instruction_node itemPtr = NULL;

    //delete only when list Active and first is not Active
    if (L->Actual != NULL && L->Actual != L->First && L->Actual->lptr != NULL){
        itemPtr = L->Actual->lptr; //assing to the new pointer
        L->Actual->lptr = itemPtr->lptr; //connect the two items around the deleting one
        if (itemPtr == L->First) { //if its the first item
            L->First = L->Actual; //Active is last
        } else {
            itemPtr->lptr->rptr = L->Actual; //connect the previous to the Active one
        }
        free(itemPtr); //free memory
    }
}

void DLPostInsert (instruction_list *L, instruction val) {

    //continue just if there is Active item
    if (L->Actual != NULL) {
        //allocate memory for new item
        struct instruction_node *newElmPtr = (struct instruction_node*) malloc(sizeof(struct instruction_node));
        if (newElmPtr == NULL ) { //not continue if allocation was unsuccessful
            DLError();
        } else {
            newElmPtr->inst = val; //assing inst
            newElmPtr->lptr = L->Actual; //connect left from new item
            newElmPtr->rptr = L->Actual->rptr; //connect right from new item
            L->Actual->rptr = newElmPtr; //connect left to new item

            if(L->Last == L->Actual){ //if the item is last item
                L->Last = newElmPtr;
            } else {
                newElmPtr->rptr->lptr = newElmPtr; //connect right to new item
            }
        }
    }
}
*/
//void DLPreInsert (instruction_list *L, instruction val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().

    //continue just if there is Active item
    if (L->Actual != NULL) {
        //allocate memory for new item
        struct instruction_node *newElmPtr = (struct instruction_node*) malloc(sizeof(struct instruction_node));
        if (newElmPtr == NULL ) { //not continue if allocation was unsuccessful
            DLError();
        } else {
            newElmPtr->inst = val; //assing inst
            newElmPtr->rptr = L->Actual; //connect right from new item
            newElmPtr->lptr = L->Actual->lptr; //connect left from new item
            L->Actual->lptr = newElmPtr; //connect right to new item

            if(L->First == L->Actual){ //if the item is first item
                L->First = newElmPtr;
            } else {
                newElmPtr->lptr->rptr = newElmPtr; //connect left to new item
            }
        }
    }
}
*/

void DLCopy (instruction_list *L, instruction *val) {
    if (L->Actual == NULL) {
        printf("ERROR");
    } else {
        *val = L->Actual->inst;
    }
}

void DLActualize (instruction_list *L, instruction val) {
    if (L->Actual != NULL) {
        L->Actual->inst = val;
    }
}

void DLSucc (instruction_list *L) {
    if (L->Actual != NULL) {
        L->Actual = L->Actual->rptr;
    }
}


void DLPred (instruction_list *L) {
    if (L->Actual != NULL) {
        L->Actual = L->Actual->lptr;
    }
}

int DLActive (instruction_list *L) {
    return L->Actual != NULL;
}