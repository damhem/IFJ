
#include "parser.h"
#include "stack.h"

ERROR_CODE parse() {
  
  line_flag = true;
  ERROR_CODE result;
  paramIndex = 0;
  peekToken.t_type = TOKEN_UNDEF;  

  stringInit(&functionName);
  stackInit(&s);

  //get first token
  token = getNextToken(&line_flag, &s);
  if (token.t_type == TOKEN_UNDEF) {
    return token.t_data.integer;
  } else {
    result = program();
  }
  

  stringDispose(&functionName);
  stackClear(&s);
  return result;
}

ERROR_CODE skipEol() {
  ERROR_CODE result;
  switch (token.t_type) {
    case TOKEN_EOL:
      //printf("SKIPPING EOL\n");
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      return skipEol();
    default:
      return ERROR_CODE_OK;
  }
  return result;
}

ERROR_CODE program() {
  // Program -> Telo_Programu EOF
  
  ERROR_CODE result = skipEol();
      if (result != ERROR_CODE_OK) return result;

  switch (token.t_type) {
    //case TOKEN_EOL:
    case TOKEN_ID:
    case TOKEN_DEF:
    case TOKEN_WHILE:
    case TOKEN_IF:
    case TOKEN_PASS:
    case TOKEN_LEFTPAR:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_STRING:
    case TOKEN_NONE:
      //get into program body
      result = programBody();
      if (result != ERROR_CODE_OK) return result;

      //there can be some eols
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      //there should be eof
      if (token.t_type == TOKEN_EOF) {
        return ERROR_CODE_OK;
      }
      else {
        return ERROR_CODE_SEM;
      }
    case TOKEN_EOF:
      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

ERROR_CODE programBody() {
  ERROR_CODE result;

  //printf("IN PROGRAM BODY\n");

  switch (token.t_type) {
    
    case TOKEN_DEF:
      //Telo_programu -> Definice_funkce Telo_programu
      result = functionDef();
      if (result != ERROR_CODE_OK) return result;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      return programBody();
    
    case TOKEN_ID:
    case TOKEN_PASS:
    case TOKEN_IF:
    case TOKEN_WHILE:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_STRING:
    case TOKEN_LEFTPAR:
    case TOKEN_NONE:
      //Telo_programu -> Prikaz Telo_programu
      result = command();
      if (result != ERROR_CODE_OK) return result;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      return programBody();


    case TOKEN_EOF:
      //Telo_programu -> eps
      return ERROR_CODE_OK;

    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

ERROR_CODE functionDef() {

  //printf("IN FUNCTION DEF\n");

  ERROR_CODE result;

  switch (token.t_type) {
    
    case TOKEN_DEF:
      //Definice_funkce -> Hlavicka_funkce eol indent Telo_funkce dedent
      result = functionHead();
      if (result != ERROR_CODE_OK) return result;

      //todo zapis instrukce?
      

      //todo symtable shit
      

      //there has to be EOL
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_EOL) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      
      //there has to be indent
      if (token.t_type != TOKEN_INDENT) {
        return ERROR_CODE_SYN;
      }

       if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      //hey there can be a lot of eols izi
      result = skipEol();
      if (result != ERROR_CODE_OK) return result;
      
      //going deeper to function body
      result = functionBody();
      if (result != ERROR_CODE_OK) return result;

      //there has to be dedent
      if (token.t_type != TOKEN_DEDENT) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      return ERROR_CODE_OK;
    
    default:
      return ERROR_CODE_SYN;
    }
  return ERROR_CODE_SYN;
}

ERROR_CODE functionHead() {
  
  ERROR_CODE result;

  //printf("IN FUNCTION HEAD\n");

  switch (token.t_type) {
    
    case TOKEN_DEF:
      //Hlavicka_funkce -> def id ( Parametry ) :
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_ID) {
        return ERROR_CODE_SYN;
      }

      //todo check id of function (maybe its already declared by our program or sth)
      stringAddChars(&functionName, token.t_data.ID.value);

      //there has to be (
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_LEFTPAR) {
        return ERROR_CODE_SYN;
      }
      
      //function parametres
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      result = functionParam();
      if (result != ERROR_CODE_OK) return result;

      //there has to be )
      if (token.t_type != TOKEN_RIGHTPAR) {
        return ERROR_CODE_SYN;
      }

      //there has to be :
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_DOUBLEDOT) {
        return ERROR_CODE_SYN;
      }

      return ERROR_CODE_OK;

    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

ERROR_CODE functionParam() {
  ERROR_CODE result;
  //printf("IN FUNCTION PARAMS\n");

  switch (token.t_type) {
    case TOKEN_ID:

      //Parametry -> id Dalsi_parametr
      //todo vytvořit string na paramname a práce se symtable
      
      paramIndex++;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      result = nextFunctionParam();
      if (result != ERROR_CODE_OK) return result;

      return ERROR_CODE_OK;
    case TOKEN_RIGHTPAR:
      //Parametry -> eps
      paramIndex = 0;
      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

ERROR_CODE nextFunctionParam() {
  //todo? result
  //printf("IN NEXT FUNCTION PARAM\n");

  switch (token.t_type) {
    case TOKEN_COMMA:
      //Dalsi_parametr -> , Parametry
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      return functionParam();
    
    case TOKEN_RIGHTPAR:
      //Dalsi_parametr -> eps
      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

ERROR_CODE functionBody() {
  ERROR_CODE result;
  //printf("IN FUNCTION BODY, token type: %d\n", token.t_type);

  switch (token.t_type) {
    case TOKEN_ID:
    case TOKEN_PASS:
    case TOKEN_WHILE:
    case TOKEN_IF:
    case TOKEN_RETURN:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_NONE:
    case TOKEN_STRING:
    case TOKEN_LEFTPAR:
      //Telo_funkce -> Prikaz Telo_funkce
      //todo lokalni tabulka symbolu?
      
      result = command();
      if (result != ERROR_CODE_OK) return result;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      return functionBody();
    
    case TOKEN_DEDENT:
      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

ERROR_CODE command() {

  //printf("IN COMMAND\n");
  ////printf("token data to command: %s\n", token.t_data.ID.value);

  ERROR_CODE result;

  switch (token.t_type) {
    case TOKEN_IF:
      ////printf("DOING IF\n");
      
      //Prikaz -> if Vyraz : eol indent Sekvence_prikazu dedent else : eol indent Sekvence_prikazu dedent
      //todo vyraz expression

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_DOUBLEDOT) {
        return ERROR_CODE_SYN;
      }
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_EOL) {
        return ERROR_CODE_SYN;
      }
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_INDENT) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      result = commands();
      if (result != ERROR_CODE_OK) {
        return result;
      }

      if (token.t_type != TOKEN_DEDENT) {
        return ERROR_CODE_SYN;
      }
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      if (token.t_type != TOKEN_ELSE) {
        return ERROR_CODE_SYN;
      }
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_DOUBLEDOT) {
        return ERROR_CODE_SYN;
      }
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_EOL) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_INDENT) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      result = commands();
      if (result != ERROR_CODE_OK) {
        return result;
        ////printf("returning ddddwith token: %d\n", token.t_type);
      }
      ////printf("returnsssing with token: %d\n", token.t_type);

      if (token.t_type != TOKEN_DEDENT) {
       // //printf("returning with sstoken: %d\n", token.t_type);
        return ERROR_CODE_SYN;
        
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      ////printf("returning wdddith token: %d\n", token.t_type);
      result = skipEol();
      if (result != ERROR_CODE_OK) return result;
      ////printf("returning with token: %d\n", token.t_type);
      return ERROR_CODE_OK;

    case TOKEN_WHILE:
      //Prikaz -> while Vyraz : eol indent Sekvence_prikazu dedent
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      //todo vyraz here

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_DOUBLEDOT) return ERROR_CODE_SYN;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_EOL) return ERROR_CODE_SYN;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_INDENT) return ERROR_CODE_SYN;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;
      
      result = commands();
      if (result != ERROR_CODE_OK) {
        return result;
      }

      if (token.t_type != TOKEN_DEDENT) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      return ERROR_CODE_OK;

    case TOKEN_PASS:
      //Prikaz -> pass eol
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_EOL) return ERROR_CODE_SYN;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      return ERROR_CODE_OK;

    case TOKEN_RETURN:
      //Prikaz -> return Vyraz eol
      //todo vyraz again lol
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_EOL) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      return ERROR_CODE_OK;

    case TOKEN_ID: ;
      //have to check if its expression
      token_type nextTokenType;
      if ((nextTokenType = peekNextToken()) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      ////printf("SUCCESS CHECK OF NEXT TOKEN, Type: %d\n", nextTokenType);
      //Prikaz -> Vyraz eol

      switch (nextTokenType) {
        case TOKEN_ADDITION:
        case TOKEN_SUBTRACTION:
        case TOKEN_MULTIPLICATION:
        case TOKEN_DIVISION:
        case TOKEN_EQUAL_EQUAL:
        case TOKEN_SMALLERTHEN:
        case TOKEN_SMALLERTHEN_EQUAL:
        case TOKEN_BIGGERTHEN:
        case TOKEN_BIGGERTHEN_EQUAL:
          //has to be operator

          //todo expression napojení (expression(token))
          
          return ERROR_CODE_SYN;
        case TOKEN_EOL:
          //i dont have to care or do I? this
          if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
          if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
          return ERROR_CODE_OK;
      
        case TOKEN_EQUAL:
          //Pokracovani_id -> = Pokracovani_id_next
          //jedna se o prizareni do promenne
          //todo some symtable magic
          if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
          if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

          //todo expression

          return ERROR_CODE_OK;


        case TOKEN_LEFTPAR:
          //just a function call (without actually var to assign to) - procedure
          //return continueID();
          //todo there has to start expression as well
          if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
          return ERROR_CODE_OK;
        default:
          ////printf("default in command: NEXT TOKEN TYPE: %d", nextTokenType);
          return ERROR_CODE_SYN;
      }

    
    case TOKEN_STRING:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_NONE:
    case TOKEN_LEFTPAR:
    
      //this is expression 100%
      //todo Prikaz -> Vyraz eol (vyraz muze byt string, int, float, )
      //todo expression
      //if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_EOL) return ERROR_CODE_SYN;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}


/*
ERROR_CODE continueID() {

  ERROR_CODE result;
  //printf("IN CONTINUE ID\n");

  switch (token.t_type) {
    case TOKEN_ID: ;
      //have to check if its expression
      token_type nextTokenType;
      if ((nextTokenType = peekNextToken()) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      //printf("SUCCESS CHECK OF NEXT TOKEN, Type: %d\n", nextTokenType);

    switch (nextTokenType) {
      case TOKEN_ADDITION:
      case TOKEN_SUBTRACTION:
      case TOKEN_MULTIPLICATION:
      case TOKEN_DIVISION:
      case TOKEN_EQUAL_EQUAL:
      case TOKEN_SMALLERTHEN:
      case TOKEN_SMALLERTHEN_EQUAL:
      case TOKEN_BIGGERTHEN:
      case TOKEN_BIGGERTHEN_EQUAL:
      case TOKEN_EOL:
        //Pokracovani_id_next -> Vyraz eol
        //has to be operator

        //printf("WILL BE DOING EXPRESSION\n");
        //todo expression napojení (expression(token))

        //todo eol token
      
        return ERROR_CODE_SYN;

      case TOKEN_LEFTPAR:
        //Pokracovani_id_next -> id ( Termy ) eol
        if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
        if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
        
        result = terms();
        if (result != ERROR_CODE_OK) return result;

        if (token.t_type != TOKEN_RIGHTPAR) return ERROR_CODE_SYN;

        if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
        if (token.t_type != TOKEN_EOL) return ERROR_CODE_SYN;

        if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
        return ERROR_CODE_OK;

      default:
        return ERROR_CODE_SYN;
      }
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

ERROR_CODE terms() {

  //printf("IN TERMS NOW\n");
  ERROR_CODE result;

  switch (token.t_type) {
    case TOKEN_ID:
      //todo sth with symtable
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      result = nextTerms();
      if (result != ERROR_CODE_OK) return result;

      if (token.t_type != TOKEN_RIGHTPAR) return ERROR_CODE_SYN;

      return ERROR_CODE_OK;

    case TOKEN_STRING:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_NONE:

      //todo sth
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      result = nextTerms();
      if (result != ERROR_CODE_OK) return result;

      if (token.t_type != TOKEN_RIGHTPAR) return ERROR_CODE_SYN;

      return ERROR_CODE_OK;
    case TOKEN_RIGHTPAR:
      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

ERROR_CODE nextTerms() {

  //printf("IN NEXT TERMS:\n");
  //todo? ERROR_CODE result;

  switch (token.t_type) {
    case TOKEN_COMMA:
      //Next_term -> , Termy
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      //todo maybe theres problem with foo(one, two, ) -> now its no mistake (peek maybe?)

      return terms();

    case TOKEN_RIGHTPAR:
      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

*/
ERROR_CODE commands() {

  ERROR_CODE result;
  //printf("IN COMMANDS\n");
  //printf("returning with comm: %d\n", token.t_type);

  switch (token.t_type) {
    case TOKEN_ID:
    case TOKEN_LEFTPAR:
    case TOKEN_IF:
    case TOKEN_WHILE:
    case TOKEN_PASS:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_STRING:
    case TOKEN_NONE:
      //Sekvence_prikazu -> Prikaz Sekvence_prikazu
      result = command();
      if (result != ERROR_CODE_OK) return result;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      return commands();

    case TOKEN_DEDENT:
      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}
