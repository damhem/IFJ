#include "expression.h"

Token token; //Převzatý token od scanneru

const char precedenceTable[PT_SIZE][PT_SIZE] = {
//           *     /     //    +     -     =    !=     <    <=     >    >=    ==     (     )   STR    INT   DOUB   ID    F     ,     $
/*  *  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '_' , '>' },
/*  /  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '_' , '>' },
/* //  */ { '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '_' , '>' },
/*  +  */ { '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '_' , '>' },
/*  -  */ { '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '_' , '>' },
/*  =  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '_' , '>' },
/*  != */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '_' , '>' },
/*  <  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '_' , '>' },
/*  <= */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '_' , '>' },
/*  >  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '_' , '>' },
/*  >= */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '_' , '>' },
/*  == */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '_' , '>' },
/*  (  */ { '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , '<' , '<' , '<' , '<' , '_' , '<' , '_' },
/*  )  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '_' , '_' , '_' , '_' , '_' , '>' },
/* STR */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '_' , '_' , '_' , '_' , '>' , '>' },
/* INT */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '_' , '_' , '_' , '_' , '>' , '>' },
/* DOUB*/ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '_' , '_' , '_' , '_' , '>' , '>' },
/*  ID */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '_' , '_' , '_' , '_' , '>' , '>' },
/*  F  */ { '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' },
/*  ,  */ { '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '<' , '<' , '<' , '<' , '_' , '<' , '_' },
/*  $  */ { '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '_' , '<' , '<' , '<' , '<' , '<' , '_' , '$' },
};


//hlavní funkce
int expression(VarType* returnValue) {/*,int expectedValue*/
    ERROR_CODE result;
    

    exp_stackInit(&stack_expression);

    result = expressionAnalysis();

    exp_stackClear(&stack_expression);

    *returnValue = retVal;
    
    return result;
}
//Analýza daného výrazu
int expressionAnalysis() {
    ERROR_CODE result;
    char sign = '_';
    while(true) {
        sign = getSignFromTable();
        if(sign == '='){
            exp_stackPush(&stack_expression,tokentoExp_element(token,false));
            result =  reducePars(&stack_expression); 
            if (result != ERROR_CODE_OK) {
            return result;
            }
            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
        }
        else if(sign == '<'){
            token_type next = peekNextToken();
            if (token.t_type == TOKEN_ID && next == TOKEN_LEFTPAR) {
                result = makeFunction();
                if (result != ERROR_CODE_OK) return result;
                return ERROR_CODE_OK;
            }


            if (token.t_type == TOKEN_ID || token.t_type == TOKEN_INT || token.t_type == TOKEN_DOUBLE || token.t_type == TOKEN_STRING){
                exp_stackPush(&stack_expression,tokentoExp_element(token,true));
            }
            else{
                exp_stackPush(&stack_expression,tokentoExp_element(token,false));
            }
            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
            firstTerm = stack_expression.top_of_stack;
        } 
        else if(sign == '>') {
            result = useRule(&stack_expression);
            if (result != ERROR_CODE_OK) {
            return result;
            }
        } 
        else if(sign == '$') {
            return 0;
        } 
        else if(sign == '_' && token.t_type == TOKEN_LEFTPAR) {
            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //leftpar
            result = makeFunction();
            if (result != ERROR_CODE_OK) return result;
            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //rightpar                return ERROR_CODE_OK;
        }    
        else{
            return 2;
        }
    }
}
// Funkce pro hledání znaku z precedencni tabulky
char getSignFromTable(){
    int a;
    int b;
    if (token.t_type == TOKEN_EOL || token.t_type == TOKEN_EOF || token.t_type == TOKEN_DOUBLEDOT) {
        b = TOKEN_UNDEF;
    }
    else {
        b = token.t_type;
    }
    a = get_stack_type(&stack_expression);
    //printf("%d %d \n", a, b);
    //printf("%c \n", precedenceTable[a][b]);
    return precedenceTable[a][b];
}

//Funkce inicializující nový element
Exp_element *newElement(int type,bool handle){
    Exp_element *new_element = malloc(sizeof(struct exp_element));
    //Inicializace noveho elementu
    if(new_element != NULL){
        if(type == TOKEN_INT){
        new_element->e_data.integer = token.t_data.integer;
        }
        if(type == TOKEN_DOUBLE){
        new_element->e_data.decimal = token.t_data.decimal;
        }
        if(type == TOKEN_STRING){
        new_element->e_data.ID = token.t_data.ID;
        }
        if (type == TOKEN_ID) {
        
        stringInit(&(new_element->e_data.ID));
        stringAddChars(&(new_element->e_data.ID), token.t_data.ID.value);
        }
        new_element->type = type;
        new_element->handle = handle;
        new_element->terminal = true;
        return new_element;
    }
    else return NULL;
}

//Funkce ziskavající typ prvního terminalu na stacku
int get_stack_type(ptrStack *stack_expression){

    if (stack_expression->top_of_stack != NULL){
        if (((Exp_element*)stack_expression->top_of_stack->value)->terminal == true){
        return ((Exp_element*)stack_expression->top_of_stack->value)->type;
        }
    }

    if (stack_expression->top_of_stack->left != NULL){
        if (((Exp_element*)stack_expression->top_of_stack->left->value)->terminal == true){
        return ((Exp_element*)stack_expression->top_of_stack->left->value)->type;
        }
    }
    
    if (stack_expression->top_of_stack->left->left != NULL){
        if (((Exp_element*)stack_expression->top_of_stack->left->left->value)->terminal == true){
        return ((Exp_element*)stack_expression->top_of_stack->left->left->value)->type;
        }
        
    }
    return ERROR_CODE_OK;
}

// Funkce prevadejici token na element
Exp_element *tokentoExp_element(Token token,bool handle){
    int type;
    if (token.t_type == TOKEN_EOL || token.t_type == TOKEN_EOF || token.t_type == TOKEN_DOUBLEDOT) {
        type = TOKEN_UNDEF;
    }
    type = token.t_type;
    Exp_element *element_to_stack = newElement(type, handle);
    return element_to_stack;
}

//Funkce resici redukce vyrazu
ERROR_CODE useRule(ptrStack *stack_expression){
    ERROR_CODE result;

    switch(firstTerm->value->type) {

        case TOKEN_INT :
        case TOKEN_DOUBLE:
        case TOKEN_ID:
        case TOKEN_STRING:
            ((Exp_element*)stack_expression->top_of_stack->value)->terminal = false;
            ((Exp_element *)stack_expression->top_of_stack->value)->handle = false;
            ((Exp_element *)stack_expression->top_of_stack->left->value)->handle = false;
            firstTerm = stack_expression->top_of_stack->left;

            //todo function term?
            

            //integer value
            if (stack_expression->top_of_stack->value->type == TOKEN_INT) {

                string operandString;
                stringInit(&operandString);

                char number[10];
                sprintf(number, "%d", stack_expression->top_of_stack->value->e_data.integer);
                //printf("number: %s\n", number);

                stringAddChars(&operandString, number);
                operand operand = initOperand(operand, operandString.value, stack_expression->top_of_stack->value->type, LF, false, false);
                retVal = typeinteger;
                oneOperandInstr(&instrList, PUSHS, operand);
            }
            //double value
            else if (stack_expression->top_of_stack->value->type == TOKEN_DOUBLE) {
                string operandString;
                stringInit(&operandString);

                char number[10];
                sprintf(number, "%f", stack_expression->top_of_stack->value->e_data.decimal);
               // printf("number: %s\n", number);

                stringAddChars(&operandString, number);
                operand operand = initOperand(operand, operandString.value, stack_expression->top_of_stack->value->type, LF, false, false);
                retVal = typedouble;
                oneOperandInstr(&instrList, PUSHS, operand);
            }
            //string value
            else if (stack_expression->top_of_stack->value->type == TOKEN_STRING) {
                operand operand = initOperand(operand, stack_expression->top_of_stack->value->e_data.ID.value, stack_expression->top_of_stack->value->type, LF, false, false);
                retVal = typestring;
                oneOperandInstr(&instrList, PUSHS, operand);
            }
            //there has to be ID analysis
            else if (stack_expression->top_of_stack->value->type == TOKEN_ID) {
                printf("yo");
                
                result = makeIdInstr();

                if (result != ERROR_CODE_OK) return result;
                //todo ID analysisi
                
            }

            return ERROR_CODE_OK;

        case TOKEN_ADDITION :
            if(stack_expression->top_of_stack->value->type == TOKEN_STRING) { //todo kdyz bude id string
                string operandString;
                stringInit(&operandString);
                operand operand1 = initOperand(operand1,operandString.value,TOKEN_ID,LF, true, false);
                operand operand2 = initOperand(operand2,stack_expression->top_of_stack->value->e_data.ID.value, stack_expression->top_of_stack->value->type, LF, false, false);
                operand operand3 = initOperand(operand3,stack_expression->top_of_stack->left->left->value->e_data.ID.value, stack_expression->top_of_stack->left->left->value->type, LF, false, false);
                threeOperandInstr(&instrList, CONCAT, operand1, operand2, operand3);
                oneOperandInstr(&instrList, PUSHS, operand1);
                break;
            }
            noOperandInstr(&instrList, ADDS);
            break;

        case TOKEN_MULTIPLICATION :
            noOperandInstr(&instrList, MULS);
            break;

        case TOKEN_SUBTRACTION:
            noOperandInstr(&instrList, SUBS);
            break;

        case TOKEN_DIVISION:
            noOperandInstr(&instrList, DIVS);
            break;

        case TOKEN_INTEGER_DIVISION:
            noOperandInstr(&instrList, DIVS);
            noOperandInstr(&instrList, FLOAT2INTS);
            break;
        case TOKEN_EQUAL_EQUAL:
            noOperandInstr(&instrList, EQS);
            break;
        case TOKEN_NEG_EQUAL:
            noOperandInstr(&instrList, EQS);
            noOperandInstr(&instrList, NOTS);
            break;
        case TOKEN_SMALLERTHEN:
            noOperandInstr(&instrList, LTS);
            break;
        case TOKEN_BIGGERTHEN:
            noOperandInstr(&instrList, GTS);
            break;
        case TOKEN_SMALLERTHEN_EQUAL:
            noOperandInstr(&instrList, GTS);
            noOperandInstr(&instrList, NOTS);
            break;
        case TOKEN_BIGGERTHEN_EQUAL:
            noOperandInstr(&instrList, LTS);
            noOperandInstr(&instrList, NOTS);
            break;

        default:
            return ERROR_CODE_SEM;
    }

    exp_stackPop(stack_expression);
    exp_stackPop(stack_expression);

    if (stack_expression->top_of_stack != NULL) {
        firstTerm = stack_expression->top_of_stack->left;
    }

    return ERROR_CODE_OK;
}


ERROR_CODE reducePars(ptrStack *stack_expression){
    if(stack_expression != NULL){
        ptStack* quicksave = stack_expression->top_of_stack->left;

        exp_stackPop(stack_expression);
        exp_stackPop(stack_expression);
        exp_stackPop(stack_expression);
        
        exp_stackPush(stack_expression,quicksave);
        if((stack_expression->top_of_stack->left->value)->terminal == true) {
            
            firstTerm = stack_expression->top_of_stack->left;
            (firstTerm->value)->handle = false;
            
        }
        else return ERROR_CODE_SYN;
    }
    else return ERROR_CODE_SYN;
    return ERROR_CODE_OK;
}

//Vyhodnocovani ID
ERROR_CODE makeIdInstr() {
    //todo function

    

    token_type next = peekNextToken();
    if (next == TOKEN_LEFTPAR) {
        printf("yo");
        if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //leftpar
        makeFunction();
        return ERROR_CODE_OK;
    }

    tBSTNodePtr helper = SYMSearch(&glSymtable,stack_expression.top_of_stack->value->e_data.ID);
    if (helper != NULL) {
        switch (helper->Vartype) {
        case typeinteger:;
            operand operand1 = initOperand(operand1, helper->Key.value, TOKEN_ID, GF, false, false);
            retVal = typeinteger;
            oneOperandInstr(&instrList, PUSHS, operand1);
            break;
        case typedouble:
            retVal = typedouble;
            operand operand2 = initOperand(operand2, helper->Key.value, TOKEN_ID, GF, false, false);
            oneOperandInstr(&instrList, PUSHS, operand2);
            break;
        case typestring:
            retVal = typestring;
            operand operand3 = initOperand(operand3, helper->Key.value, TOKEN_ID, GF, false, false);
            oneOperandInstr(&instrList, PUSHS, operand3);
            break;
        case undefined:
            fprintf(stderr, "Promenna nebyla definovana (%s)\n", helper->Key.value);
            return ERROR_CODE_SEM;
            break;
        default:
            //not initialized vartype -> err
            return ERROR_CODE_INTERNAL;
        }
    }
    else {
        //promenna se nenasla v tabulce
        return ERROR_CODE_SEM;
    }
    return ERROR_CODE_OK;
}


ERROR_CODE makeFunction() {
    ERROR_CODE result;
    

    token_type next = peekNextToken();

    if (token.t_type == TOKEN_ID && next == TOKEN_LEFTPAR) {
        //starting function
        //have to check if its in symtable

        if (token.t_type == TOKEN_ID && next == TOKEN_LEFTPAR && strcmp(token.t_data.ID.value, "print") == 0) {
            return makePrintFunction();
        }

        tBSTNodePtr helper = SYMSearch(&glSymtable, token.t_data.ID);
        if (helper == NULL) return ERROR_CODE_SEM_OTHER;
        
        
        
    }

    




    switch (token.t_type) {
    case TOKEN_ID:
      //todo sth with symtable
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = nextTerms();
      if (result != ERROR_CODE_OK) return result;

      if (token.t_type != TOKEN_RIGHTPAR) return ERROR_CODE_SYN;

      return ERROR_CODE_OK;

    case TOKEN_STRING:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_NONE:

      //todo sth
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = nextTerms();
      if (result != ERROR_CODE_OK) return result;

      if (token.t_type != TOKEN_RIGHTPAR) return ERROR_CODE_SYN;

      return ERROR_CODE_OK;
    case TOKEN_RIGHTPAR:
      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_SYN;
}
}


ERROR_CODE nextTerms() {
  
  switch (token.t_type) {
    case TOKEN_COMMA:
      //Next_term -> , Termy
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      return makeFunction();

    case TOKEN_RIGHTPAR:
      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}



ERROR_CODE makePrintFunction() {
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //now leftpar
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //first term
    
    operand1 = initOperand(operand1, "tmp2", TOKEN_ID, GF, false, false);
    operand2 = initOperand(operand2, "2", TOKEN_INT, GF, false, false);
    twoOperandInstr(&instrList, MOVE, operand1, operand2); 

    operand1 = initOperand(operand1, "%endwhile", TOKEN_STRING, GF, false, false);
    oneOperandInstr(&instrList, PUSHS, operand1);

    ptrStack myStack;
    exp_stackInit(&myStack);
    exp_stackPop(&myStack);

    while (token.t_type != TOKEN_RIGHTPAR ) {
        if (token.t_type == TOKEN_STRING || token.t_type == TOKEN_ID || token.t_type == TOKEN_INT || token.t_type == TOKEN_DOUBLE || token.t_type == TOKEN_NONE) {
            exp_stackPush(&myStack, tokentoExp_element(token, false));
        }
        if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
        if (token.t_type == TOKEN_COMMA) {
            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
        }
        else if (token.t_type == TOKEN_RIGHTPAR) continue;
        else {
            return ERROR_CODE_SEM_OTHER;
        }
    }


    while (exp_stackEmpty(&myStack) != 1) {
        switch(myStack.top_of_stack->value->type) {
            case TOKEN_STRING:
                operand1 = initOperand(operand1, myStack.top_of_stack->value->e_data.ID.value, TOKEN_STRING, GF, false, false);
                oneOperandInstr(&instrList, PUSHS, operand1);
                break;
            case TOKEN_INT:;
                string operandString;
                stringInit(&operandString);
                char number[15];
                sprintf(number, "%d", myStack.top_of_stack->value->e_data.integer);
                stringAddChars(&operandString, number);
                operand1 = initOperand(operand1, operandString.value, TOKEN_STRING, GF, false, false);
                oneOperandInstr(&instrList, PUSHS, operand1);
                break;
            case TOKEN_DOUBLE:;
                string operandString1;
                stringInit(&operandString1);
                char number1[25];
                sprintf(number1, "%a", myStack.top_of_stack->value->e_data.decimal);
                stringAddChars(&operandString1, number1);
                operand1 = initOperand(operand1, operandString1.value, TOKEN_STRING, GF, false, false);
                oneOperandInstr(&instrList, PUSHS, operand1);
                break;
            case TOKEN_NONE:
                operand1 = initOperand(operand1, "None", TOKEN_STRING, GF, false, false);
                oneOperandInstr(&instrList, PUSHS, operand1);
                break;
            case TOKEN_ID:
            default:
                return ERROR_CODE_SEM_OTHER;

        }

        exp_stackPop(&myStack);
    }

    exp_stackClear(&myStack);

    operand1 = initOperand(operand1, "print", TOKEN_ID, GF, false, true);
    oneOperandInstr(&instrList, CALL, operand1);

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
    return ERROR_CODE_OK;
}