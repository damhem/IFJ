
#include "parser.h"



ERROR_CODE parse() {

  ERROR_CODE result;

  line_flag = true;
  inFunctionflag = false;
  paramIndex = 0;
  peekToken.t_type = TOKEN_UNDEF;

  //initialize structures
  if (stackInit(&s) == ERROR_CODE_INTERNAL) {
    return ERROR_CODE_INTERNAL;
  }
  if (symTableInit(&glSymtable) == ERROR_CODE_INTERNAL) {
    return ERROR_CODE_INTERNAL;
  }
  exp_stackInit(&stack_expression);
  stringInit(&functionName);

  //get first token
  token = getNextToken(&line_flag, &s);
  if (token.t_type == TOKEN_UNDEF) {
    return token.t_data.integer;
  } else {
    result = program();
  }

  //free memory
  stringDispose(&functionName);
  exp_stackClear(&stack_expression);
  stackClear(&s);
  symTableDispose(&glSymtable);
  symTableDispose(&lcSymtable);

  return result;
}

ERROR_CODE skipEol() {
  ERROR_CODE result;
  switch (token.t_type) {
    case TOKEN_EOL:
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
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
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
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
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_EOL) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      //there has to be indent
      if (token.t_type != TOKEN_INDENT) {
        return ERROR_CODE_SYN;
      }

       if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      //hey there can be a lot of eols izi
      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      //create local symtable
      if (symTableInit(&lcSymtable) == ERROR_CODE_INTERNAL) {
        return ERROR_CODE_INTERNAL;
      }

      //set global symtable to pointer
      tBSTNodePtr helper = symTableSearch(&glSymtable, functionName);
      if (helper == NULL) {
        return ERROR_CODE_SEM_OTHER;
      }
      else {
        //now we have to add paramnames to local symtable
        for (int i = 0; i < helper->parametrs; i++) {
          symTableInsert(&lcSymtable, helper->paramName[i], false);
        }
      }

      //going deeper to function body
      result = functionBody();
      if (result != ERROR_CODE_OK) return result;

      //there has to be dedent
      if (token.t_type != TOKEN_DEDENT) {
        return ERROR_CODE_SYN;
      }

      //dispose function name
      stringClear(&functionName);

      //clear paramIndex
      paramIndex = 0;
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
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
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_ID) {
        return ERROR_CODE_SYN;
      }

      // check id of function (maybe its already declared by our program or sth)
      stringAddChars(&functionName, token.t_data.ID.value);
      tBSTNodePtr helper = symTableSearch(&glSymtable, functionName);
      if (helper != NULL) {
        //oooo lol its already declared
        //have to check if its not variable
        if (helper->DataType == Variable) {
          fprintf(stderr, "Uz byla vytvorena promenna s nazvem: %s", functionName.value);
          return ERROR_CODE_SEM;
        }
        //when function has been declared, but not defined
        else if (strcmp(helper->Key.value, functionName.value) == 0 && (helper->defined == false)) {
          result = symTableInsert(&glSymtable, functionName, true);
          if (result != ERROR_CODE_OK) return result;
          helper->defined = true;

          //todo check if the number of parametres match
        }
        else {
          //there is this function already
          fprintf(stderr, "Uz byla vytvorena funkce s jmenem: %s\n", functionName.value);
          return ERROR_CODE_SEM;
        }
      }
      else {
        //this is new entry to the function
        result = symTableInsert(&glSymtable, functionName, true);
        if (result != ERROR_CODE_OK) return result;
        if ((helper = symTableSearch(&glSymtable, functionName)) == NULL) {
          return ERROR_CODE_SEM_OTHER;
        }
        else {
          //add parametres
          helper->defined = true;
          helper->DataType = Function;
          helper->parametrs = 0;
        }
      }


      //there has to be (
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_LEFTPAR) {
        return ERROR_CODE_SYN;
      }

      //function parametres
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = functionParam();
      if (result != ERROR_CODE_OK) return result;

      //there has to be )
      if (token.t_type != TOKEN_RIGHTPAR) {
        return ERROR_CODE_SYN;
      }

      //there has to be :
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
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
    case TOKEN_ID: ;
      //Parametry -> id Dalsi_parametr

      string Paramname;
      stringInit(&Paramname);
      stringAddChars(&Paramname, token.t_data.ID.value);

      tBSTNodePtr helper = symTableSearch(&glSymtable, Paramname);
      if (helper != NULL) {
        //there has been function with the same ID
        fprintf(stderr, "Nemuzete mit stejny nazev funkce/promenne a nazev parametru funkce: %s\n", Paramname.value);
        return ERROR_CODE_SEM;
      }

      helper = symTableSearch(&glSymtable, functionName);

      if (helper != NULL) {
        //means that function is in symtable
        //have to check previous paramentres (might have the same name)
        for (int i = 0; i < paramIndex; i++) {
          if (strcmp(helper->paramName[i].value, Paramname.value) == 0) {
            fprintf(stderr, "Uz mate definovany parametr: %s\n", Paramname.value);
            return ERROR_CODE_SEM;
          }
        }

        //now we can actually insert a parameter
        stringInit(&(helper->paramName[paramIndex]));
        stringAddChars(&(helper->paramName[paramIndex]), Paramname.value);

        //increment number of function paramentres
        (helper->parametrs)++;

        //increment the index
        paramIndex++;

      }
      else {
        return ERROR_CODE_SEM_OTHER;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = nextFunctionParam();
      if (result != ERROR_CODE_OK) return result;

      return ERROR_CODE_OK;

    case TOKEN_RIGHTPAR:
      //Parametry -> eps
      return ERROR_CODE_OK;

    default:
      return ERROR_CODE_SYN;

  }
  return ERROR_CODE_SYN;
}

ERROR_CODE nextFunctionParam() {
  //todo? result

  switch (token.t_type) {
    case TOKEN_COMMA:
      //Dalsi_parametr -> , Parametry
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      if (token.t_type == TOKEN_RIGHTPAR) return ERROR_CODE_SYN;

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

      //todo? some var infunction

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

  ERROR_CODE result;

  switch (token.t_type) {
    case TOKEN_IF:
      //Prikaz -> if Vyraz : eol indent Sekvence_prikazu dedent else : eol indent Sekvence_prikazu dedent
      //todo vyraz expression

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_DOUBLEDOT) {
        return ERROR_CODE_SYN;
      }
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_EOL) {
        return ERROR_CODE_SYN;
      }
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_INDENT) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      result = commands();
      if (result != ERROR_CODE_OK) {
        return result;
      }

      if (token.t_type != TOKEN_DEDENT) {
        return ERROR_CODE_SYN;
      }
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      if (token.t_type != TOKEN_ELSE) {
        return ERROR_CODE_SYN;
      }
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_DOUBLEDOT) {
        return ERROR_CODE_SYN;
      }
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_EOL) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_INDENT) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

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

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      ////printf("returning wdddith token: %d\n", token.t_type);
      result = skipEol();
      if (result != ERROR_CODE_OK) return result;
      ////printf("returning with token: %d\n", token.t_type);
      return ERROR_CODE_OK;

    case TOKEN_WHILE:
      //Prikaz -> while Vyraz : eol indent Sekvence_prikazu dedent
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      //todo vyraz here

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_DOUBLEDOT) return ERROR_CODE_SYN;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_EOL) return ERROR_CODE_SYN;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_INDENT) return ERROR_CODE_SYN;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      result = commands();
      if (result != ERROR_CODE_OK) {
        return result;
      }

      if (token.t_type != TOKEN_DEDENT) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      return ERROR_CODE_OK;

    case TOKEN_PASS:
      //Prikaz -> pass eol
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_EOL) return ERROR_CODE_SYN;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      return ERROR_CODE_OK;

    case TOKEN_RETURN:
      //Prikaz -> return Vyraz eol
      //todo vyraz again lol
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_EOL) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      return ERROR_CODE_OK;

    case TOKEN_ID: ;
      //have to check if its expression
      token_type nextTokenType;
      if ((nextTokenType = peekNextToken()) == TOKEN_UNDEF) return ERROR_CODE_LEX;

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
          if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
          if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
          return ERROR_CODE_OK;

        case TOKEN_EQUAL:
          //Pokracovani_id -> = Pokracovani_id_next
          //jedna se o prizareni do promenne

          //have to update symtable
          if (functionName.length == 0) {
            //now im in main program -> looking into global symtable
            //make sure that this var is not in table
            if (symTableSearch(&glSymtable, token.t_data.ID) == NULL) {
              symTableInsert(&glSymtable, token.t_data.ID, false);
            }
          }
          else {
            //now im in function -> looking into local symtable
            //make sure that this var is not in the table already
            if (symTableSearch(&lcSymtable, token.t_data.ID) == NULL) {
              symTableInsert(&lcSymtable, token.t_data.ID, false);
            }
          }

          //todo? some expected value?

          //take the = from stdin
          if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

          //now we are sure that there has to be expression after =
          if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

          //todo expression

          return ERROR_CODE_OK;


        case TOKEN_LEFTPAR:
          //just a function call (without actually var to assign to) - procedure


          //todo there has to start expression as well
          if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
          return ERROR_CODE_OK;
        default:
          return ERROR_CODE_SYN;
      }

    case TOKEN_STRING:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_NONE:
    case TOKEN_LEFTPAR:

      // Prikaz -> Vyraz eol (vyraz muze byt string, int, float, )

      //wtf
      //can we do sth with this?
      //while (token.t_type != TOKEN_EOL) {
      //  if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      //}


      result = expression();
      //if (result != ERROR_CODE_OK) return result;

      if (token.t_type != TOKEN_EOL) return ERROR_CODE_SYN;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

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
        if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
        if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

        result = terms();
        if (result != ERROR_CODE_OK) return result;

        if (token.t_type != TOKEN_RIGHTPAR) return ERROR_CODE_SYN;

        if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
        if (token.t_type != TOKEN_EOL) return ERROR_CODE_SYN;

        if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
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
  return ERROR_CODE_SYN;
}

ERROR_CODE nextTerms() {

  //printf("IN NEXT TERMS:\n");
  //todo? ERROR_CODE result;

  switch (token.t_type) {
    case TOKEN_COMMA:
      //Next_term -> , Termy
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
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
