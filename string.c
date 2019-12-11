/**
* Projekt IFJ/IAL 2019 - Překladač imperativního jazyka IFJ19
*
* @file string.c
* @brief Knihovna pro praci s dynamickym stringem
*
* @author Daniel Pátek (xpatek08)
*/

#include "string.h"
#include "error_code.h"

#include <stdlib.h>
#include <string.h>


int stringInit(string *str) {
    // memory allocation
    if ( (str->value = (char*) malloc(STRING_INIT_SIZE * sizeof(char))) == NULL )
        return ERROR_CODE_INTERNAL;
    //inicialize structure
    str->length = 0;
    str->value[0] = '\0';
    str->spaceAlloc = STRING_INIT_SIZE;
    return ERROR_CODE_OK;
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

int stringClear(string *str) {
    for (int i = 0; i < str->spaceAlloc; i++) {
        str->value[i] = '\0';
    }
    str->length = 0;

    return ERROR_CODE_OK;
}

void stringDispose(string *str) {
    free(str->value);
    str->value = NULL;
    str->length = str->spaceAlloc = 0;
}