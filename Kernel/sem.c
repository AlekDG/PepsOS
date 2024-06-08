#include <interrupts.h>
#include <lib.h>
#include <memMan.h>
#include <scheduler.h>
#include <sem.h>

typedef struct {
  semaphore sem;
  uint64_t availability;
} instance;
static instance sem_inst[SEM_AMOUNT];

int available_sem(void) {
  for (int i = 0; i < SEM_AMOUNT; i++) {
    if (sem_inst[i].availability) {
      sem_inst[i].availability = FALSE;
      return i;
    }
  }
  return -1;
}

void start_sems(void) {
  for (int i = 0; i < SEM_AMOUNT; i++) {
    sem_inst[i].availability = TRUE;
  }
}

uint64_t create_sem(uint64_t val, char *id) {
  int indx = available_sem();
  if (indx == -1 || *id == NULL || string_length(id) >= NAME_LENGTH)
    return -1;
  semaphore *sem = &(sem_inst[indx].sem);
  if (!val)
    sem->lock = 1;
  string_copy(id, sem->name);
  sem->val = val;
  sem->size = 1;
  sem->size_list = 0;
  sem->first_proc = NULL;
  sem->last_proc = NULL;
  return indx;
}

void clear_sem(int index) {
  if (index <= 0 || index >= NAME_LENGTH)
    return;
  while (sem_inst[index].sem.size_list > 0)
    sem_dq_proc(index);
  sem_inst[index].availability = TRUE;
}

uint64_t sem_post(int index) {
  if (index <= 0 || index >= NAME_LENGTH)
    return 1;
  semaphore *sem = &(sem_inst[index].sem);
  if (sem->val = !0)
    enter_region(&(sem->lock), index);
  sem->val++;
  if (sem->size_list > 0) {
    int pid = sem_dq_proc(index);
    unblock(pid);
  }
  leave_region(&(sem->lock));
  return 0;
}

uint64_t sem_wait(int index) {
  if (index <= 0 || index >= NAME_LENGTH)
    return;
  semaphore *sem = &(sem_inst[index].sem);
  enter_region(&(sem->lock), index);
  sem_inst[index].sem.val--;
  if (sem_inst[index].sem.val != 0) {
    leave_region(&(sem->lock));
  }
  return 0;
}

int sem_dq_proc(int indx) {
  instance *sem_instance = &sem_inst[indx];
  if (sem_instance->availability)
    return -1;
  semaphore *sem = &(sem_instance->sem);
  sem_proc *current_process = sem->first_proc;
  if (current_process == NULL)
    return -1;

  int pid = current_process->pid;
  sem->first_proc = current_process->next;
  if (sem->first_proc == NULL)
    sem->last_proc = NULL;
  sem->size_list--;
  freeMemory(current_process);

  return pid;
}

int sem_nq_proc(int index, int pid) {
  instance *sem_instance = &sem_inst[index];
  if (sem_instance->availability)
    return -1;
  semaphore *sem = &(sem_instance->sem);
  sem_proc *new_proc = allocMemory(sizeof(sem_proc));
  if (new_proc == NULL)
    return -1;

  new_proc->pid = pid;
  new_proc->next = NULL;
  if (sem->size_list == 0) {
    sem->first_proc = new_proc;
    sem->last_proc = new_proc;
  } else {
    sem->last_proc->next = new_proc;
    sem->last_proc = new_proc;
  }
  sem->size_list++;

  return 0;
}

int get_sem_indx(char *id) {
  for (int i = 0; i < SEM_AMOUNT; i++) {
    if (sem_inst[i].availability == FALSE &&
        same_string(id, sem_inst[i].sem.name))
      return i;
  }
  return -1;
}

int sem_open(int val, char *id) {
  int indx;
  if ((indx = get_sem_indx(id)) == -1) {
    indx = create_sem(val, id);
  }
  return indx;
}

int sem_close(char *id) {
  int indx = get_sem_indx(id);
  if (indx == -1)
    return -1;
  clear_sem(indx);
  return 0;
}

void sem_miss(uint64_t index) {
  int current_pid = getPid();
  if (current_pid != -1) {
    sem_nq_proc(index, current_pid);
  }
  block(current_pid);
}