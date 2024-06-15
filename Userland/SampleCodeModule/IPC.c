#include <IPC.h>
#include <io.h>
#include <UserSyscalls.h>

#define MAX_READ 1024

void wc() {
  int i = 0;
  int count = 1;
  char aux = get_char();
  for (; i < MAX_READ && aux; i++) {
    aux = get_char();
    if (aux == '\n')
      count++;
  }
  char text[] = "Text is ";
  print_f(text);
  print_f_int(count);
  char text2[] = " Lines long";
  print_f(text2);
  call_exit();
}
// Only meant to be used in is_a_vowel so we assume that c is at least 65
char to_lowercase(char c) {
  if (c < 90)
    return c += 32;
  return c;
}

int is_a_vowel(char c) {
  if (c < 65 || c > 117)
    return 0;
  c = to_lowercase(c);
  return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

void filter() {
  char *output_string = call_malloc(MAX_READ);
  int i = 0;
  int j = 0;
  char aux = get_char();
  for (; i < MAX_READ && aux; i++) {
    aux = get_char();
    if (is_a_vowel(aux))
      output_string[j++] = aux;
  }
  print_f(output_string);
  call_exit();
}

void cat(){
  char current_letter;
  int cat_active = 1;
  while (cat_active) {
    current_letter = get_char();
    switch (current_letter) {
    case KILL_SIGNAL:
      call_kill(call_getPid());
    case EOF:
      cat_active = 0;
      put_char('\0');
      break;
    default:
      put_char(current_letter);
      break;
    }
  }
  call_exit();
}