#include <IPC.h>
#include <io.h>
#include <UserSyscalls.h>

#define MAX_READ 1024

void wc() {
  int count = 1;
  char aux = get_char();
  for (int i = 0; aux != EOF && aux; i++) {
    if (aux == '\n'){
      char text[] = "Text is ";
      print_f(text);
      print_f_int(count++);
      char text2[] = " Lines long\n";
      print_f(text2);
      }
    aux = get_char();
  }
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
  char aux = get_char();
  for (int i = 0; aux && aux!=EOF; i++) {
    if (is_a_vowel(aux))
      put_char(aux);;
    aux = get_char();
  }
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
    default:
      put_char(current_letter);
      break;
    }
  }
  call_exit();
}