#ifndef IPC_H
#define IPC_H
#define STDIN 0
#define STDOUT 1

void cat(int input_pipe);
int wc(int input_pipe);
void filter(int input_pipe);

#endif