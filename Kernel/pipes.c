// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <defs.h>
#include <lib.h>
#include <pipes.h>
#include <sem.h>

typedef struct {
  int r_indx;
  int w_indx;
  int read_sem;
  char read_sem_name[NAME_LENGTH];
  char buffer[SIZE];
  char name[NAME_LENGTH];
  int proc_count;
} pipe_t;

typedef struct {
  pipe_t pipe;
  int available;
} instance;

int sem_for_pipe_man;
instance pipe_inst[PIPES_AMOUNT];
int get_free_instance();

int create_pipe(char *id) {
    int length = string_length(id);
    if (length <= 0 || length >= NAME_LENGTH - 1)
        return -1;
    int indx;
    if ((indx = get_free_instance()) != -1) {
        pipe_t *new_pipe = &(pipe_inst[indx].pipe);
        string_copy(id, new_pipe->name);
        new_pipe->r_indx = 0;
        new_pipe->w_indx = 0;

        string_copy(id, new_pipe->read_sem_name);
        new_pipe->read_sem_name[length] = 'R';
        new_pipe->read_sem_name[length + 1] = 0;
        int sem_read = create_sem(0, new_pipe->read_sem_name);
        if (sem_read == -1)
            return -1;
        new_pipe->read_sem = sem_read;
    }
    return indx;
}

int initialize_pipes(void) {
  if ((sem_for_pipe_man = create_sem(1, "Pipe Manager")) == -1)
    return -1;
  for (int i = 0; i < PIPES_AMOUNT; i++)
    pipe_inst[i].available = TRUE;
  create_pipe(STDIN);
  create_pipe(STDOUT);
  return 0;
}

int get_pipe(char *name) {
  for (int i = 0; i < PIPES_AMOUNT; i++) {
    if (pipe_inst[i].available == FALSE &&
        same_string(name, pipe_inst[i].pipe.name))
      return i;
  }
  return -1;
}

int get_free_instance() {
  for (int i = 0; i < PIPES_AMOUNT; i++) {
    if (pipe_inst[i].available == TRUE) {
      pipe_inst[i].available = FALSE;
      return i;
    }
  }
  return -1;
}



int open_pipe(char *id) {
  if (sem_wait(sem_for_pipe_man))
    return -1;

  int indx = get_pipe(id);
  if (indx == -1)
    indx = create_pipe(id);
  if (indx == -1) {
    sem_post(sem_for_pipe_man);
    return -1;
  }
  pipe_inst[indx].pipe.proc_count++;
  if (sem_post(sem_for_pipe_man))
    return -1;
  return indx;
}

int valid_index(int indx) {
  return indx < PIPES_AMOUNT && pipe_inst[indx].available == FALSE;
}

int close_pipe(int index) {
  if (!valid_index(index))
    return -1;
  if (sem_wait(sem_for_pipe_man) == -1)
    return -1;

  int close_read = sem_close(pipe_inst[index].pipe.read_sem_name);
  if (close_read == -1)
    return -1;
  pipe_inst[index].available = TRUE;
  if (sem_post(sem_for_pipe_man) == -1)
    return -1;
  return 1;
}

int write_char(int indx, char c) {
  if (!valid_index(indx))
    return -1;
  pipe_t *pipe_to_write = &pipe_inst[indx].pipe;
  pipe_to_write->buffer[pipe_to_write->w_indx % SIZE] = c;
  pipe_to_write->w_indx++;
  if (sem_post(pipe_to_write->read_sem) == -1)
    return -1;
  return 1;
}

int write_to_pipe(int index, char *string) {
  if (!valid_index(index))
    return -1;
  int count = 0;
  while (*string != 0) {
    count++;
    if (write_char(index, *string++) == -1)
      return -1;
  }
  return count;
}

char read_pipe(int index) {
  if (!valid_index(index))
    return -1;
  pipe_t *pipe = &pipe_inst[index].pipe;
  if (sem_wait(pipe->read_sem) == -1)
    return -1;
  char c = pipe->buffer[pipe->r_indx % SIZE];
  pipe->r_indx++;
  return c;
}
