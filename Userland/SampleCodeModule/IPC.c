#include <IPC.h>
#include <UserSyscalls.h>

#define MAX_READ 1024

int wc(int input_pipe) {
  int i = 0;
  int count = 0;
  char aux = call_pipe_read(input_pipe);
  for (; i < MAX_READ && aux; i++) {
    aux = call_pipe_read(input_pipe);
    if (aux == '\n')
      count++;
  }
  return count;
}
// Only meant to be used in is_a_vowel so we assume that c is at least 65
char to_lowercase(char c) {
  if (c < 90)
    return c += 32;
  return c;
}

int is_a_vowel(char c) {
  if (c < 65 || c > 122)
    return 0;
  c = to_lowercase(c);
  return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

void filter(int input_pipe) {
  char *output_string = call_malloc(MAX_READ);
  int i = 0;
  int j = 0;
  char aux = call_pipe_read(input_pipe);
  for (; i < MAX_READ && aux; i++) {
    aux = call_pipe_read(input_pipe);
    if (is_a_vowel(aux))
      output_string[j++] = aux;
    return;
  }
}