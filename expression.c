#include "expression.h"

Token token; //Převzatý token od scanneru

const char precedenceTable[PT_SIZE][PT_SIZE] = {
//           *     /     //    +     -     =    !=     <    <=     >    >=    ==     (     )   STR    INT   DOUB   ID   NONE        $
/*  *  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '>' },
/*  /  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '>' },
/* //  */ { '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '>' },
/*  +  */ { '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '>' },
/*  -  */ { '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '>' },
/*  =  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '_' , '>' },
/*  != */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '<' , '>' },
/*  <  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '<' , '>' },
/*  <= */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '<' , '>' },
/*  >  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '<' , '>' },
/*  >= */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '<' , '>' },
/*  == */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '<' , '<' , '>' },
/*  (  */ { '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , '<' , '<' , '<' , '<' , '<' , '_' },
/*  )  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '_' , '_' , '_' , '_' , '>' },
/* STR */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '_' , '_' , '_' , '_' , '>' },
/* INT */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '_' , '_' , '_' , '_' , '>' },
/* DOUB*/ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '_' , '_' , '_' , '_' , '>' },
/*  ID */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '_' , '_' , '_' , '_' , '>' },
/* NONE*/ { '_' , '_' , '_' , '_' , '_' , '_' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '_' , '_' , '_' , '_' , '>' },
/*  $  */ { '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '_' , '<' , '<' , '<' , '<' , '<' , '$' },
};


//hlavní funkce
int expression(VarType* returnValue) {/*,int expectedValue*/
    ERROR_CODE result;

    retVal = 8;

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
            if (next == TOKEN_UNDEF) {
                return ERROR_CODE_LEX;
            }
            if (token.t_type == TOKEN_ID && next == TOKEN_LEFTPAR) {
                result = makeFunction();
                if (result != ERROR_CODE_OK) return result;
                return ERROR_CODE_OK;
            }
            if (token.t_type == TOKEN_ADDITION || token.t_type == TOKEN_SUBTRACTION ||
            token.t_type == TOKEN_MULTIPLICATION || token.t_type == TOKEN_DIVISION ||
            token.t_type == TOKEN_INTEGER_DIVISION || token.t_type == TOKEN_EQUAL ||
            token.t_type == TOKEN_EQUAL_EQUAL || token.t_type == TOKEN_BIGGERTHAN ||
            token.t_type == TOKEN_BIGGERTHAN_EQUAL || token.t_type == TOKEN_SMALLERTHAN ||
            token.t_type == TOKEN_SMALLERTHAN_EQUAL || token.t_type == TOKEN_NEG_EQUAL) {
                if (stack_expression.top_of_stack->value->type == TOKEN_ADDITION ||
                stack_expression.top_of_stack->value->type == TOKEN_SUBTRACTION ||
                stack_expression.top_of_stack->value->type == TOKEN_MULTIPLICATION ||
                stack_expression.top_of_stack->value->type == TOKEN_DIVISION ||
                stack_expression.top_of_stack->value->type == TOKEN_INTEGER_DIVISION ||
                stack_expression.top_of_stack->value->type == TOKEN_EQUAL_EQUAL ||
                stack_expression.top_of_stack->value->type == TOKEN_EQUAL ||
                stack_expression.top_of_stack->value->type == TOKEN_BIGGERTHAN_EQUAL ||
                stack_expression.top_of_stack->value->type == TOKEN_BIGGERTHAN ||
                stack_expression.top_of_stack->value->type == TOKEN_SMALLERTHAN_EQUAL ||
                stack_expression.top_of_stack->value->type == TOKEN_SMALLERTHAN ||
                stack_expression.top_of_stack->value->type == TOKEN_NEG_EQUAL) {
                    fprintf(stderr, "Dva operatory za sebou ve vyrazu. \n");
                    return  ERROR_CODE_SYN;
                }
            }

            if (token.t_type == TOKEN_ID || token.t_type == TOKEN_INT || token.t_type == TOKEN_DOUBLE || token.t_type == TOKEN_STRING || token.t_type == TOKEN_LEFTPAR || token.t_type == TOKEN_NONE){
                exp_stackPush(&stack_expression,tokentoExp_element(token,true));
            }
            else{
                if (exp_stackEmpty(&stack_expression)) {
                    fprintf(stderr, "Operátor musí mít obě strany. %d\n", token.t_type);
                    return ERROR_CODE_SYN;
                }
                exp_stackPush(&stack_expression,tokentoExp_element(token,false));
            }
            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
            firstTerm = stack_expression.top_of_stack;
        }
        else if(sign == '>') {
            if (token.t_type == TOKEN_EOL || token.t_type == TOKEN_EOF || token.t_type == TOKEN_DOUBLEDOT) {
                if (stack_expression.top_of_stack->value->type == TOKEN_ADDITION ||
                stack_expression.top_of_stack->value->type == TOKEN_SUBTRACTION ||
                stack_expression.top_of_stack->value->type == TOKEN_MULTIPLICATION ||
                stack_expression.top_of_stack->value->type == TOKEN_DIVISION ||
                stack_expression.top_of_stack->value->type == TOKEN_INTEGER_DIVISION ||
                stack_expression.top_of_stack->value->type == TOKEN_EQUAL_EQUAL ||
                stack_expression.top_of_stack->value->type == TOKEN_EQUAL ||
                stack_expression.top_of_stack->value->type == TOKEN_BIGGERTHAN_EQUAL ||
                stack_expression.top_of_stack->value->type == TOKEN_BIGGERTHAN ||
                stack_expression.top_of_stack->value->type == TOKEN_SMALLERTHAN_EQUAL ||
                stack_expression.top_of_stack->value->type == TOKEN_SMALLERTHAN ||
                stack_expression.top_of_stack->value->type == TOKEN_NEG_EQUAL) {
                    fprintf(stderr, "Vyraz konci operatorem.\n");
                    return  ERROR_CODE_SYN;

                }
            }
            result = useRule(&stack_expression);
            if (result != ERROR_CODE_OK) {
            return result;
            }
        }
        else if(sign == '$') {
            return ERROR_CODE_OK;
        }
        else{
            fprintf(stderr, "Chybny vyraz (kompatibilita lexemu z vyrazu). %d\n", token.t_type);
            if (stack_expression.top_of_stack != NULL) {
                if (stack_expression.top_of_stack->value->type == TOKEN_NONE || token.t_type == TOKEN_NONE) {
                    return ERROR_CODE_SEM_COMP;
                }
            }
            return ERROR_CODE_SYN;
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


        if (stack_expression->top_of_stack->left != NULL){
            if (((Exp_element*)stack_expression->top_of_stack->left->value)->terminal == true){
            return ((Exp_element*)stack_expression->top_of_stack->left->value)->type;
            }


            if (stack_expression->top_of_stack->left->left != NULL){
                if (((Exp_element*)stack_expression->top_of_stack->left->left->value)->terminal == true){
                return ((Exp_element*)stack_expression->top_of_stack->left->left->value)->type;
                }

            }
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
        case TOKEN_NONE:
            ((Exp_element*)stack_expression->top_of_stack->value)->terminal = false;
            ((Exp_element *)stack_expression->top_of_stack->value)->handle = false;
            ((Exp_element *)stack_expression->top_of_stack->left->value)->handle = false;
            firstTerm = stack_expression->top_of_stack->left;

            //todo function term?


            //integer value
            if (stack_expression->top_of_stack->value->type == TOKEN_INT) {

                string operandString;
                stringInit(&operandString);

                char number[15];
                sprintf(number, "%d", stack_expression->top_of_stack->value->e_data.integer);

                stringAddChars(&operandString, number);
                operand operand = initOperand(operand, operandString.value, stack_expression->top_of_stack->value->type, LF, false, false);
                retVal = typeinteger;
                oneOperandInstr(&instrList, PUSHS, operand);
            }
            //double value
            else if (stack_expression->top_of_stack->value->type == TOKEN_DOUBLE) {
                string operandString;
                stringInit(&operandString);

                char number[25];
                sprintf(number, "%a", stack_expression->top_of_stack->value->e_data.decimal);

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

                result = makeIdInstr();
                if (result != ERROR_CODE_OK) return result;
                
            }
            else if (stack_expression->top_of_stack->value->type == TOKEN_NONE) {
                retVal = undefined;
                addInstruction(&instrList, PUSHS, "nil@nil", NULL, NULL);
            }

            return ERROR_CODE_OK;

        case TOKEN_ADDITION :
            if(stack_expression->top_of_stack->value->type == TOKEN_STRING) { //todo kdyz bude id string
                result = checkOperands(TOKEN_STRING, stack_expression->top_of_stack->value, stack_expression->top_of_stack->left->left->value);
                if (result != ERROR_CODE_OK) return result;
                string operandString;
                stringInit(&operandString);
                operand operand1 = initOperand(operand1,operandString.value,TOKEN_ID,LF, true, false);
                operand operand2 = initOperand(operand2,stack_expression->top_of_stack->value->e_data.ID.value, stack_expression->top_of_stack->value->type, LF, false, false);
                operand operand3 = initOperand(operand3,stack_expression->top_of_stack->left->left->value->e_data.ID.value, stack_expression->top_of_stack->left->left->value->type, LF, false, false);
                threeOperandInstr(&instrList, CONCAT, operand1, operand2, operand3);
                oneOperandInstr(&instrList, PUSHS, operand1);
                break;
            }
            result = checkOperands(TOKEN_INT, stack_expression->top_of_stack->value, stack_expression->top_of_stack->left->left->value);
            if (result != ERROR_CODE_OK) return result;
            noOperandInstr(&instrList, ADDS);
            break;

        case TOKEN_MULTIPLICATION :
        result = checkOperands(TOKEN_INT, stack_expression->top_of_stack->value, stack_expression->top_of_stack->left->left->value);
            if (result != ERROR_CODE_OK) return result;
            noOperandInstr(&instrList, MULS);
            break;

        case TOKEN_SUBTRACTION:
        result = checkOperands(TOKEN_INT, stack_expression->top_of_stack->value, stack_expression->top_of_stack->left->left->value);
            if (result != ERROR_CODE_OK) return result;
            noOperandInstr(&instrList, SUBS);
            break;

        case TOKEN_DIVISION:
            if (stack_expression != NULL) {
                if (stack_expression->top_of_stack != NULL) {
                    if (stack_expression->top_of_stack->value->type == TOKEN_INT) {
                        if (stack_expression->top_of_stack->value->e_data.integer == 0) {
                            return ERROR_CODE_ZERO_DEV;
                        }
                    }
                    else if(stack_expression->top_of_stack->value->type == TOKEN_DOUBLE) {
                        if (stack_expression->top_of_stack->value->e_data.decimal == 0.0) {
                            return ERROR_CODE_ZERO_DEV;
                        }
                    }
                }
            }
            result = checkOperands(TOKEN_INT, stack_expression->top_of_stack->value, stack_expression->top_of_stack->left->left->value);
            if (result != ERROR_CODE_OK) return result;
            noOperandInstr(&instrList, DIVS);
            break;

        case TOKEN_INTEGER_DIVISION:
            if (stack_expression != NULL) {
                if (stack_expression->top_of_stack != NULL) {
                    if (stack_expression->top_of_stack->value->type == TOKEN_INT) {
                        if (stack_expression->top_of_stack->value->e_data.integer == 0) {
                            return ERROR_CODE_ZERO_DEV;
                        }
                    }
                    else if(stack_expression->top_of_stack->value->type == TOKEN_DOUBLE) {
                        if (stack_expression->top_of_stack->value->e_data.decimal == 0.0) {
                            return ERROR_CODE_ZERO_DEV;
                        }
                    }
                }
            }
            result = checkOperands(TOKEN_INT, stack_expression->top_of_stack->value, stack_expression->top_of_stack->left->left->value);
            if (result != ERROR_CODE_OK) return result;
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
        case TOKEN_SMALLERTHAN:
            noOperandInstr(&instrList, LTS);
            break;
        case TOKEN_BIGGERTHAN:
            noOperandInstr(&instrList, GTS);
            break;
        case TOKEN_SMALLERTHAN_EQUAL:
            noOperandInstr(&instrList, GTS);
            noOperandInstr(&instrList, NOTS);
            break;
        case TOKEN_BIGGERTHAN_EQUAL:
            noOperandInstr(&instrList, LTS);
            noOperandInstr(&instrList, NOTS);
            break;

        default:
            return ERROR_CODE_SEM_COMP;
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
        Exp_element* quicksave = stack_expression->top_of_stack->left->value;

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

    tBSTNodePtr helpergf = SYMSearch(&glSymtable,stack_expression.top_of_stack->value->e_data.ID);
    tBSTNodePtr helperlf = SYMSearch(&lcSymtable,stack_expression.top_of_stack->value->e_data.ID);

    //first I have to check, if the id is not a function ID
    if (helpergf != NULL) {
        if (helpergf->DataType == Function) {
            fprintf(stderr, "Ve vyrazu pouzivate ID funkce. (\"%s\")\n", stack_expression.top_of_stack->value->e_data.ID.value);
            return ERROR_CODE_SEM;
        }
    }
   
    if(helperlf != NULL) {
        //ted jsem urcite ve funkci ne?
        switch (helperlf->Vartype) {
            case typeinteger:;
                operand operand1 = initOperand(operand1, helperlf->Key.value, TOKEN_ID, LF, false, false);
                retVal = typeinteger;
                oneOperandInstr(&instrList, PUSHS, operand1);
                break;
            case typedouble:
                retVal = typedouble;
                operand operand2 = initOperand(operand2, helperlf->Key.value, TOKEN_ID, LF, false, false);
                oneOperandInstr(&instrList, PUSHS, operand2);
                break;
            case typestring:
                retVal = typestring;
                operand operand3 = initOperand(operand3, helperlf->Key.value, TOKEN_ID, LF, false, false);
                oneOperandInstr(&instrList, PUSHS, operand3);
                break;
            case undefined:
                retVal = undefined;
                operand operand4 = initOperand(operand4, helperlf->Key.value, TOKEN_ID, LF, false, false);
                oneOperandInstr(&instrList, PUSHS, operand4);
                break;
            default:
                //not initialized vartype -> err
                printf("%s", helperlf->Key.value);
                return ERROR_CODE_INTERNAL;
        }
    }
    else if (helpergf != NULL) {
        switch (helpergf->Vartype) {
        case typeinteger:;
            operand operand1 = initOperand(operand1, helpergf->Key.value, TOKEN_ID, GF, false, false);
            retVal = typeinteger;
            oneOperandInstr(&instrList, PUSHS, operand1);
            break;
        case typedouble:
            retVal = typedouble;
            operand operand2 = initOperand(operand2, helpergf->Key.value, TOKEN_ID, GF, false, false);
            oneOperandInstr(&instrList, PUSHS, operand2);
            break;
        case typestring:
            retVal = typestring;
            operand operand3 = initOperand(operand3, helpergf->Key.value, TOKEN_ID, GF, false, false);
            oneOperandInstr(&instrList, PUSHS, operand3);
            break;
        case undefined:
            retVal = undefined;
            operand operand4 = initOperand(operand4, helpergf->Key.value, TOKEN_ID, GF, false, false);
            oneOperandInstr(&instrList, PUSHS, operand4);
            break;
        default:
            //not initialized vartype -> err
            return ERROR_CODE_INTERNAL;
        }
    }
    else {
        //promenna se nenasla v tabulce
        fprintf(stderr, "Promenna \"%s\" se nenasla v tabulce symbolu.\n", stack_expression.top_of_stack->value->e_data.ID.value);
        return ERROR_CODE_SEM;
    }
    return ERROR_CODE_OK;
}


ERROR_CODE makeFunction() {
    retVal = undefined;
    //i know for sure that now I have token ID as function name in token and nexttoken is leftpar
    //starting function
    //have to check if its in symtable
    //print function is standalone
    if (token.t_type == TOKEN_ID && strcmp(token.t_data.ID.value, "print") == 0) {
            return makePrintFunction();
    }
    if (token.t_type == TOKEN_ID && strcmp(token.t_data.ID.value, "len") == 0) {
            return makeLenFunction();
    }
    if (token.t_type == TOKEN_ID && strcmp(token.t_data.ID.value, "ord") == 0) {
            return makeOrdFunction();
    }
    if (token.t_type == TOKEN_ID && strcmp(token.t_data.ID.value, "chr") == 0) {
            return makeChrFunction();
    }
    if (token.t_type == TOKEN_ID && strcmp(token.t_data.ID.value, "substr") == 0) {
            return makeSubstrFunction();
    }

    tBSTNodePtr helper = SYMSearch(&glSymtable, token.t_data.ID);
    tBSTNodePtr helperLC = SYMSearch(&lcSymtable, token.t_data.ID);
    if (helperLC != NULL) {
        if (helperLC->DataType == Variable) {
            //there has been variable with the same name
            fprintf(stderr, "Jiz jste definovali promennou s nazvem \"%s\" ve funkci \"%s\".\n", token.t_data.ID.value, functionName.value);
            return ERROR_CODE_SEM;
        }
    }
    if (helper == NULL) {
        //now I know that this function doesnt exist yet
        if (strlen(functionName.value) != 0) {
            //jsem ve funkci a funkce neni definovana -> stejne ji zavolam
            //pridam ji do symtable (ale jako nedefinovanou)
            SYMInsert(&glSymtable, token.t_data.ID, true);
            helper = SYMSearch(&glSymtable, token.t_data.ID);
            helper->defined = false;
            //create temporary frame
            noOperandInstr(&instrList, CREATEFRAME);
            //count params
            int paramsCounter = 0;
            //eat (
            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
            if (token.t_type != TOKEN_LEFTPAR) {
                //co kdyby nahodou
                return ERROR_CODE_SYN;
            }
            //eat term
            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
            //now I have to generate params
            while (token.t_type != TOKEN_RIGHTPAR) {
                string nextParam;
                stringInit(&nextParam);
                stringAddChar(&nextParam, '%');
                char str[3];
                sprintf(str, "%d", paramsCounter);
                stringAddChars(&nextParam, str);
                operand1 = initOperand(operand1, nextParam.value, TOKEN_ID, TF, false, false);
                oneOperandInstr(&instrList, DEFVAR, operand1);
                switch(token.t_type) {
                    case TOKEN_STRING:
                        operand1 = initOperand(operand1, nextParam.value, TOKEN_ID, TF, false, false);
                        operand2 = initOperand(operand2, token.t_data.ID.value, TOKEN_STRING, TF, false, false);
                        twoOperandInstr(&instrList, MOVE, operand1, operand2);
                        break;
                    case TOKEN_INT:
                        operand1 = initOperand(operand1, nextParam.value, TOKEN_ID, TF, false, false);
                        string intString;
                        stringInit(&intString);
                        char str[15];
                        sprintf(str, "%d", token.t_data.integer);
                        stringAddChars(&intString, str);
                        operand2 = initOperand(operand2, intString.value, TOKEN_INT, TF, false, false);
                        twoOperandInstr(&instrList, MOVE, operand1, operand2);
                        break;
                    case TOKEN_DOUBLE:
                        operand1 = initOperand(operand1, nextParam.value, TOKEN_ID, TF, false, false);
                        string doubleString;
                        stringInit(&doubleString);
                        char str2[40];
                        sprintf(str2, "%a", token.t_data.decimal);
                        stringAddChars(&doubleString, str2);
                        operand2 = initOperand(operand2, doubleString.value, TOKEN_DOUBLE, TF, false, false);
                        twoOperandInstr(&instrList, MOVE, operand1, operand2);
                        break;
                    case TOKEN_NONE:
                        operand1 = initOperand(operand1, nextParam.value, TOKEN_ID, TF, false, false);
                        operand2 = initOperand(operand2, "None", TOKEN_STRING, TF, false, false);
                        twoOperandInstr(&instrList, MOVE, operand1, operand2);
                        break;
                    case TOKEN_ID:
                        operand1 = initOperand(operand1, nextParam.value, TOKEN_ID, TF, false, false);
                        tBSTNodePtr helper2 = SYMSearch(&glSymtable, token.t_data.ID);
                        tBSTNodePtr helper3 = SYMSearch(&lcSymtable, token.t_data.ID);
                        if (helper2 == NULL && helper3 == NULL) {
                            fprintf(stderr, "Promenna \"%s\" pouzita pri volani funkce \"%s\" neni definovana.\n", token.t_data.ID.value, helper->Key.value);
                            return ERROR_CODE_SEM;
                        }
                        else if (helper2 != NULL) {
                            if (helper2->DataType != Variable) {
                                fprintf(stderr, "Pri volani funkce \"%s\" pouzivate v parametrech ID funkce: %s\n", helper->Key.value, token.t_data.ID.value);
                                return ERROR_CODE_SEM;
                            }
                        }
                        if (helper3 != NULL) {
                            operand2 = initOperand(operand2, helper3->Key.value , TOKEN_ID, LF, false, false);
                        }
                        else {
                            operand2 = initOperand(operand2, helper2->Key.value , TOKEN_ID, GF, false, false);
                        }
                        twoOperandInstr(&instrList, MOVE, operand1, operand2);
                        break;
                    default:
                        fprintf(stderr, "Chyba v syntaxi volani funkce \"%s\".\n", helper->Key.value);
                        return ERROR_CODE_SYN;
                }
                if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
                if (token.t_type == TOKEN_COMMA) {
                    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
                    if (token.t_type == TOKEN_RIGHTPAR) {
                        //chyba v syntaxi volani funkce
                        fprintf(stderr, "Chyba v syntaxi volani funkce \"%s\".\n", helper->Key.value);
                        return ERROR_CODE_SYN;
                    }
                }
                else if (token.t_type == TOKEN_RIGHTPAR) {
                    paramsCounter++;
                    continue;
                }
                else {
                    //chyba v syntaxi volani funkce
                    fprintf(stderr, "Chyba v syntaxi volani funkce \"%s\".\n", helper->Key.value);
                    return ERROR_CODE_SYN;
                }

                paramsCounter++;
            }
            helper->parametrs = paramsCounter;

            operand1 = initOperand(operand1, helper->Key.value, TOKEN_ID, GF, false, true);
            oneOperandInstr(&instrList, CALL, operand1);

            noOperandInstr(&instrList, POPFRAME);

            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

            return ERROR_CODE_OK;
        }
        else if(helper == NULL && strlen(functionName.value) == 0) {
            //now im in main body -> error
            fprintf(stderr, "Funkce v hlavnim tele neexistuje (%s).\n", token.t_data.ID.value);
            return ERROR_CODE_SEM;
        }
    }
    else {
        //now I know that this function already exists (maybe still not defined)
        if (helper->DataType != Function) {
            //there has been variable with the same name
            fprintf(stderr, "Definovali jste promennou s nazvem \"%s\", ne funkci.\n", token.t_data.ID.value);
            return ERROR_CODE_SEM;
        }
        else {
            //funkce je definovana a muze v pohode probehnout volani fce
            //create temporary frame
            noOperandInstr(&instrList, CREATEFRAME);

            //count params
            int paramsCounter = 0;

            //eat (
            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
            if (token.t_type != TOKEN_LEFTPAR) {
                //co kdyby nahodou
                return ERROR_CODE_SYN;
            }

            //eat term
            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

            //now I have to generate params
            while (token.t_type != TOKEN_RIGHTPAR) {
                string nextParam;
                stringInit(&nextParam);
                stringAddChar(&nextParam, '%');
                char str[3];
                sprintf(str, "%d", paramsCounter);
                stringAddChars(&nextParam, str);
                operand1 = initOperand(operand1, nextParam.value, TOKEN_ID, TF, false, false);
                oneOperandInstr(&instrList, DEFVAR, operand1);

                switch(token.t_type) {
                    case TOKEN_STRING:
                        operand1 = initOperand(operand1, nextParam.value, TOKEN_ID, TF, false, false);
                        operand2 = initOperand(operand2, token.t_data.ID.value, TOKEN_STRING, TF, false, false);
                        twoOperandInstr(&instrList, MOVE, operand1, operand2);
                        break;
                    case TOKEN_INT:
                        operand1 = initOperand(operand1, nextParam.value, TOKEN_ID, TF, false, false);
                        string intString;
                        stringInit(&intString);
                        char str[15];
                        sprintf(str, "%d", token.t_data.integer);
                        stringAddChars(&intString, str);
                        operand2 = initOperand(operand2, intString.value, TOKEN_INT, TF, false, false);
                        twoOperandInstr(&instrList, MOVE, operand1, operand2);
                        break;
                    case TOKEN_DOUBLE:
                        operand1 = initOperand(operand1, nextParam.value, TOKEN_ID, TF, false, false);
                        string doubleString;
                        stringInit(&doubleString);
                        char str2[40];
                        sprintf(str2, "%a", token.t_data.decimal);
                        stringAddChars(&doubleString, str2);
                        operand2 = initOperand(operand2, doubleString.value, TOKEN_DOUBLE, TF, false, false);
                        twoOperandInstr(&instrList, MOVE, operand1, operand2);
                        break;
                    case TOKEN_NONE:
                        operand1 = initOperand(operand1, nextParam.value, TOKEN_ID, TF, false, false);
                        operand2 = initOperand(operand2, "None", TOKEN_STRING, TF, false, false);
                        twoOperandInstr(&instrList, MOVE, operand1, operand2);
                        break;
                    case TOKEN_ID:
                        operand1 = initOperand(operand1, nextParam.value, TOKEN_ID, TF, false, false);
                        tBSTNodePtr helper2 = SYMSearch(&glSymtable, token.t_data.ID);
                        tBSTNodePtr helper3 = SYMSearch(&lcSymtable, token.t_data.ID);
                        if (helper2 == NULL && helper3 == NULL) {
                            fprintf(stderr, "Promenna \"%s\" pouzita pri volani funkce \"%s\" neni definovana.\n", token.t_data.ID.value, helper->Key.value);
                            return ERROR_CODE_SEM;
                        }
                        else if (helper2 != NULL) {
                            if (helper2->DataType != Variable) {
                                fprintf(stderr, "Pri volani funkce \"%s\" pouzivate v parametrech ID funkce: %s\n", helper->Key.value, token.t_data.ID.value);
                                return ERROR_CODE_SEM;
                            }
                        }
                        if (helper3 != NULL) {
                            operand2 = initOperand(operand2, helper3->Key.value , TOKEN_ID, LF, false, false);
                        }
                        else {
                            operand2 = initOperand(operand2, helper2->Key.value , TOKEN_ID, GF, false, false);
                        }
                        twoOperandInstr(&instrList, MOVE, operand1, operand2);
                        break;
                    default:
                        fprintf(stderr, "Chyba v syntaxi volani funkce \"%s\".\n", helper->Key.value);
                        return ERROR_CODE_SYN;
                }
                if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
                if (token.t_type == TOKEN_COMMA) {
                    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
                    if (token.t_type == TOKEN_RIGHTPAR) {
                        //chyba v syntaxi volani funkce
                        fprintf(stderr, "Chyba v syntaxi volani funkce \"%s\".\n", helper->Key.value);
                        return ERROR_CODE_SYN;
                    }
                }
                else if (token.t_type == TOKEN_RIGHTPAR) {
                    paramsCounter++;
                    continue;
                }
                else {
                    //chyba v syntaxi volani funkce
                    fprintf(stderr, "Chyba v syntaxi volani funkce \"%s\".\n", helper->Key.value);
                    return ERROR_CODE_SYN;
                }

                paramsCounter++;
            }

            //now check params number
            if (helper->defined == true) {
                if (paramsCounter != helper->parametrs) {
                    fprintf(stderr, "Chybny pocet parametru pri volani funkce \"%s\".\n", helper->Key.value);
                    return ERROR_CODE_SEM_FUNC;
                }
            }
            else {
                helper->parametrs = paramsCounter;
            }

            operand1 = initOperand(operand1, helper->Key.value, TOKEN_ID, GF, false, true);
            oneOperandInstr(&instrList, CALL, operand1);

            noOperandInstr(&instrList, POPFRAME);

            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

            return ERROR_CODE_OK;
        }
    }
    return ERROR_CODE_INTERNAL;
}



ERROR_CODE makePrintFunction() {
    retVal = undefined;
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
            if (token.t_type == TOKEN_RIGHTPAR) {
                return ERROR_CODE_SYN;
            }
        }
        else if (token.t_type == TOKEN_RIGHTPAR) continue;
        else {
            fprintf(stderr, "Spatna syntax volani funkce \"print\".\n");
            return ERROR_CODE_SYN;
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
            case TOKEN_ID:;
                tBSTNodePtr helper22 = SYMSearch(&lcSymtable, myStack.top_of_stack->value->e_data.ID);
                tBSTNodePtr helper33 = SYMSearch(&glSymtable, myStack.top_of_stack->value->e_data.ID);
                if (helper33 != NULL) {
                    if (helper33->DataType == Function) {
                        fprintf(stderr, "Pri volani funkce \"print\" jste pouzili ID funkce. (%s)\n", myStack.top_of_stack->value->e_data.ID.value);
                        return ERROR_CODE_SEM;
                    }
                }
                
                if (helper22 == NULL && helper33 == NULL) {
                    fprintf(stderr, "Pri volani funkce \"print\" jste pouzili nedeklarovanou promennou: \"%s\"\n", myStack.top_of_stack->value->e_data.ID.value);
                    return ERROR_CODE_SEM;
                }
                else if (helper22 != NULL) {
                    operand1 = initOperand(operand1, myStack.top_of_stack->value->e_data.ID.value, TOKEN_ID, LF, false, false);
                    oneOperandInstr(&instrList, PUSHS, operand1);
                }
                else if (helper33 != NULL) {
                    operand1 = initOperand(operand1, myStack.top_of_stack->value->e_data.ID.value, TOKEN_ID, GF, false, false);
                    oneOperandInstr(&instrList, PUSHS, operand1);
                }
                break;
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

ERROR_CODE makeLenFunction() {
    retVal = typeinteger;
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //leftpar
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //first and only term

    noOperandInstr(&instrList, CREATEFRAME);

    if (token.t_type == TOKEN_ID) {
        tBSTNodePtr helper2 = SYMSearch(&glSymtable, token.t_data.ID);
        tBSTNodePtr helper3 = SYMSearch(&lcSymtable, token.t_data.ID);
        if (helper2 == NULL && helper3 == NULL) {
            fprintf(stderr, "Promenna \"%s\" pouzita pri volani funkce \"Len\" neni definovana.\n", token.t_data.ID.value);
            return ERROR_CODE_SEM;
        }
        else if (helper2 != NULL) {
            if (helper2->DataType != Variable) {
                fprintf(stderr, "Pri volani funkce \"Len\" pouzivate v parametrech ID funkce: %s\n", token.t_data.ID.value);
                return ERROR_CODE_SEM;
            }
        }
        if (helper3 != NULL) {
            if (helper3->Vartype == undefined || helper3->Vartype == typestring) {
                addInstruction(&instrList, DEFVAR, "TF@%1", NULL, NULL);
                operand1 = initOperand(operand1, "%1", TOKEN_ID, TF, false, false);
                operand2 = initOperand(operand2, helper3->Key.value , TOKEN_ID, LF, false, false);
                twoOperandInstr(&instrList, MOVE, operand1, operand2);
            }
            else {
                fprintf(stderr, "Pouzili jste nespravny typ parametru pri volani funkce \"Len\".\n (%s)", token.t_data.ID.value);
                return ERROR_CODE_SEM_COMP;
            }
        }
        else {
            if (helper2->Vartype == undefined || helper2->Vartype == typestring) {
                addInstruction(&instrList, DEFVAR, "TF@%1", NULL, NULL);
                operand1 = initOperand(operand1, "%1", TOKEN_ID, TF, false, false);
                operand2 = initOperand(operand2, helper2->Key.value , TOKEN_ID, GF, false, false);
                twoOperandInstr(&instrList, MOVE, operand1, operand2);
            }
            else {
                fprintf(stderr, "Pouzili jste nespravny typ parametru pri volani funkce \"Len\".\n (%s)", token.t_data.ID.value);
                return ERROR_CODE_SEM_COMP;
            }
        }

    }
    else if (token.t_type == TOKEN_STRING) {
        addInstruction(&instrList, DEFVAR, "TF@%1", NULL, NULL);
        operand1 = initOperand(operand1, "%1", TOKEN_ID, TF, false, false);
        operand2 = initOperand(operand2, token.t_data.ID.value , TOKEN_STRING, GF, false, false);
        twoOperandInstr(&instrList, MOVE, operand1, operand2);
    }
    else {
        fprintf(stderr, "Pouzili jste nespravny parametr pri volani funkce \"Len\".\n");
        return ERROR_CODE_SEM_COMP;
    }

    addInstruction(&instrList, CALL, "$len", NULL, NULL);

    noOperandInstr(&instrList, POPFRAME);

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
    if(token.t_type != TOKEN_RIGHTPAR) {
        return ERROR_CODE_SYN;
    }
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

    return ERROR_CODE_OK;
}

ERROR_CODE makeOrdFunction() {
    retVal = typeinteger;
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //leftpar
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //first and only term

    noOperandInstr(&instrList, CREATEFRAME);

    if (token.t_type == TOKEN_ID) {
        tBSTNodePtr helper2 = SYMSearch(&glSymtable, token.t_data.ID);
        tBSTNodePtr helper3 = SYMSearch(&lcSymtable, token.t_data.ID);
        if (helper2 == NULL && helper3 == NULL) {
            fprintf(stderr, "Promenna \"%s\" pouzita pri volani funkce \"Ord\" neni definovana.\n", token.t_data.ID.value);
            return ERROR_CODE_SEM;
        }
        else if (helper2 != NULL) {
            if (helper2->DataType != Variable) {
                fprintf(stderr, "Pri volani funkce \"Ord\" pouzivate v parametrech ID funkce: %s\n", token.t_data.ID.value);
                return ERROR_CODE_SEM;
            }
        }
        if (helper3 != NULL) {
            if (helper3->Vartype == undefined || helper3->Vartype == typestring) {
                addInstruction(&instrList, DEFVAR, "TF@%1", NULL, NULL);
                operand1 = initOperand(operand1, "%1", TOKEN_ID, TF, false, false);
                operand2 = initOperand(operand2, helper3->Key.value , TOKEN_ID, LF, false, false);
                twoOperandInstr(&instrList, MOVE, operand1, operand2);
            }
            else {
                fprintf(stderr, "Pouzili jste nespravny typ parametru pri volani funkce \"Ord\".\n (%s)", token.t_data.ID.value);
                return ERROR_CODE_SEM_COMP;
            }
        }
        else {
            if (helper2->Vartype == undefined || helper2->Vartype == typestring) {
                addInstruction(&instrList, DEFVAR, "TF@%1", NULL, NULL);
                operand1 = initOperand(operand1, "%1", TOKEN_ID, TF, false, false);
                operand2 = initOperand(operand2, helper2->Key.value , TOKEN_ID, GF, false, false);
                twoOperandInstr(&instrList, MOVE, operand1, operand2);
            }
            else {
                fprintf(stderr, "Pouzili jste nespravny typ parametru pri volani funkce \"Ord\".\n (%s)", token.t_data.ID.value);
                return ERROR_CODE_SEM_COMP;
            }
        }

    }
    else if (token.t_type == TOKEN_STRING) {
        addInstruction(&instrList, DEFVAR, "TF@%1", NULL, NULL);
        operand1 = initOperand(operand1, "%1", TOKEN_ID, TF, false, false);
        operand2 = initOperand(operand2, token.t_data.ID.value , TOKEN_STRING, GF, false, false);
        twoOperandInstr(&instrList, MOVE, operand1, operand2);
    }
    else {
        fprintf(stderr, "Pouzili jste nespravny parametr pri volani funkce \"Ord\".\n");
        return ERROR_CODE_SEM_COMP;
    }

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //comma
    if(token.t_type != TOKEN_COMMA) {
        return ERROR_CODE_SYN;
    }

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

    if (token.t_type == TOKEN_ID) {
        tBSTNodePtr helper22 = SYMSearch(&glSymtable, token.t_data.ID);
        tBSTNodePtr helper33 = SYMSearch(&lcSymtable, token.t_data.ID);
        if (helper22 == NULL && helper33 == NULL) {
            fprintf(stderr, "Promenna \"%s\" pouzita pri volani funkce \"Ord\" neni definovana.\n", token.t_data.ID.value);
            return ERROR_CODE_SEM;
        }
        else if (helper22 != NULL) {
            if (helper22->DataType != Variable) {
                fprintf(stderr, "Pri volani funkce \"Ord\" pouzivate v parametrech ID funkce: %s\n", token.t_data.ID.value);
                return ERROR_CODE_SEM;
            }
        }
        if (helper33 != NULL) {
            if (helper33->Vartype == undefined || helper33->Vartype == typeinteger) {
                addInstruction(&instrList, DEFVAR, "TF@%2", NULL, NULL);
                operand1 = initOperand(operand1, "%2", TOKEN_ID, TF, false, false);
                operand2 = initOperand(operand2, helper33->Key.value , TOKEN_ID, LF, false, false);
                twoOperandInstr(&instrList, MOVE, operand1, operand2);
            }
            else {
                fprintf(stderr, "Pouzili jste nespravny typ parametru pri volani funkce \"Ord\".\n (%s)", token.t_data.ID.value);
                return ERROR_CODE_SEM_COMP;
            }
        }
        else {
            if (helper22->Vartype == undefined || helper22->Vartype == typeinteger) {
                addInstruction(&instrList, DEFVAR, "TF@%2", NULL, NULL);
                operand1 = initOperand(operand1, "%2", TOKEN_ID, TF, false, false);
                operand2 = initOperand(operand2, helper22->Key.value , TOKEN_ID, GF, false, false);
                twoOperandInstr(&instrList, MOVE, operand1, operand2);
            }
            else {
                fprintf(stderr, "Pouzili jste nespravny typ parametru pri volani funkce \"Ord\".\n (%s)", token.t_data.ID.value);
                return ERROR_CODE_SEM_COMP;
            }
        }

    }
    else if (token.t_type == TOKEN_INT) {
        addInstruction(&instrList, DEFVAR, "TF@%2", NULL, NULL);
        operand1 = initOperand(operand1, "%2", TOKEN_ID, TF, false, false);
        string temp;
        stringInit(&temp);
        char number[20];
        sprintf(number, "%d", token.t_data.integer);
        stringAddChars(&temp, number);
        operand2 = initOperand(operand2, temp.value , TOKEN_INT, GF, false, false);
        twoOperandInstr(&instrList, MOVE, operand1, operand2);
    }
    else {
        fprintf(stderr, "Pouzili jste nespravny parametr pri volani funkce \"Ord\".\n");
        return ERROR_CODE_SEM_COMP;
    }

    addInstruction(&instrList, CALL, "$ord", NULL, NULL);

    noOperandInstr(&instrList, POPFRAME);

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
    if(token.t_type != TOKEN_RIGHTPAR) {
        return ERROR_CODE_SYN;
    }
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

    return ERROR_CODE_OK;
}

ERROR_CODE makeChrFunction() {
    retVal = typestring;
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //leftpar
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //first and only term

    noOperandInstr(&instrList, CREATEFRAME);

    if (token.t_type == TOKEN_ID) {
        tBSTNodePtr helper2 = SYMSearch(&glSymtable, token.t_data.ID);
        tBSTNodePtr helper3 = SYMSearch(&lcSymtable, token.t_data.ID);
        if (helper2 == NULL && helper3 == NULL) {
            fprintf(stderr, "Promenna \"%s\" pouzita pri volani funkce \"Chr\" neni definovana.\n", token.t_data.ID.value);
            return ERROR_CODE_SEM;
        }
        else if (helper2 != NULL) {
            if (helper2->DataType != Variable) {
                fprintf(stderr, "Pri volani funkce \"Chr\" pouzivate v parametrech ID funkce: %s\n", token.t_data.ID.value);
                return ERROR_CODE_SEM;
            }
        }
        if (helper3 != NULL) {
            if (helper3->Vartype == undefined || helper3->Vartype == typeinteger) {
                addInstruction(&instrList, DEFVAR, "TF@%1", NULL, NULL);
                operand1 = initOperand(operand1, "%1", TOKEN_ID, TF, false, false);
                operand2 = initOperand(operand2, helper3->Key.value , TOKEN_ID, LF, false, false);
                twoOperandInstr(&instrList, MOVE, operand1, operand2);
            }
            else {
                fprintf(stderr, "Pouzili jste nespravny typ parametru pri volani funkce \"CHr\".\n (%s)", token.t_data.ID.value);
                return ERROR_CODE_SEM_COMP;
            }
        }
        else {
            if (helper2->Vartype == undefined || helper2->Vartype == typestring) {
                operand1 = initOperand(operand1, "%1", TOKEN_ID, TF, false, false);
                operand2 = initOperand(operand2, helper2->Key.value , TOKEN_ID, GF, false, false);
                twoOperandInstr(&instrList, MOVE, operand1, operand2);
            }
            else {
                fprintf(stderr, "Pouzili jste nespravny typ parametru pri volani funkce \"Chr\".\n (%s)", token.t_data.ID.value);
                return ERROR_CODE_SEM_COMP;
            }
        }

    }
    else if (token.t_type == TOKEN_INT) {
        addInstruction(&instrList, DEFVAR, "TF@%1", NULL, NULL);
        operand1 = initOperand(operand1, "%1", TOKEN_ID, TF, false, false);
        if (token.t_data.integer >= 256 || token.t_data.integer < 0) {
            fprintf(stderr, "Pouzili jste nezname cislo pri volani funkce Chr.\n");
            return ERROR_CODE_SEM_COMP;
        }
        string temp;
        stringInit(&temp);
        char number[20];
        sprintf(number, "%d", token.t_data.integer);
        stringAddChars(&temp, number);
        operand2 = initOperand(operand2, temp.value , TOKEN_INT, GF, false, false);
        twoOperandInstr(&instrList, MOVE, operand1, operand2);
    }
    else {
        fprintf(stderr, "Pouzili jste nespravny parametr pri volani funkce \"Chr\".\n");
        return ERROR_CODE_SEM_COMP;
    }

    addInstruction(&instrList, CALL, "$chr", NULL, NULL);

    noOperandInstr(&instrList, POPFRAME);

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
    if(token.t_type != TOKEN_RIGHTPAR) {
        return ERROR_CODE_SYN;
    }
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

    return ERROR_CODE_OK;
}

ERROR_CODE makeSubstrFunction() {
    retVal = typestring;
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //leftpar  
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //string
    if (token.t_type == TOKEN_STRING || token.t_type == TOKEN_ID) {
        ;
    }
    else {
        return ERROR_CODE_SEM_COMP;
    }
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //comma
    if (token.t_type != TOKEN_COMMA) {
        return ERROR_CODE_SYN;
    }
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //integer
    if (token.t_type == TOKEN_ID || token.t_type == TOKEN_INT) {
        ;
    }
    else {
        return ERROR_CODE_SEM_COMP;
    }
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //comma
    if (token.t_type != TOKEN_COMMA) {
        return ERROR_CODE_SYN;
    }
     if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //integer
    if (token.t_type == TOKEN_ID || token.t_type == TOKEN_INT) {
        ;
    }
    else {
        return ERROR_CODE_SEM_COMP;
    }

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //rightpar
    if (token.t_type != TOKEN_RIGHTPAR) {
        return ERROR_CODE_SYN;
    }

    addInstruction(&instrList, PUSHS, "string@NotCompltedYet", NULL, NULL);

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

    return ERROR_CODE_OK;
}

ERROR_CODE checkOperands(token_type rightResultType, Exp_element* element1, Exp_element* element2) {
    if (element1 == NULL || element2 == NULL) {
        return ERROR_CODE_INTERNAL;
    }
    if (element1->type == TOKEN_ID) {
        tBSTNodePtr helper = SYMSearch(&glSymtable, element1->e_data.ID);
        tBSTNodePtr helper2 = SYMSearch(&lcSymtable, element1->e_data.ID);
        if (helper != NULL) {
            if (helper->DataType == Function) {
                fprintf(stderr, "Ve vyrazu jste pouzili ID funkce.\n");
                return ERROR_CODE_SEM;
            }
        }
        if (helper2 == NULL && helper == NULL) {
            fprintf(stderr, "Promenna pouzita ve vyrazu nebyla definovana.\n");
            return ERROR_CODE_SEM;
        }
        else if (helper2 != NULL) {
            if (rightResultType == TOKEN_INT) {
                if (helper2->Vartype == typestring) {
                    return ERROR_CODE_SEM_COMP;
                }
            }
            else if (rightResultType == TOKEN_STRING) {
                if (helper2->Vartype == typedouble || helper2->Vartype == typeinteger) {
                    return ERROR_CODE_SEM_COMP;
                }
            }
        }
        else if (helper != NULL) {
            if (rightResultType == TOKEN_INT) {
                if (helper->Vartype == typestring) {
                    return ERROR_CODE_SEM_COMP;
                }
            }
            else if (rightResultType == TOKEN_STRING) {
                if (helper->Vartype == typedouble || helper->Vartype == typeinteger) {
                    return ERROR_CODE_SEM_COMP;
                }
            }
        }
    }
    else if (element1->type == TOKEN_INT || element1->type == TOKEN_DOUBLE) {
        if (rightResultType != TOKEN_INT) {
            return ERROR_CODE_SEM_COMP;
        }
    }
    else if (element1->type == TOKEN_STRING) {
        if (rightResultType != TOKEN_STRING) {
            return ERROR_CODE_SEM_COMP;
        }
    }

    if (element2->type == TOKEN_ID) {
        tBSTNodePtr helper = SYMSearch(&glSymtable, element2->e_data.ID);
        tBSTNodePtr helper2 = SYMSearch(&lcSymtable, element2->e_data.ID);
        if (helper != NULL) {
            if (helper->DataType == Function) {
                fprintf(stderr, "Ve vyrazu jste pouzili ID funkce.\n");
                return ERROR_CODE_SEM;
            }
        }
        if (helper2 == NULL && helper == NULL) {
            fprintf(stderr, "Promenna pouzita ve vyrazu nebyla definovana.\n");
            return ERROR_CODE_SEM;
        }
        else if (helper2 != NULL) {
            if (rightResultType == TOKEN_INT) {
                if (helper2->Vartype == typestring) {
                    return ERROR_CODE_SEM_COMP;
                }
            }
            else if (rightResultType == TOKEN_STRING) {
                if (helper2->Vartype == typedouble || helper2->Vartype == typeinteger) {
                    return ERROR_CODE_SEM_COMP;
                }
            }
        }
        else if (helper != NULL) {
            if (rightResultType == TOKEN_INT) {
                if (helper->Vartype == typestring) {
                    return ERROR_CODE_SEM_COMP;
                }
            }
            else if (rightResultType == TOKEN_STRING) {
                if (helper->Vartype == typedouble || helper->Vartype == typeinteger) {
                    return ERROR_CODE_SEM_COMP;
                }
            }
        }
    }
    else if (element2->type == TOKEN_INT || element2->type == TOKEN_DOUBLE) {
        if (rightResultType != TOKEN_INT) {
            return ERROR_CODE_SEM_COMP;
        }
    }
    else if (element2->type == TOKEN_STRING) {
        if (rightResultType != TOKEN_STRING) {
            return ERROR_CODE_SEM_COMP;
        }
    }


    return ERROR_CODE_OK;
}