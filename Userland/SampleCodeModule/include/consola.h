#ifndef CONSOLE_H
#define CONSOLE_H

#define RED 0xFF0000
#define ENLARGE_FONT_SIZE_CMD "increasefont"
#define REDUCE_FONT_SIZE_CMD "reducefont"
#define TEST_DIV_ZERO_CMD "testdivzero"
#define TEST_INVALID_OPCODE_CMD "testinvalidopcode"
#define PRINT_HELP_CMD "help"
#define MEM_STATE_CMD "mem"
#define PROCESS_STATE_CMD "ps"
#define PROCESS_LOOP_CMD "loop"
#define PROCESS_KILL_CMD "kill"
#define PROCESS_NICE_CMD "nice"
#define PROCESS_BLOCK_CMD "block"
#define IPC_CAT_CMD "cat"
#define IPC_LINE_COUNT_CMD "wc"
#define IPC_FILTER_VOWELS_CMD "filter"
#define IPC_PHYLO_CMD "phylo"

void drawConsole();
void runConsole();
void deleteConsole();
void ps();
void loop();

typedef enum {
  CMD_ENLARGE_FONT_SIZE,
  CMD_DECREASE_FONT_SIZE,
  CMD_TEST_DIV_ZERO,
  CMD_TEST_INV_OPCODE,
  CMD_PRINT_HELP,
  CMD_PRINT_MEM_STATE,
  CMD_PROCESSES_STATE,
  CMD_PROCESS_LOOP,
  CMD_PROCESS_KILL,
  CMD_PROCESS_NICE,
  CMD_PROCESS_BLOCK_TOGGLE,
  CMD_IPC_CAT,
  CMD_IPC_LINE_COUNT,
  CMD_IPC_FILTER_VOWELS,
  CMD_IPC_PHYLO,
  CMD_UNKNOWN
} CommandType;
#endif
