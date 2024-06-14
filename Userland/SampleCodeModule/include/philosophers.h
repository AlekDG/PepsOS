#ifndef ARQUITPE_PHILOSOPHERS_H
#define ARQUITPE_PHILOSOPHERS_H
#include <stdint.h>
#include <UserSyscalls.h>

int my_atoi(const char *str);
char* my_itoa(int num, char* str, int base);
void reverse(char* str, int length);

void think(int i);
void eat(int i);
void testPhylo(int i);
void take_forks(int i);
void put_forks(int i);
void philosopher(int argc, char **argv);


void initializePhilosophers();
void start_philosophers();
void render_philosophers();
void run_Philosophers(int argc, char **argv);


#endif
