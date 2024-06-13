#ifndef PIPES_H
#define PIPES_H

#define STDIN "STDIN"
#define STDOUT "STDOUT"
#define STDIN_PIPE 0
#define STDOUT_PIPE 1
#define SIZE 1024
#define PIPES_AMOUNT 16

int initialize_pipes(void);
int open_pipe(char* id);
int close_pipe(int index);
int write_to_pipe(int index, char*string);
char read_pipe(int index);

#endif