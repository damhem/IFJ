#include "expression.h"
#include "string.h"
#include "scanner.h"

Token token; //Převzatý token od scanneru

const char precedenceTable[PT_SIZE][PT_SIZE] = {
//           *     /     //    +     -     =    !=     <    <=     >    >=    ==    (     )     ID     $
/*  *  */ { '_' , '_' , '_' , '>' , '>' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '_' , '<' , '_' },
/*  /  */ { '_' , '_' , '_' , '>' , '>' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '_' , '<' , '_' },
/*  // */ { '_' , '_' , '_' , '>' , '>' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '_' , '<' , '_' },
/*  +  */ { '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '_' , '<' , '_' },
/*  -  */ { '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '_' , '<' , '_' },
/*  =  */ { '_' , '_' , '_' , '<' , '<' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '_' , '<' , '_' },
/*  != */ { '_' , '_' , '_' , '<' , '<' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '_' , '<' , '_' },
/*  <  */ { '_' , '_' , '_' , '<' , '<' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '_' , '<' , '_' },
/*  <= */ { '_' , '_' , '_' , '<' , '<' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '_' , '<' , '_' },
/*  >  */ { '_' , '_' , '_' , '<' , '<' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '_' , '<' , '_' },
/*  >= */ { '_' , '_' , '_' , '<' , '<' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '_' , '<' , '_' },
/*  == */ { '_' , '_' , '_' , '<' , '<' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '_' , '<' , '_' },
/*  (  */ { '_' , '_' , '_' , '<' , '<' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '=' , '<' , '_' },
/*  )  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '>' },
/*  ID */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '>' },
/*  $  */ { '_' , '_' , '_' , '<' , '<' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '_' , '<' , '$' },
};

int expression(){/*,int expectedValue*/
    int result;




    result=expressionAnalysis(&stack_expression);

    exp_stackClear(&stack_expression);
    return result;
}

int expressionAnalysis(ptrStack *stack_expression){
    /*Exp_element element_on_stack;*/
    char sign = '_';
    while(1){
          sign = getSignFromTable();
          if(sign == '='){
              exp_stackPush(stack_expression,tokentoExp_element(token,false));
              if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
          }else if(sign == '<'){
              if(stack_expression != NULL){
                   ((Exp_element*)stack_expression->top_of_stack->value)->handle = true;
              }
              exp_stackPush(stack_expression,tokentoExp_element(token,true));
              if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
          }else if(sign == '>'){
              exp_stackPush(stack_expression, tokentoExp_element(token, false));
              if( ((Exp_element*)stack_expression->top_of_stack->value)->handle == true){
                  exp_stackPop(stack_expression);
              }
              if( ((Exp_element*)stack_expression->top_of_stack->left->value)->handle == true){
                  exp_stackPop(stack_expression);
                  exp_stackPop(stack_expression);
                  if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

              }
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

int initexpressionStack(ptrStack *stack_expression){
    if(stack_expression != NULL){
      exp_stackInit(stack_expression);
    }


    //Vytvořím string "$"
    string dollar;
    stringInit(&dollar);
    stringAddChar(&dollar, '$');
    int type = EXP_DOLLAR;
    Exp_element *new_element = newElement(type,false);
    exp_stackPush(stack_expression, new_element);
    return 0 ;
}


Exp_element *newElement(int type,bool handle){
  Exp_element *new_element = malloc(sizeof(struct exp_element));

  //Inicializace noveho elementu
  if(new_element != NULL){
      new_element->type = type;
      new_element->handle = handle;
      /*new_element->terminal = terminal;*/

      return new_element;
  }
  else return NULL;
}
int get_stack_type(ptrStack *stack_expression){
    return ((Exp_element*)stack_expression->top_of_stack->value)->type;
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
/*
int useRule(ptrStack *stack_expression){

    switch
}*/
