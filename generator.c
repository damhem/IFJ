/**
* Projekt IFJ/IAL 2019 - Překladač imperativního jazyka IFJ19
*
* @file generator.c
*
* @author Daniel Pátek (xpatek08)
*/

#include "generator.h"
#include "linear_list.h"
#include "scanner.h"





operand initOperand(operand operand, char * value, int type, frame frame, bool is_temp, bool is_label){
    stringInit(&(operand.value));
    stringAddChars(&(operand.value), value);
    operand.type = type; //token type
    operand.frame = frame;
    operand.is_temp = is_temp;
    operand.is_label = is_label;
    return operand;
}

void noOperandInstr(struct instruction_list *List, inst_type type) {
    addInstruction(List, type, NULL, NULL, NULL);
}


void oneOperandInstr(instruction_list *List, int instType, operand first){
    
    string operand1;
    stringInit(&operand1);
    string temp_operand1;
    stringInit(&temp_operand1);

    stringAddChars(&temp_operand1, first.value.value);

    int temp_operand1_type = first.type;
    bool temp_operand1_is_temp = first.is_temp;
    //bool temp_operand1_isScope = first.isScope;
    bool temp_operand1_is_label = first.is_label;
    frame temp_operand1_frame = first.frame;

    // Operand is variable
    if(temp_operand1_type == TOKEN_ID) {
        // Operand is temp variable
        if (temp_operand1_is_temp == true) {
            stringClear(&operand1);
            stringAddChars(&operand1, TMP);
        }
        else if (temp_operand1_frame == LF) {
            stringClear(&operand1);
            stringAddChars(&operand1, FRAMELF);
            stringAddChars(&operand1, temp_operand1.value);

        }
        else if (temp_operand1_frame == GF) {
            stringClear(&operand1);
            stringAddChars(&operand1, FRAMEGF);
            stringAddChars(&operand1, temp_operand1.value);
        }
        else /* při práci s proměnnou na Temporary frame */{
            stringClear(&operand1);
            stringAddChars(&operand1, FRAMETF);
            stringAddChars(&operand1, temp_operand1.value);
        }
    }
    // Operand je konstanta
    else if(temp_operand1_type == TOKEN_INT){
        stringClear(&operand1);
        stringAddChars(&operand1, TYPEINT);
        stringAddChars(&operand1, temp_operand1.value);
    }
    else if(temp_operand1_type == TOKEN_DOUBLE){
        stringClear(&operand1);
        stringAddChars(&operand1, TYPEDOUBLE);
        stringAddChars(&operand1, temp_operand1.value);
    }
    else if(temp_operand1_type == TOKEN_STRING){
        stringClear(&operand1);
        stringAddChars(&operand1, TYPESTRING);
        stringAddChars(&operand1, temp_operand1.value);
    }
    // Operand je návěští
    if(temp_operand1_is_label == true) {
        stringClear(&operand1);
        stringAddChars(&operand1, LABELSYMBOL);
        stringAddChars(&operand1, temp_operand1.value);
    }
    /*
    if (temp_operand1_isScope == true) {
        stringClear(&operand1);
        stringAddChars(&operand1, SCOPE);
    }
*/
    addInstruction(List, instType, operand1.value, NULL, NULL);

}


void twoOperandInstr(instruction_list *L, int instType, operand first, operand second){
    
    string operand1;
    stringInit(&operand1);
    string temp_operand1;
    stringInit(&temp_operand1);
    
    stringAddChars(&temp_operand1, first.value.value);

    int temp_operand1_type = first.type;
    bool temp_operand1_is_temp = first.is_temp;
    //bool temp_operand1_isScope = first.isScope;
    bool temp_operand1_is_label = first.is_label;
    frame temp_operand1_frame = first.frame;
    
    if(temp_operand1_type == TOKEN_ID) {
        
        if (temp_operand1_is_temp == true) {
            stringClear(&operand1);
            stringAddChars(&operand1, TMP);
        }
        else if (temp_operand1_frame == LF) {
            stringClear(&operand1);
            stringAddChars(&operand1, FRAMELF);
            stringAddChars(&operand1, temp_operand1.value);

        }
        else if (temp_operand1_frame == GF) {
            stringClear(&operand1);
            stringAddChars(&operand1, FRAMEGF);
            stringAddChars(&operand1, temp_operand1.value);

        }
        else {
            stringClear(&operand1);
            stringAddChars(&operand1, FRAMETF);
            stringAddChars(&operand1, temp_operand1.value);
        }
    }
    else if(temp_operand1_type == TOKEN_INT){
        stringClear(&operand1);
        stringAddChars(&operand1, TYPEINT);
        stringAddChars(&operand1, temp_operand1.value);
    }
    else if(temp_operand1_type == TOKEN_DOUBLE){
        stringClear(&operand1);
        stringAddChars(&operand1, TYPEDOUBLE);
        stringAddChars(&operand1, temp_operand1.value);
    }
    else if(temp_operand1_type == TOKEN_STRING){
        stringClear(&operand1);
        stringAddChars(&operand1, TYPESTRING);
        stringAddChars(&operand1, temp_operand1.value);
    }
    if(temp_operand1_is_label == true) {
        stringClear(&operand1);
        stringAddChars(&operand1, LABELSYMBOL);
        stringAddChars(&operand1, temp_operand1.value);
    }
    /*
    if (temp_operand1_isScope == true) {
        stringClear(&operand1);
        stringAddChars(&operand1, SCOPE);
    }
    */

    string operand2;
    stringInit(&operand2);
    string tmp_operand2;
    stringInit(&tmp_operand2);
    stringAddChars(&tmp_operand2, second.value.value);

    int tmp_operand2_type = second.type;
    bool tmp_operand2_is_temp = second.is_temp;
    bool tmp_operand2_is_label = second.is_label;
    frame tmp_operand2_frame = second.frame;
    //INPUTTYPE tmp_operand2_inputtype = second.inputType;
    if(tmp_operand2_type == TOKEN_ID)
        if (tmp_operand2_is_temp == true) {
            stringClear(&operand2);
            stringAddChars(&operand2, TMP);
        }
        else if (tmp_operand2_frame == LF) {
            stringClear(&operand2);
            stringAddChars(&operand2, FRAMELF);
            stringAddChars(&operand2, tmp_operand2.value);
        }
        else if (tmp_operand2_frame == GF) {
            stringClear(&operand2);
            stringAddChars(&operand2, FRAMEGF);
            stringAddChars(&operand2, tmp_operand2.value);
        }
        else{
            stringClear(&operand2);
            stringAddChars(&operand2, FRAMETF);
            stringAddChars(&operand2, tmp_operand2.value);
        }
    else if(tmp_operand2_type == TOKEN_INT) {
        stringClear(&operand2);
        stringAddChars(&operand2, TYPEINT);
        stringAddChars(&operand2, tmp_operand2.value);
    }
    else if(tmp_operand2_type == TOKEN_DOUBLE){
        stringClear(&operand2);
        stringAddChars(&operand2, TYPEDOUBLE);
        stringAddChars(&operand2, tmp_operand2.value);
    }
    else if(tmp_operand2_type == TOKEN_STRING){
        stringClear(&operand2);
        stringAddChars(&operand2, TYPESTRING);
        stringAddChars(&operand2, tmp_operand2.value);
    }
    if(tmp_operand2_is_label == true) {
        stringClear(&operand2);
        stringAddChars(&operand2, LABELSYMBOL);
        stringAddChars(&operand2, tmp_operand2.value);
    }
    /*
    if (tmp_operand2_inputtype == INPUT_INT) {
        stringClear(&operand2);
        stringAddChars(&operand2, "int");
    } else if (tmp_operand2_inputtype == INPUT_DOUBLE) {
        stringClear(&operand2);
        stringAddChars(&operand2, "float");
    } else if (tmp_operand2_inputtype == INPUT_STRING) {
        stringClear(&operand2);
        stringAddChars(&operand2, "string");
    }
    */
    addInstruction(L, instType, operand1.value, operand2.value, NULL);
}

void threeOperandInstr(instruction_list *L, int instType, operand first, operand second, operand third){
    string operand1;
    stringInit(&operand1);
    string temp_operand1;
    stringInit(&temp_operand1);
    stringAddChars(&temp_operand1, first.value.value);

    int temp_operand1_type = first.type;
    bool temp_operand1_is_temp = first.is_temp;
    //bool temp_operand1_isScope = first.isScope;
    bool temp_operand1_is_label = first.is_label;
    frame temp_operand1_frame = first.frame;
    if(temp_operand1_type == TOKEN_ID) {
        if (temp_operand1_is_temp == true) {
            stringClear(&operand1);
            stringAddChars(&operand1, TMP);
        }
        else if (temp_operand1_frame == LF) {
            stringClear(&operand1);
            stringAddChars(&operand1, FRAMELF);
            stringAddChars(&operand1, temp_operand1.value);

        }
        else if (temp_operand1_frame == GF) {
            stringClear(&operand1);
            stringAddChars(&operand1, FRAMEGF);
            stringAddChars(&operand1, temp_operand1.value);

        }
        else{
            stringClear(&operand1);
            stringAddChars(&operand1, FRAMETF);
            stringAddChars(&operand1, temp_operand1.value);
        }
    }
    else if(temp_operand1_type == TOKEN_INT){
        stringClear(&operand1);
        stringAddChars(&operand1, TYPEINT);
        stringAddChars(&operand1, temp_operand1.value);
    }
    else if(temp_operand1_type == TOKEN_DOUBLE){
        stringClear(&operand1);
        stringAddChars(&operand1, TYPEDOUBLE);
        stringAddChars(&operand1, temp_operand1.value);
    }
    else if(temp_operand1_type == TOKEN_STRING){
        stringClear(&operand1);
        stringAddChars(&operand1, TYPESTRING);
        stringAddChars(&operand1, temp_operand1.value);
    }
    if(temp_operand1_is_label == true) {
        stringClear(&operand1);
        stringAddChars(&operand1, LABELSYMBOL);
        stringAddChars(&operand1, temp_operand1.value);
    }
    /*
    if (temp_operand1_isScope == true) {
        stringClear(&operand1);
        stringAddChars(&operand1, SCOPE);
    }
    */


    string operand2;
    stringInit(&operand2);
    string tmp_operand2;
    stringInit(&tmp_operand2);
    stringAddChars(&tmp_operand2, second.value.value);

    int tmp_operand2_type = second.type;
    bool tmp_operand2_is_temp = second.is_temp;
    bool tmp_operand2_is_label = second.is_label;
    frame tmp_operand2_frame = second.frame;
    //INPUTTYPE tmp_operand2_inputtype = second.inputType;
    if(tmp_operand2_type == TOKEN_ID)
        if (tmp_operand2_is_temp == true) {
            stringClear(&operand2);
            stringAddChars(&operand2, TMP);
        }
        else if (tmp_operand2_frame == LF) {
            stringClear(&operand2);
            stringAddChars(&operand2, FRAMELF);
            stringAddChars(&operand2, tmp_operand2.value);
        }
        else if (tmp_operand2_frame == GF) {
            stringClear(&operand2);
            stringAddChars(&operand2, FRAMEGF);
            stringAddChars(&operand2, tmp_operand2.value);
        }
        else{
            stringClear(&operand2);
            stringAddChars(&operand2, FRAMETF);
            stringAddChars(&operand2, tmp_operand2.value);
        }
    else if(tmp_operand2_type == TOKEN_INT) {
        stringClear(&operand2);
        stringAddChars(&operand2, TYPEINT);
        stringAddChars(&operand2, tmp_operand2.value);
    }
    else if(tmp_operand2_type == TOKEN_DOUBLE){
        stringClear(&operand2);
        stringAddChars(&operand2, TYPEDOUBLE);
        stringAddChars(&operand2, tmp_operand2.value);
    }
    else if(tmp_operand2_type == TOKEN_STRING){
        stringClear(&operand2);
        stringAddChars(&operand2, TYPESTRING);
        stringAddChars(&operand2, tmp_operand2.value);
    }
    if(tmp_operand2_is_label == true) {
        stringClear(&operand2);
        stringAddChars(&operand2, LABELSYMBOL);
        stringAddChars(&operand2, tmp_operand2.value);
    }
    /*
    // Operand je typ vstupní hodnoty
    if (tmp_operand2_inputtype == INPUT_INT) {
        stringClear(&operand2);
        stringAddChars(&operand2, "int");
    } else if (tmp_operand2_inputtype == INPUT_DOUBLE) {
        stringClear(&operand2);
        stringAddChars(&operand2, "float");
    } else if (tmp_operand2_inputtype == INPUT_STRING) {
        stringClear(&operand2);
        stringAddChars(&operand2, "string");
    }
*/
    string operand3;
    stringInit(&operand3);
    string tmp_operand3;
    stringInit(&tmp_operand3);
    stringAddChars(&tmp_operand3, third.value.value);

    int tmp_operand3_type = third.type;
    bool tmp_operand3_is_temp = third.is_temp;
    bool tmp_operand3_is_label = third.is_label;
    frame tmp_operand3_frame = third.frame;
    if(tmp_operand3_type == TOKEN_ID)
        if (tmp_operand3_is_temp == true) {
            stringClear(&operand3);
            stringAddChars(&operand3, TMP);
        }
        else if (tmp_operand3_frame == LF) {
            stringClear(&operand3);
            stringAddChars(&operand3, FRAMELF);
            stringAddChars(&operand3, tmp_operand3.value);
        }else if (tmp_operand3_frame == GF) {
            stringClear(&operand3);
            stringAddChars(&operand3, FRAMEGF);
            stringAddChars(&operand3, tmp_operand3.value);
        }
        else {
            stringClear(&operand3);
            stringAddChars(&operand3, FRAMETF);
            stringAddChars(&operand3, tmp_operand3.value);
        }
    else if(tmp_operand3_type == 42){
        stringClear(&operand3);
        stringAddChars(&operand3, TYPEBOOL);
        stringAddChars(&operand3, tmp_operand3.value);
    }
    else if(tmp_operand3_type == TOKEN_INT) {
        stringClear(&operand3);
        stringAddChars(&operand3, TYPEINT);
        stringAddChars(&operand3, tmp_operand3.value);
    }
    else if(tmp_operand3_type == TOKEN_DOUBLE){
        stringClear(&operand3);
        stringAddChars(&operand3, TYPEDOUBLE);
        stringAddChars(&operand3, tmp_operand3.value);
    }
    else if(tmp_operand3_type == TOKEN_STRING){
        stringClear(&operand3);
        stringAddChars(&operand3, TYPESTRING);
        stringAddChars(&operand3, tmp_operand3.value);
    }
    if(tmp_operand3_is_label == true) {
        stringClear(&operand3);
        stringAddChars(&operand3, LABELSYMBOL);
        stringAddChars(&operand3, tmp_operand3.value);
    }
    addInstruction(L, instType, operand1.value, operand2.value, operand3.value);
}


void addInstruction(struct instruction_list *List, int instType, void *adress1, void *adress2, void *adress3) {
    instruction instruction;
    instruction.instructionType = instType;
    instruction.adress1 = adress1;
    instruction.adress2 = adress2;
    instruction.adress3 = adress3;
    // generovani instrukce
    DLInsertLast(List, instruction);
}


void printInstructionList(instruction_list *List) {
    instruction tempInst; // docasne ulozeni instrukce
    DLFirst(List); // aktivita je na prvni instruci
    while ( DLActive(List) ) {
        DLCopy(List, &tempInst);
        // vypsani typu instrukce
        switch (tempInst.instructionType) {
            case START:           printf(".IFJcode19\n");        break;
            case COMMENT:         printf("# %s\n",               (char*)tempInst.adress1);       break;
            case MOVE:            printf("MOVE %s %s\n",         (char*)tempInst.adress1,        (char*)tempInst.adress2);       break;
            case CREATEFRAME:     printf("CREATEFRAME\n");       break;
            case PUSHFRAME:       printf("PUSHFRAME\n");         break;
            case POPFRAME:        printf("POPFRAME\n");          break;
            case DEFVAR:          printf("DEFVAR %s\n",          (char*)tempInst.adress1);       break;
            case CALL:            printf("CALL %s\n",            (char*) tempInst.adress1);      break;
            case RETURN:          printf("RETURN\n");            break;
            case PUSHS:           printf("PUSHS %s\n",           (char*)tempInst.adress1);       break;
            case POPS:            printf("POPS %s\n",            (char*)tempInst.adress1);       break;
            case CLEARS:          printf("CLEARS\n");            break;
            case ADD:             printf("ADD %s %s %s\n",       (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case SUB:             printf("SUB %s %s %s\n",       (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case MUL:             printf("MUL %s %s %s\n",       (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case DIV:             printf("DIV %s %s %s\n",       (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case IDIV:            printf("IDIV %s %s %s\n",      (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case ADDS:            printf("ADDS\n");              break;
            case SUBS:            printf("SUBS\n");              break;
            case MULS:            printf("MULS\n");              break;
            case DIVS:            printf("DIVS\n");              break;
            case IDIVS:           printf("IDIVS\n");             break;
            case LT:              printf("LT %s %s %s\n",        (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case GT:              printf("GT %s %s %s\n",        (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case EQ:              printf("EQ %s %s %s\n",        (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case LTS:             printf("LTS\n");               break;
            case GTS:             printf("GTS\n");               break;
            case EQS:             printf("EQS\n");               break;
            case AND:             printf("AND %s %s %s\n",       (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case OR:              printf("OR %s %s %s\n",        (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case NOT:             printf("NOT %s %s %s\n",       (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case ANDS:            printf("ANDS\n");              break;
            case ORS:             printf("ORS\n");               break;
            case NOTS:            printf("NOTS \n"  );           break;
            case INT2FLOAT:       printf("INT2FLOAT %s %s\n",    (char*)tempInst.adress1,        (char*)tempInst.adress2);       break;
            case FLOAT2INT:       printf("FLOAT2INT %s %s\n",    (char*)tempInst.adress1,        (char*)tempInst.adress2);       break;
            case INT2CHAR:        printf("INT2CHAR %s %s\n",     (char*)tempInst.adress1,        (char*)tempInst.adress2);       break;
            case STRI2INT:        printf("STRI2INT %s %s %s\n",  (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case INT2FLOATS:      printf("INT2FLOATS\n");        break;
            case FLOAT2INTS:      printf("FLOAT2INTS\n");        break;
            case INT2CHARS:       printf("INT2CHARS\n");         break;
            case STRI2INTS:       printf("STRI2INTS\n");         break;
            case READ:            printf("READ %s %s\n",         (char*)tempInst.adress1,        (char*)tempInst.adress2);       break;
            case WRITE:           printf("WRITE %s\n",           (char*)tempInst.adress1);       break;
            case CONCAT:          printf("CONCAT %s %s %s\n",    (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case STRLEN:          printf("STRLEN %s %s\n",       (char*)tempInst.adress1,        (char*)tempInst.adress2);       break;
            case GETCHAR:         printf("GETCHAR %s %s %s\n",   (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case SETCHAR:         printf("SETCHAR %s %s %s\n",   (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case TYPE:            printf("TYPE %s %s\n",         (char*)tempInst.adress1,        (char*)tempInst.adress2);       break;
            case LABEL:           printf("LABEL %s\n",           (char*)tempInst.adress1);       break;
            case JUMP:            printf("JUMP %s\n",            (char*)tempInst.adress1);       break;
            case JUMPIFEQ:        printf("JUMPIFEQ %s %s %s\n",  (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case JUMPIFNEQ:       printf("JUMPIFNEQ %s %s %s\n", (char*)tempInst.adress1,        (char*)tempInst.adress2,        (char*)tempInst.adress3);        break;
            case JUMPIFEQS:       printf("JUMPIFEQS %s\n",       (char*)tempInst.adress1);       break;
            case JUMPIFNEQS:      printf("JUMPIFNEQS %s\n",      (char*)tempInst.adress1);       break;
            case BREAK:           printf("BREAK\n");             break;
            case DPRINT:          printf("DPRINT %s\n",          (char*)tempInst.adress1);       break;
            case EXIT:            printf("EXIT\n");              break;
            //case QUESTIONMARK:  printf("?");                   break;
        }
        // vypsani operandu instrukce (delete pak)
        //printf(" %p %p %p\n", tempInst.adress1, tempInst.adress2, tempInst.adress3); // vypise adresy operandu instrukce

        DLSucc(List);
    }
}

void inputSFunction() {
    operand1 = initOperand(operand1, "inputs", TOKEN_ID, GF, false, true);
    oneOperandInstr(&instrList, LABEL, operand1);

    noOperandInstr(&instrList, PUSHFRAME);
    
    addInstruction(&instrList, READ, "GF@tmp", "string", NULL);

    operand1 = initOperand(operand1, "", TOKEN_ID, GF, true, false);
    oneOperandInstr(&instrList, PUSHS, operand1);

    noOperandInstr(&instrList, RETURN);
}

void inputIFunction() {
    operand1 = initOperand(operand1, "inputi", TOKEN_ID, GF, false, true);
    oneOperandInstr(&instrList, LABEL, operand1);

    noOperandInstr(&instrList, PUSHFRAME);
    
    addInstruction(&instrList, READ, "GF@tmp", "int", NULL);

    operand1 = initOperand(operand1, "", TOKEN_ID, GF, true, false);
    oneOperandInstr(&instrList, PUSHS, operand1);

    noOperandInstr(&instrList, RETURN);
}

void inputFFunction() {
    operand1 = initOperand(operand1, "inputf", TOKEN_ID, GF, false, true);
    oneOperandInstr(&instrList, LABEL, operand1);

    noOperandInstr(&instrList, PUSHFRAME);
    
    addInstruction(&instrList, READ, "GF@tmp", "float", NULL);

    operand1 = initOperand(operand1, "", TOKEN_ID, GF, true, false);
    oneOperandInstr(&instrList, PUSHS, operand1);

    noOperandInstr(&instrList, RETURN);
}

void lenFunction() { //params: s
    //label

    operand1 = initOperand(operand1, "len", TOKEN_ID, GF, false, true);
    oneOperandInstr(&instrList, LABEL, operand1);

    //pushframe
    noOperandInstr(&instrList, PUSHFRAME);



    //make it do sth (goes to tmp)
    operand1 = initOperand(operand1, "", TOKEN_ID, GF, true, false);
    operand2 = initOperand(operand2, "%1", TOKEN_ID, LF, false, false);
    twoOperandInstr(&instrList, STRLEN, operand1, operand2);

    oneOperandInstr(&instrList, PUSHS, operand1);

    //popping from expression

    noOperandInstr(&instrList, RETURN);
}

void ordFunction() { //params: s, i
    //label

    operand1 = initOperand(operand1, "ord", TOKEN_ID, GF, false, true);
    oneOperandInstr(&instrList, LABEL, operand1);

    //pushframe
    noOperandInstr(&instrList, PUSHFRAME);

    //sub a 1 (bcs string goes from 0) to strlen-1
    operand1 = initOperand(operand1, "%2", TOKEN_ID, LF, false, false);
    operand2 = initOperand(operand2, "1", TOKEN_INT, LF, false, false);
    threeOperandInstr(&instrList, SUB, operand1, operand1, operand2);

    //result is in global1 var
    operand1 = initOperand(operand1, "", TOKEN_ID, GF, true, false);
    operand2 = initOperand(operand2, "%1", TOKEN_ID, LF, false, false);
    operand3 = initOperand(operand3, "%2", TOKEN_ID, LF, false, false);
    threeOperandInstr(&instrList, STRI2INT, operand1, operand2, operand3);

    oneOperandInstr(&instrList, PUSHS, operand1);

    noOperandInstr(&instrList, RETURN);
}

void chrFunction() { //params: i
    //label
    operand1 = initOperand(operand1, "chr", TOKEN_ID, GF, false, true);
    oneOperandInstr(&instrList, LABEL, operand1);

    //pushframe
    noOperandInstr(&instrList, PUSHFRAME);

    operand1 = initOperand(operand1, "", TOKEN_ID, GF, true, false);
    operand2 = initOperand(operand2, "%1", TOKEN_ID, LF, false, false);
    twoOperandInstr(&instrList, INT2CHAR, operand1, operand2);

    oneOperandInstr(&instrList, PUSHS, operand1);

    noOperandInstr(&instrList, RETURN);
}

void printFunction() { //params: term1 term2 term3 ....
    //label
    operand1 = initOperand(operand1, "print", TOKEN_ID, GF, false, true);
    oneOperandInstr(&instrList, LABEL, operand1);

    operand1 = initOperand(operand1, "", TOKEN_ID, GF, true, false);
    oneOperandInstr(&instrList, POPS, operand1);

    operand1 = initOperand(operand1, "tmp1", TOKEN_ID, GF, false, false);
    operand2 = initOperand(operand2, "", TOKEN_ID, GF, true, false);
    twoOperandInstr(&instrList, TYPE, operand1, operand2);
    
    operand1 = initOperand(operand1, "fornumber", TOKEN_ID, GF, false, true);
    operand2 = initOperand(operand2, "tmp1", TOKEN_ID, GF, false, false);
    operand3 = initOperand(operand3, "int", TOKEN_STRING, GF, false, false);
    threeOperandInstr(&instrList, JUMPIFEQ, operand1, operand2, operand3);

    operand1 = initOperand(operand1, "fornumber", TOKEN_ID, GF, false, true);
    operand2 = initOperand(operand2, "tmp1", TOKEN_ID, GF, false, false);
    operand3 = initOperand(operand3, "float", TOKEN_STRING, GF, false, false);
    threeOperandInstr(&instrList, JUMPIFEQ, operand1, operand2, operand3);


    operand2 = initOperand(operand2, "%endwhile", TOKEN_STRING, GF, false, false);
    operand3 = initOperand(operand3, "tmp1", TOKEN_ID, GF, false, false);
    twoOperandInstr(&instrList, MOVE, operand3, operand2);

    //when %endwhile is on the stack, time to end
    operand1 = initOperand(operand1, "", TOKEN_ID, GF, true, false);
    operand3 = initOperand(operand3, "endwhile", TOKEN_ID, GF, false, true);
    operand2 = initOperand(operand2, "tmp1", TOKEN_ID, GF, false, false);
    threeOperandInstr(&instrList, JUMPIFEQ, operand3, operand1, operand2);

    operand1 = initOperand(operand1, "continue_write", TOKEN_ID, GF, false, true);
    operand2 = initOperand(operand2, "tmp2", TOKEN_ID, GF, false, false);
    operand3 = initOperand(operand3, "1", TOKEN_INT, GF, false, false);
    threeOperandInstr(&instrList, JUMPIFNEQ, operand1, operand2, operand3);

    operand1 = initOperand(operand1, "\\032", TOKEN_STRING, GF, false, false);
    oneOperandInstr(&instrList, WRITE, operand1);

    operand1 = initOperand(operand1, "continue_write", TOKEN_ID, GF, false, true);
    oneOperandInstr(&instrList, LABEL, operand1);

    //print string
    operand1 = initOperand(operand1, "", TOKEN_ID, GF, true, false);
    oneOperandInstr(&instrList, WRITE, operand1);


    operand1 = initOperand(operand1, "tmp2", TOKEN_ID, GF, false, false);
    operand2 = initOperand(operand2, "1", TOKEN_INT, GF, false, false);
    twoOperandInstr(&instrList, MOVE, operand1, operand2); 

    operand1 = initOperand(operand1, "print", TOKEN_ID, GF, false, true);
    oneOperandInstr(&instrList, JUMP, operand1);


    operand1 = initOperand(operand1, "fornumber", TOKEN_ID, GF, false, true);
    oneOperandInstr(&instrList, LABEL, operand1);

    operand1 = initOperand(operand1, "continue_writee", TOKEN_ID, GF, false, true);
    operand2 = initOperand(operand2, "tmp2", TOKEN_ID, GF, false, false);
    operand3 = initOperand(operand3, "1", TOKEN_INT, GF, false, false);
    threeOperandInstr(&instrList, JUMPIFNEQ, operand1, operand2, operand3);

    operand1 = initOperand(operand1, "\\032", TOKEN_STRING, GF, false, false);
    oneOperandInstr(&instrList, WRITE, operand1);

    operand1 = initOperand(operand1, "continue_writee", TOKEN_ID, GF, false, true);
    oneOperandInstr(&instrList, LABEL, operand1);

    operand1 = initOperand(operand1, "", TOKEN_ID, GF, true, false);
    oneOperandInstr(&instrList, WRITE, operand1);

    operand1 = initOperand(operand1, "tmp2", TOKEN_ID, GF, false, false);
    operand2 = initOperand(operand2, "1", TOKEN_INT, GF, false, false);
    twoOperandInstr(&instrList, MOVE, operand1, operand2); 

    operand1 = initOperand(operand1, "print", TOKEN_ID, GF, false, true);
    oneOperandInstr(&instrList, JUMP, operand1);

    operand1 = initOperand(operand1, "endwhile", TOKEN_STRING, GF, false, true);
    oneOperandInstr(&instrList, LABEL, operand1);

    operand1 = initOperand(operand1, "\\010", TOKEN_STRING, GF, false, false);
    oneOperandInstr(&instrList, WRITE, operand1);

    noOperandInstr(&instrList, RETURN);
}