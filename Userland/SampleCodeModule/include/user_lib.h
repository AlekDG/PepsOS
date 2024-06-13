#ifndef USER_LIB_H
#define USER_LIB_H
#include <stdint.h>

#define NULL 0
#define FALSE 0
#define TRUE 1
#define ARG_LENGTH 20

int compareStrings(char *s1,char *s2);
//Breaks up String into String-s1-s2 based on the token
void stringTokenizer(char* string,char* s1,char* s2, char token);
int strLen(const char *str);
int hextCharToInt(char c);
unsigned long long hexToULL(const char *hexStr);
int64_t satoi(char *str);

#endif