#include <io.h>
#include <UserSyscalls.h>
#include <user_lib.h>

void print_error(char* string){
    call_drawStringFormatted(string,RED,BLACK,2);
}

void print_f(char* string){
    call_system_write(string);
}

char get_char(){
  char c;

  call_system_read(&c,1);

  return c;
}