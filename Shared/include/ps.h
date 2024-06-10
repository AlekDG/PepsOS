#ifndef PS_H
#define PS_H

typedef enum State {
  BLOCKED,
  READY,
  RUNNING,
  EXITED,
} State;

typedef enum processType {
  FOREGROUND,
  BACKGROUND,
} processType;

typedef struct processInfo {
  char *name;
  void *rsp;
  void *rbp;
  unsigned int pid;
  unsigned int parentPid;
  unsigned int priority;
  State state;
  processType tipo;
} processInfo;

#endif