#include "expression.h"

Token token; //Převzatý token od scanneru

const char precedenceTable[PT_SIZE][PT_SIZE] = {
//           *     /     //    +     -     =    !=     <    <=     >    >=    ==     (     )     ID    F     ,     $
/*  *  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  /  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  /  */ { '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  +  */ { '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  -  */ { '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  =  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  != */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  <  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  <= */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  >  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  >= */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  == */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  (  */ { '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , '<' , '_' , '<' , '_' },
/*  )  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '_' , '_' , '>' },
/*  ID */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '_' , '>' , '>' },
/*  F  */ { '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '_' , '_' , '_' , '_' , '_' },
/*  ,  */ { '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '<' , '_' , '<' , '_' },
/*  $  */ { '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '_' , '<' , '<' , '_' , '$' },
};

int expression() {/*,int expectedValue*/
    ERROR_CODE result;

    result = expressionAnalysis();

    exp_stackClear(&stack_expression);
    return result;
}

int expressionAnalysis() {
    ERROR_CODE result;
    char sign = '_';
    while(true) {
          sign = getSignFromTable();
          if(sign == '='){

              exp_stackPush(&stack_expression,tokentoExp_element(token,false));
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

char getSignFromTable(){
    int a;
    int b;
    b = token.t_type;
    a = get_stack_type(&stack_expression);
    //printf("%d %d \n", a, b);
    //printf("%c \n", precedenceTable[a][b]);
    return precedenceTable[a][b];
}


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
      new_element->type = type;
      new_element->handle = handle;
      new_element->terminal = true;

      return new_element;
  }
  else return NULL;
}
int get_stack_type(ptrStack *stack_expression){
    if (((Exp_element*)stack_expression->top_of_stack->value)->terminal == true){
        return ((Exp_element*)stack_expression->top_of_stack->value)->type;
      }
    if (((Exp_element*)stack_expression->top_of_stack->left->value)->terminal == true){
        return ((Exp_element*)stack_expression->top_of_stack->left->value)->type;
    }
    if (((Exp_element*)stack_expression->top_of_stack->left->value)->terminal == true){
        return ((Exp_element*)stack_expression->top_of_stack->left->value)->type;
    }
    return ERROR_CODE_OK;
}

Exp_element *tokentoExp_element(Token token,bool handle){
    int type;
    if (token.t_type == TOKEN_EOL || token.t_type == TOKEN_EOF) {
        type = TOKEN_UNDEF;
    }
    type = token.t_type;
    Exp_element *element_to_stack = newElement(type, handle);
    return element_to_stack;
}
/*
int convertTokenToIndex(Token token){
    switch(token.t_type){
        case TOKEN_MULTIPLICATION:
            return EXP_MULTIPLY;
        case TOKEN_DIVISION:
            return EXP_DIVIDE;
        case TOKEN_INTEGER_DIVISION:
            return EXP_INTEGER_DIVIDE;
        case TOKEN_ADDITION:
            return EXP_PLUS;
        case TOKEN_SUBTRACTION:
            return EXP_MINUS;
        case TOKEN_EQUAL:
            return EXP_EQUAL;
        case TOKEN_NEG_EQUAL:
            return EXP_NOT_EQUAL;
        case TOKEN_SMALLERTHEN:
            return EXP_LESS;
        case TOKEN_SMALLERTHEN_EQUAL:
            return EXP_LESS_EQUAL;
        case TOKEN_BIGGERTHEN:
            return EXP_MORE;
        case TOKEN_BIGGERTHEN_EQUAL:
            return EXP_MORE_EQUAL;
        case TOKEN_EQUAL_EQUAL:
            return EXP_COMPARISON;
        case TOKEN_LEFTPAR:
            return EXP_LEFT_PAR;
        case TOKEN_RIGHTPAR:
            return EXP_RIGHT_PAR;
        case TOKEN_INT:
            return EXP_OPERAND;
        case TOKEN_DOUBLE:
            return EXP_OPERAND;
        case TOKEN_STRING:
            return EXP_OPERAND;
        case TOKEN_EOL:
            return EXP_DOLLAR;

        default:
            return EXP_OTHER;
    }
}
*/
int useRule(ptrStack *stack_expression){

    switch(firstTerm->value->type) {

        case EXP_OPERAND :
            ((Exp_element*)stack_expression->top_of_stack->value)->terminal = false;
            ((Exp_element *)stack_expression->top_of_stack->value)->handle = false;
            ((Exp_element *)stack_expression->top_of_stack->left->value)->handle = false;
            firstTerm = stack_expression->top_of_stack->left;

            //todo function term?

            if (stack_expression->top_of_stack->value->type == TOKEN_INT) {
                string operandString;
                stringInit(&operandString);
                char number[6];
                sprintf(number, "%d", stack_expression->top_of_stack->value->e_data.integer);
                stringAddChars(&operandString, number);
                operand operand = initOperand(operand, operandString, stack_expression->top_of_stack->value->type, LF, false, false);
                oneOperandInstr(&instrList, PUSHS, operand);
            }
            
            return ERROR_CODE_OK;

        case EXP_PLUS :
            //todo concatenace stringu

            noOperandInstr(&instrList, ADDS);
            break;

        case EXP_MULTIPLY :
            noOperandInstr(&instrList, MULS);
            break;

        case EXP_MINUS:
            noOperandInstr(&instrList, SUBS);
            break;

        case EXP_DIVIDE:
            noOperandInstr(&instrList, DIVS);
            break;
            
        case EXP_INTEGER_DIVIDE:
            noOperandInstr(&instrList, DIVS);
            noOperandInstr(&instrList, FLOAT2INTS);
            break;
        case EXP_EQUAL:
            noOperandInstr(&instrList, EQS);
            break;
        case EXP_NOT_EQUAL:
            noOperandInstr(&instrList, EQS);
            noOperandInstr(&instrList, NOTS);
            break;
        case EXP_LESS:
            noOperandInstr(&instrList, LTS);
            break;
        case EXP_MORE:
            noOperandInstr(&instrList, GTS);
            break;
        case EXP_LESS_EQUAL:
            noOperandInstr(&instrList, GTS);
            noOperandInstr(&instrList, NOTS);
            break;
        case EXP_MORE_EQUAL:
            noOperandInstr(&instrList, LTS);
            noOperandInstr(&instrList, NOTS);
            break;
        
        default:
          return ERROR_CODE_SEM;
    }
    exp_stackPop(stack_expression);
    exp_stackPop(stack_expression);
    if(stack_expression->top_of_stack != NULL){
        firstTerm = stack_expression->top_of_stack->left;
    }
    return ERROR_CODE_OK;
}
