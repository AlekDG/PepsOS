#ifndef SEM_H
#define SEM_H

#include <stdint.h>

#define NAME_LENGTH 16
#define SEM_AMOUNT 128

typedef struct procNode {
  struct procNode *next;
  uint64_t pid;
} sem_proc;

typedef struct {
  char name[NAME_LENGTH];
  sem_proc *first_proc;
  sem_proc *last_proc;
  uint64_t lock;
  int val;
  uint64_t size;
  uint64_t size_list;
} semaphore;

void start_sems(void);
// Save the return value to use as an argument for post and wait
uint64_t create_sem(uint64_t val, char *id);
uint64_t sem_post(int index);
uint64_t sem_wait(int index);
int sem_open(int val, char *id);
int sem_close(char *id);
int get_sem_indx(char *id);

#endif