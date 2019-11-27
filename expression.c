#include "expression.h"
#include "string.h"
#include "scanner.h"

Token token; //Převzatý token od scanneru

const char precedenceTable[PT_SIZE][PT_SIZE] = {
//           *     /     //    +     -     =    !=     <    <=     >    >=    ==     (     )     ID    F     ,     $
/*  *  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  /  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/* //  */ { '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  +  */ { '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  -  */ { '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  =  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  != */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  <  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  <= */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  >  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  >= */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  == */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' };
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

int expressionAnalysis(ptrStack *stack_expression) {
    /*Exp_element element_on_stack;*/
    //ERROR_CODE result;
    char sign = '_';
    while(1) {
          sign = getSignFromTable();
          if(sign == '='){
              exp_stackPush(stack_expression,tokentoExp_element(token,false));
              if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
          }else if(sign == '<'){
              if (convertTokenToIndex(token) == EXP_OPERAND){
                  exp_stackPush(stack_expression,tokentoExp_element(token,true));
              }else{
                  exp_stackPush(stack_expression,tokentoExp_element(token,false));
              }
              if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
          }else if(sign == '>'){
              return useRule(&stack_expression);

          }else if(sign == '$'){

              return 0;


          }else{
              printf("dhdhhd\n");

              return 2;

          }
    }
}

char getSignFromTable(){
    int a;
    int b;
    b = convertTokenToIndex(token);
    a = get_stack_type(&stack_expression);
    printf("%d %d \n", a, b);
    printf("%c \n", precedenceTable[a][b]);
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
    if (((Exp_element*)stack_expression->top_of_stack->left->value)->terminal == true){
        return ((Exp_element*)stack_expression->top_of_stack->left->value)->type;
    }
    if (((Exp_element*)stack_expression->top_of_stack->left->value)->terminal == true){
        return ((Exp_element*)stack_expression->top_of_stack->left->value)->type;
    }
}

Exp_element *tokentoExp_element(Token token,bool handle){
    int type = convertTokenToIndex(token);
    Exp_element *element_to_stack = newElement(type, handle);
    return element_to_stack;
}
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

int useRule(ptrStack *stack_expression){

    switch(stack_expression->top_of_stack->type) {
        case EXP_OPERAND :
            if( ((Exp_element*)stack_expression->top_of_stack->value)->handle == true){
                ((Exp_element*)stack_expression->top_of_stack->value)->terminal = false;
            }

    }
}
