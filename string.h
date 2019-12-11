/**
* Projekt IFJ/IAL 2019 - Překladač imperativního jazyka IFJ19
*
* @file string.h
*
* @author Daniel Pátek (xpatek08)
*/

#ifndef IFJ_STRING_H
#define IFJ_STRING_H

#include <stdbool.h>

#define STRING_INIT_SIZE 8

//string structure
typedef struct {
    char *value; // the string
    int length; // length of string
    int spaceAlloc; // length of allocated space
} string;

//function declaration
int stringInit(string *str);
int stringAddChar(string *str, char c);
void stringDispose(string *str);
int stringAddChars(string *str, char* c);
int stringClear(string *str);
#endif
