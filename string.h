//TODO hlavicka


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


//TODO: predpisy funkci
int stringInit(string *str);
int stringAddChar(string *str, char c);
void stringDispose(string *str);
int stringAddChars(string *str, char* c);
#endif
