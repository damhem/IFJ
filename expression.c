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
          }else if(sign == '<'){

              if (token.t_type == TOKEN_ID || token.t_type == TOKEN_INT || token.t_type == TOKEN_DOUBLE || token.t_type == TOKEN_STRING){
                  exp_stackPush(&stack_expression,tokentoExp_element(token,true));
              }else{

                  exp_stackPush(&stack_expression,tokentoExp_element(token,false));
              }

              if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
              firstTerm = stack_expression.top_of_stack;

          } else if(sign == '>') {

              result = useRule(&stack_expression);
              if (result != ERROR_CODE_OK) {
                  return result;
              }

          } else if(sign == '$') {
              

            return 0;

          }else{

              return 2;

          }
    }
}
// Funkce pro hledání znaku z precedencni tabulky
char getSignFromTable(){
    int a;
    int b;
    if (token.t_type == TOKEN_EOL || token.t_type == TOKEN_EOF) {
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
      if(token.t_type == TOKEN_INT){
      new_element->e_data.integer = token.t_data.integer;
      }
      if(token.t_type == TOKEN_DOUBLE){
      new_element->e_data.decimal = token.t_data.decimal;
      }
      if(token.t_type == TOKEN_STRING){
      new_element->e_data.ID = token.t_data.ID;
      }
      if (token.t_type == TOKEN_ID) {
        //printf("ddjdjd  %s\n", token.t_data.ID.value);

        stringInit(&(new_element->e_data.ID));
        stringAddChars(&(new_element->e_data.ID), new_element->e_data.ID.value);
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
    if (token.t_type == TOKEN_EOL || token.t_type == TOKEN_EOF) {
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
                operand operand = initOperand(operand, operandString, stack_expression->top_of_stack->value->type, LF, false, false);
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
                operand operand = initOperand(operand, operandString, stack_expression->top_of_stack->value->type, LF, false, false);
                retVal = typedouble;
                oneOperandInstr(&instrList, PUSHS, operand);
            }
            //string value
            else if (stack_expression->top_of_stack->value->type == TOKEN_STRING) {
                operand operand = initOperand(operand, stack_expression->top_of_stack->value->e_data.ID, stack_expression->top_of_stack->value->type, LF, false, false);
                retVal = typestring;
                oneOperandInstr(&instrList, PUSHS, operand);
            }
            //there has to be ID analysis
            else if (stack_expression->top_of_stack->value->type == TOKEN_ID) {
                result = makeIdInstr();
                if (result != ERROR_CODE_OK) return result;
                //todo ID analysisi
                return 2;
            }

            return ERROR_CODE_OK;

        case TOKEN_ADDITION :
            if(stack_expression->top_of_stack->value->type == TOKEN_STRING) { //todo kdyz bude id string
                string operandString;
                stringInit(&operandString);
                operand operand1 = initOperand(operand1,operandString,TOKEN_ID,LF, true, false);
                operand operand2 = initOperand(operand2,stack_expression->top_of_stack->value->e_data.ID, stack_expression->top_of_stack->value->type, LF, false, false);
                operand operand3 = initOperand(operand3,stack_expression->top_of_stack->left->left->value->e_data.ID, stack_expression->top_of_stack->left->left->value->type, LF, false, false);
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
            
        }else return ERROR_CODE_SYN;
}else return ERROR_CODE_SYN;
return ERROR_CODE_OK;
}

//Vyhodnocovani ID
ERROR_CODE makeIdInstr() {
    //todo function
    tBSTNodePtr helper = symTableSearch(&glSymtable, firstTerm->value->e_data.ID);
    if (helper != NULL) {
        switch (helper->Vartype) {
        case typeinteger:
            
            break;
        case typedouble:

        case typestring:

        case undefined:
            //variable is not defined -> dont know what to do with it
            //printf("im here");
            return ERROR_CODE_SEM;
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