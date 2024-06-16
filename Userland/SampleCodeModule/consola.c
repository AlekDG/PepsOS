#include <IPC.h>
#include <UserSyscalls.h>
#include <consola.h>
#include <io.h>
#include <menu.h>
#include <philosophers.h>
#include <test.h>
#include <user_lib.h>

void deleteConsole();
void interpretCommand(char command[]);

void runConsole() {
  call_set_color(DARK_GRAY, LIGHT_GRAY);
  call_clear();
  char internalBuffer[50] = {0}; // tamaño maximo de 50 chars
  int bufferSize = 0;
  int consoleRunning = 1; // flag de corte de ejecucion
  char currentLetter;

  while (consoleRunning) {
    currentLetter = get_char();
    switch (currentLetter) {
    case '\n':
      interpretCommand(internalBuffer);
      call_clear();
      while (bufferSize > 0) {
        internalBuffer[bufferSize - 1] = 0;
        bufferSize--;
      }
      call_setXBuffer(0);
      call_setYBuffer(10);
      break;
    case 0: // omite teclas no asignadas
      break;
    case 27: // current letter=esc
      call_exit();
    case '\b': // borrado de caracter
      if (bufferSize <= 0) {
        bufferSize = 0;
        break;
      }
      call_deleteLetterBuffered();
      internalBuffer[bufferSize--] =
          0; // borra el ultimo caracter y reduce el tamaño del buffer
      break;
    default:
      if (bufferSize >= 50) {
        break;
      }
      call_drawLetterFromChar(currentLetter);
      internalBuffer[bufferSize++] =
          currentLetter; // se guarda la letra escrita
      break;
    }
  }
}

CommandType getCommandType(const char *command, char *arg1, char *arg2);
void askForAnyletter();
void enlargeFontSize();
void decreaseFontSize();
void printHelp();
void printMemState();
int bgFlag(char *arg);
int isPipe(char *arg);
void *getFunctionPtr(CommandType cmd, char *argv) {
  switch (cmd) {
  case CMD_IPC_CAT:
    stringCopyNaive("cat", argv);
    return cat;
  case CMD_IPC_FILTER_VOWELS:
    stringCopyNaive("flt", argv);
    return filter;
  case CMD_IPC_LINE_COUNT:
    stringCopyNaive("wc", argv);
    return wc;
  default:
    return NULL;
  }
}

void interpretCommand(char command[]) {
  char arg1[ARG_LENGTH] = {0};
  char arg2[ARG_LENGTH] = {0};
  CommandType cmdType = getCommandType(command, arg1, arg2);
  switch (cmdType) {
  case CMD_PIPED_PAIR:
    if (arg2 == NULL)
      return;
    CommandType cmd1 = getCommandType(command, NULL, NULL);
    CommandType cmd2 = getCommandType(arg2, NULL, NULL);
    char argc[4] = {0};
    char arg22[4] = {0};
    char *argv1[] = {argc};
    char *argv2[] = {arg22};
    void *func1 = getFunctionPtr(cmd1, argc);
    void *func2 = getFunctionPtr(cmd2, arg22);
    if (!(func1 == NULL || func2 == NULL)) {
      int shared_pipe = call_pipe_open("NewPipe");
      int fd1[] = {STDIN, shared_pipe};
      int fd2[] = {shared_pipe, STDOUT};
      int *fds[] = {fd1, fd2};
      int argc[] = {0, 0};
      char **argv[] = {argv1, argv2};
      call_createProcessesWithPipe(func1, func2, argc, argv, fds);
    }
    return;
  case CMD_ENLARGE_FONT_SIZE:
    if (call_getSize() < 4) {
      enlargeFontSize();
    } else {
      call_setSize(4);
    }
    askForAnyletter();
    call_pipe_read(STDIN);
    return;
  case CMD_DECREASE_FONT_SIZE:
    if (call_getSize() > 1) {
      decreaseFontSize();
    } else {
      call_setSize(1);
    }
    askForAnyletter();
    call_pipe_read(STDIN);
    return;
  case CMD_TEST_DIV_ZERO:
    call_excepDivZero();
    return;
  case CMD_TEST_INV_OPCODE:
    call_excepInvalidOp();
    return;
  case CMD_PRINT_HELP:
    printHelp();
    askForAnyletter();
    call_pipe_read(STDIN);
    return;
  case CMD_PRINT_MEM_STATE:
    printMemState();
    askForAnyletter();
    call_pipe_read(STDIN);
    return;
  case CMD_PROCESS_LOOP:
    char *loopArgv[] = {"loop"};
    call_createBackgroundProcess(loop, 0, loopArgv, 0, NULL);
    break;
  case CMD_PROCESSES_STATE:
    char *psArgv[] = {"ps"};
    if (bgFlag(arg1))
      call_createBackgroundProcess(ps, 0, psArgv, 3, NULL);
    else
      call_createForegroundProcess(ps, 0, psArgv, 4, NULL);
    break;
  case CMD_PROCESS_KILL:
    int wasKilled = call_kill(satoi(arg1));
    if (wasKilled) {
      call_drawStringFormatted("\nProcess killed", RED, LIGHT_GRAY, 2);
    } else {
      print_f("\nProcess not killed");
    }
    call_pipe_read(STDIN);
    return;
    break;
  case CMD_PROCESS_NICE:
    int wasChanged = call_changePriority(satoi(arg1), satoi(arg2));
    if (wasChanged) {
      call_drawStringFormatted("\nProcess priority changed", GREEN, LIGHT_GRAY,
                               2);
    } else {
      call_drawStringFormatted("\nProcess priority not changed", BLACK,
                               LIGHT_GRAY, 2);
    }
    call_pipe_read(STDIN);
    return;
    break;
  case CMD_PROCESS_BLOCK_TOGGLE:
    int wasBlock = call_block(satoi(arg1));
    if (!wasBlock) {
      call_unblock(satoi(arg1));
    }
    call_pipe_read(STDIN);
    return;
  case CMD_IPC_CAT:
    char *catArgv[] = {"cat"};
    call_clear();
    if (bgFlag(arg1))
      call_createBackgroundProcess(cat, 0, catArgv, 3, NULL);
    else
      call_createForegroundProcess(cat, 0, catArgv, 4, NULL);
    return;
  case CMD_IPC_PHYLO:
    char *philoArgv[] = {"phylo"};
    if (bgFlag(arg1))
      call_createBackgroundProcess(run_Philosophers, 0, philoArgv, 3, NULL);
    else
      call_createForegroundProcess(run_Philosophers, 0, philoArgv, 4, NULL);
    return;
  case CMD_TEST_MM:
    char *mmArgv[] = {"testmm", arg1};
    if (bgFlag(arg1))
      call_createBackgroundProcess(test_mm, 1, mmArgv, 3, NULL);
    else
      call_createForegroundProcess(test_mm, 1, mmArgv, 4, NULL);
    return;
  case CMD_TEST_SCH:
    char *schArgv[] = {"testsch"};
    if (bgFlag(arg1))
      call_createBackgroundProcess(test, 0, schArgv, 4, NULL);
    else
      call_createForegroundProcess(test, 0, schArgv, 4, NULL);
    return;

  case CMD_UNKNOWN:
  default:
    break;
  }
}

int bgFlag(char *arg) { return arg[0] == '&' && arg[1] == NULL; }

int isPipe(char *arg) { return arg[0] == '|' && arg[1] == NULL; }

int isBlockCommand(char *command, char *arg1) {
  if (compareStrings(command, "block") != 0)
    return FALSE;
  if (!satoi(arg1))
    return FALSE;
  return TRUE;
}

int isKillCommand(char *command, char *arg1) {
  if (compareStrings(command, "kill") != 0)
    return FALSE;
  if (!satoi(arg1))
    return FALSE;
  return TRUE;
}

int isNiceComand(char *command, char *arg1, char *arg2) {
  if (compareStrings(command, "nice") != 0)
    return FALSE;
  if (!satoi(arg1))
    return FALSE;
  if (!satoi(arg2))
    return FALSE;
  return TRUE;
}

CommandType getCommandType(const char *command, char *arg1, char *arg2) {
  stringTokenizer(command, arg1, arg2, ' ');
  if (isPipe(arg1)) {
    return CMD_PIPED_PAIR;
  } else if (isKillCommand(command, arg1)) {
    return CMD_PROCESS_KILL;
  } else if (isNiceComand(command, arg1, arg2)) {
    return CMD_PROCESS_NICE;
  } else if (isBlockCommand(command, arg1)) {
    return CMD_PROCESS_BLOCK_TOGGLE;
  } else if (compareStrings(command, ENLARGE_FONT_SIZE_CMD) == 0) {
    return CMD_ENLARGE_FONT_SIZE;
  } else if (compareStrings(command, REDUCE_FONT_SIZE_CMD) == 0) {
    return CMD_DECREASE_FONT_SIZE;
  } else if (compareStrings(command, TEST_DIV_ZERO_CMD) == 0) {
    return CMD_TEST_DIV_ZERO;
  } else if (compareStrings(command, TEST_INVALID_OPCODE_CMD) == 0) {
    return CMD_TEST_INV_OPCODE;
  } else if (compareStrings(command, PRINT_HELP_CMD) == 0) {
    return CMD_PRINT_HELP;
  } else if (compareStrings(command, MEM_STATE_CMD) == 0) {
    return CMD_PRINT_MEM_STATE;
  } else if (compareStrings(command, PROCESS_STATE_CMD) == 0) {
    return CMD_PROCESSES_STATE;
  } else if (compareStrings(command, PROCESS_LOOP_CMD) == 0) {
    return CMD_PROCESS_LOOP;
  } else if (compareStrings(command, PROCESS_KILL_CMD) == 0) {
    return CMD_PROCESS_KILL;
  } else if (compareStrings(command, PROCESS_NICE_CMD) == 0) {
    return CMD_PROCESS_NICE;
  } else if (compareStrings(command, PROCESS_BLOCK_CMD) == 0) {
    return CMD_PROCESS_BLOCK_TOGGLE;
  } else if (compareStrings(command, IPC_CAT_CMD) == 0) {
    return CMD_IPC_CAT;
  } else if (compareStrings(command, IPC_LINE_COUNT_CMD) == 0) {
    return CMD_IPC_LINE_COUNT;
  } else if (compareStrings(command, IPC_FILTER_VOWELS_CMD) == 0) {
    return CMD_IPC_FILTER_VOWELS;
  } else if (compareStrings(command, IPC_PHYLO_CMD) == 0) {
    return CMD_IPC_PHYLO;
  } else if (compareStrings(command, TEST_MM_CMD) == 0) {
    return CMD_TEST_MM;
  } else if (compareStrings(command, TEST_SCH_CMD) == 0) {
    return CMD_TEST_SCH;
  } else {
    return CMD_UNKNOWN;
  }
}

void drawConsole() {
  call_drawRectangle(LIGHT_GRAY, 0, 0, call_getWidth(),
                     call_getHeight()); // justo al final de pepsiman
  call_setFGColor(DARK_GRAY);
  call_setBGColor(LIGHT_GRAY);
  int currentsize = call_getSize();
  call_setSize(1);
  call_setXBuffer(call_getWidth() - (40 * call_getSize() * 8));
  call_setYBuffer(call_getHeight() - (13 * call_getSize()));
  char texto[] = "Consola PepsOS ltd. all rights reserved.";
  for (int i = 0; texto[i] != 0; i++) {
    call_drawLetterFromChar(texto[i]);
  }
  call_setSize(currentsize);
}
void deleteConsole() {
  call_drawRectangle(BLACK, 0, 500, call_getWidth(), call_getHeight() - 500);
}

void askForAnyletter() {
  int currentsize = call_getSize();
  call_setSize(1);
  char text[] = "\n\n\n\n->PRESIONE CUALQUIER TECLA PARA CONTINUAR";
  for (int i = 0; text[i] != 0; i++) {
    call_drawLetterFromChar(text[i]);
  }
  call_setSize(currentsize);
}

void printHelp() {
  call_clear();
  int currentsize = call_getSize();
  call_setSize(2);
  char text[] =
      "->Mostrar Procesos: ps\n->Crear proceso loop: loop\n->Matar un proceso: "
      "kill (pid)\n->Cambiar prioridad (numero de 0 a 4) de un proceso: nice "
      "(pid) (prioridad)\n->Testear division por cero: testdivzero\n->Testear "
      "invalid op code: testinvalidopcode\n->Aumentar tamanio de fuente: "
      "increasefontsize\n->Reducir tamanio de fuente: reducefontsize\n->Estado "
      "de la memoria: mem\n->Juego de los filosofos:phylo\n";
  call_setXBuffer(0);
  call_setYBuffer(5);
  for (int i = 0; text[i] != 0; i++) {
    call_drawLetterFromChar(text[i]);
  }
  call_setSize(currentsize);
}
void enlargeFontSize() { call_setSize(call_getSize() + 1); }
void decreaseFontSize() { // no actualiza el tamño del menu mientras este en la
                          // consola
  call_setSize(call_getSize() - 1); // es comportamiento esperado
}

void printMemState() {
  call_clear();
  int currentsize = call_getSize();
  call_setSize(2);
  char text[] = "ESTADO ACTUAL DE MEMORIA (LIBRE, ALOCADA, TOTAL):\n ";
  call_setXBuffer(0);
  call_setYBuffer(2 * (call_getHeight() / 3) + 13 * call_getSize());
  for (int i = 0; text[i] != 0; i++) {
    call_drawLetterFromChar(text[i]);
  }
  call_setSize(currentsize);

  unsigned long long int freeMemory = 0;
  unsigned long long int allocatedMemory = 0;
  unsigned long long int totalMemory = 0;
  call_mem_state(&freeMemory, &totalMemory, &allocatedMemory);
  freeMemory = totalMemory - allocatedMemory;
  call_print_long_long_int(freeMemory);
  call_drawLetterFromChar('B');
  call_drawLetterFromChar('/');
  call_print_long_long_int(allocatedMemory);
  call_drawLetterFromChar('B');
  call_drawLetterFromChar('/');
  call_print_long_long_int(totalMemory);
  call_drawLetterFromChar('B');
}