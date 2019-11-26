/*
  @brief Working with string structures.
  @file string.c

... ?
*/

//TODO: komenty všude↓

#include "string.h"
#include "error_code.h"

#include <stdlib.h>
#include <string.h>




int stringInit(string *str) {

    // memory allocation
    if ( (str->value = (char*) malloc(STRING_INIT_SIZE * sizeof(char))) == NULL )
        return ERROR_CODE_INTERNAL;

    str->length = 0;
    str->value[0] = '\0';
    str->spaceAlloc = STRING_INIT_SIZE;

    return ERROR_CODE_OK;
}



int stringLength(string *str){
  return strlen(str->value);
}

bool stringCompare(string *str1, string *str2) {
  if ( strcmp(str1->value, str2->value) == 0 )
        return true;
    else
        return false;
}

int stringAddChar(string *str, char c) {

    // if the string is full - it needs to be reallocated
    if (str->length + 1 >= str->spaceAlloc) {
        if ((str->value = (char *) realloc(str->value, str->spaceAlloc + STRING_INIT_SIZE * sizeof(char))) == NULL)
            return ERROR_CODE_INTERNAL;
        str->spaceAlloc += STRING_INIT_SIZE;
    }

    // adding char to the end of string
    str->value[str->length] = c;
    str->value[str->length+1] = '\0';
    str->length++;

    return ERROR_CODE_OK;
}


int stringAddChars(string *str, char* c) {
    int result_code;
    for (unsigned int i = 0; i < strlen(c); i++) {
        result_code = stringAddChar(str, c[i]);
    }
    return result_code;
}


void stringUpdateLastChar(string *str, char c) {
    if (str->length > 0) {
        str->value[str->length-1] = c;
    }
}

void stringUpdateCharIndex(string *str, char c, int index) {
    if ((str->length > 0) && (str->length >= index)) {
        str->value[index - 1] = c;
    }
}


int stringAddFirstChar(string * str, char c) {

    if (str->length + 1 >= str->spaceAlloc) {
        if ((str->value = (char *) realloc(str->value, str->spaceAlloc + STRING_INIT_SIZE * sizeof(char))) == NULL)
            return ERROR_CODE_INTERNAL;
        str->spaceAlloc += STRING_INIT_SIZE;
    }

    for (int i = str->length; i >= 0; i--) {
        str->value[i + 1] = str->value[i];
    }

    str->value[0] = c;
    str->length++;

    return ERROR_CODE_OK;
}


int stringClear(string *str) {
    for (int i = 0; i < str->spaceAlloc; i++) {
        str->value[i] = '\0';
    }
    str->length = 0;

    return ERROR_CODE_OK;
}

char stringGetLastChar(string *str) {
    if (str->length > 0)
        return str->value[str->length - 1];
    else
        return -1;
}

char stringGetCharIndex(string *str, int index) {
    if ((str->length > 0) && (str->length >= index))
        return str->value[index - 1];
    else
        return -1;
}


int stringIsKeyWord(string *str) {
    char *keywords[] = {"def\0", "else\0", "if\0", "None\0", "pass\0", "return\0", "while\0"};

    unsigned int keywordsLength = 7;

    // porovnani tokenu s klicovymi slovy
    for (unsigned int i = 0; i < keywordsLength; i++) {
        if ( strcmp(keywords[i], str->value) == 0 )
            return ERROR_CODE_TRUE;
    }
    return ERROR_CODE_FALSE;
}


void stringDispose(string *str) {
    free(str->value);
    str->value = NULL;
    str->length = str->spaceAlloc = 0;
}




void charUndo(char c) {
    ungetc(c, stdin);
}


int charIsSpace(char c) {
    if (c == ' ')
        return ERROR_CODE_TRUE;
    else
        return ERROR_CODE_FALSE;
}


int charIsNewLine(char c) {
    if (c == '\n')
        return ERROR_CODE_TRUE;
    else
        return ERROR_CODE_FALSE;
}

int charIsTab(char c) {
    if ( c == '\t' )
        return ERROR_CODE_TRUE;
    else
        return ERROR_CODE_FALSE;
}


int charIsDigit(char c) {
    if (c >= '0' && c <= '9')
        return ERROR_CODE_TRUE;
    else
        return ERROR_CODE_FALSE;
}


int charIsLetter(char c) {
    if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') )
        return ERROR_CODE_TRUE;
    else
        return ERROR_CODE_FALSE;
}


int charToDec(char c) {
    return c - 48;
}


char decToChar(int c) {
    return c + 48;
}
