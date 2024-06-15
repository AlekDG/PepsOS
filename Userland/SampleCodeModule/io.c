#include <io.h>
#include <UserSyscalls.h>
#include <user_lib.h>

void print_error(char* string){
    call_drawStringFormatted(string,RED,BLACK,2);
}

void print_f(char* string){
    call_system_write(string);
}

void print_f_int(int num) {
  char buffer[5];
  char buffer2[5];
  int index = 0;

  while (index < 5) {
    int temp = num % 10;
    buffer[index] = temp + '0';
    num = num / 10;
    index++;
  }
  int j=0;
  // Reverse the hexadecimal array
  for (int i = index - 1; i >= 0; i--)
    buffer2[j++]=buffer[i];
  call_system_write(buffer2);
}

char get_char(){
  char c;

  call_system_read(&c,1);

  return c;
}

void put_char(char c){
  char aux[2];
  aux[0]=c;
  aux[1]=0;
  call_system_write(aux);
}