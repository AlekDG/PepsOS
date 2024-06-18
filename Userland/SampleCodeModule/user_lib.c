// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <user_lib.h>

void copyString(char *from, char *to) {
  int i = 0;
  for (; from[i]; i++) {
    to[i] = from[i];
  }
  to[i] = from[i];
}

int compareStrings(char *s1, char *s2) {
  while (*s1 != 0 && *s2 != 0) {
    char first = *s1;
    char last = *s2;
    if (first > last) {
      return 1;
    } else if (first < last) {
      return -1;
    }
    s1++;
    s2++;
  }
  if (*s1 != 0 && *s2 == 0) {
    return 1;
  }
  if (*s1 == 0 && *s2 != 0) {
    return -1;
  }
  return 0;
}

void stringTokenizer(char *string, char *s1, char *s2, char token) {
  if (token == NULL)
    return;
  int indx;
  for (indx = 0; string[indx] && string[indx] != token; indx++)
    ;
  if (string[indx] == NULL)
    return;
  string[indx++] = NULL;
  for (int i = 0; string[indx] && string[indx] != token && i < ARG_LENGTH;
       indx++)
    s1[i++] = string[indx];
  if (string[indx] == NULL)
    return;
  string[indx++] = NULL;
  for (int j = 0; string[indx] && string[indx] != token && j < ARG_LENGTH;
       indx++)
    s2[j++] = string[indx];
  string[indx] = NULL;
  return;
}

void stringCopyNaive(char *from,char *to){
  int i=0;
  while(from[i]){
    to[i]=from[i];
    i++;
  }
}

int strLen(const char *str) {
  int len = 0;
  while (str[len] != '\0') {
    len++;
  }
  return len;
}

int hexCharToInt(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  } else if (c >= 'a' && c <= 'f') {
    return c - 'a' + 10;
  } else if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  } else {
    return -1;
  }
}

unsigned long long hexToULL(const char *hexStr) {
  unsigned long long result = 0;
  int len = strLen(hexStr);

  for (int i = 0; i < len; i++) {
    int value = hexCharToInt(hexStr[i]);
    if (value == -1) {
      return -1;
    }
    result = (result << 4) | value;
  }

  return result;
}

int64_t satoi(char *str) {
  uint64_t i = 0;
  int64_t res = 0;
  int8_t sign = 1;

  if (!str)
    return 0;

  if (str[i] == '-') {
    i++;
    sign = -1;
  }

  for (; str[i] != '\0'; ++i) {
    if (str[i] < '0' || str[i] > '9')
      return 0;
    res = res * 10 + str[i] - '0';
  }

  return res * sign;
}