#include <UserSyscalls.h>
#include <consola.h>
#include <menu.h>
#include "philosophers.h"

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
int isBlockCommand(char *command, int *pid) {
  int i = 0;
  char *block = "block";
  while (command[i] != 0 && block[i] != 0) {
    if (command[i] != block[i]) {
      return 0;
    }
    i++;
  }
  if (command[i] == 0) {
    return 0;
  }
  while (command[i] == ' ') {
    i++;
  }
  if (command[i] == 0) {
    return 0;
  }
  int aux = 0;
  while (command[i] != 0 && command[i] != ' ') {
    if (command[i] < '0' || command[i] > '9') {
      return 0;
    }
    aux = aux * 10 + command[i] - '0';
    i++;
  }
  *pid = aux;
  return 1;
}

int isKillCommand(char *command, int *pid) {
  int i = 0;
  char *kill = "kill";
  while (command[i] != 0 && kill[i] != 0) {
    if (command[i] != kill[i]) {
      return 0;
    }
    i++;
  }
  if (command[i] == 0) {
    return 0;
  }
  while (command[i] == ' ') {
    i++;
  }
  if (command[i] == 0) {
    return 0;
  }
  int aux = 0;
  while (command[i] != 0 && command[i] != ' ') {
    if (command[i] < '0' || command[i] > '9') {
      return 0;
    }
    aux = aux * 10 + command[i] - '0';
    i++;
  }
  *pid = aux;
  return 1;
}

int isNiceComand(char *command, int *pid, int *priority) {
  int i = 0;
  char *nice = "nice";
  while (command[i] != 0 && nice[i] != 0) {
    if (command[i] != nice[i]) {
      return 0;
    }
    i++;
  }
  if (command[i] == 0) {
    return 0;
  }
  while (command[i] == ' ') {
    i++;
  }
  if (command[i] == 0) {
    return 0;
  }
  int aux = 0;
  while (command[i] != 0 && command[i] != ' ') {
    if (command[i] < '0' || command[i] > '9') {
      return 0;
    }
    aux = aux * 10 + command[i] - '0';
    i++;
  }
  *pid = aux;
  while (command[i] == ' ') {
    i++;
  }
  if (command[i] == 0) {
    return 1;
  }
  aux = 0;
  while (command[i] != 0 && command[i] != ' ') {
    if (command[i] < '0' || command[i] > '9') {
      return 0;
    }
    aux = aux * 10 + command[i] - '0';
    i++;
  }
  *priority = aux;
  return 1;
}

CommandType getCommandType(const char *command, int *arg1, int *arg2) {
  if (isKillCommand(command, arg1)) {
    return CMD_PROCESS_KILL;
  }
  if (isNiceComand(command, arg1, arg2)) {
    return CMD_PROCESS_NICE;
  }
  if (isBlockCommand(command, arg1)) {
    return CMD_PROCESS_BLOCK_TOGGLE;
  }

  if (compareStrings(command, ENLARGE_FONT_SIZE_CMD) == 0) {
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
  } else {
    return CMD_UNKNOWN;
  }
}

int compareStrings(char *s1, char *s2) {
  while (*s1 != 0 && *s2 != 0) {
    char first = *s1;
    char last = *s2;
    if (first > last) {
      return 1;
    } else if (first < last) {
      return -1;
    }
    s1++;
    s2++; // sigo comparando
  }
  if (*s1 != 0 && *s2 == 0) {
    return 1;
  }
  if (*s1 == 0 && *s2 != 0) {
    return -1;
  }
  return 0; // ambos son iguales
}

int strLen(const char *str) {
  int len = 0;
  while (str[len] != '\0') {
    len++;
  }
  return len;
}

int hexCharToInt(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  } else if (c >= 'a' && c <= 'f') {
    return c - 'a' + 10;
  } else if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  } else {
    return -1;
  }
}

unsigned long long hexToULL(const char *hexStr) {
  unsigned long long result = 0;
  int len = strLen(hexStr);

  for (int i = 0; i < len; i++) {
    int value = hexCharToInt(hexStr[i]);
    if (value == -1) {
      // Invalid character in the hex string
      return -1;
    }
    result = (result << 4) | value;
  }

  return result;
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
  deleteConsole();
  call_drawRectangle(LIGHT_GRAY, 0, 0, call_getWidth(), call_getHeight());
  int currentsize = call_getSize();
  call_setSize(2);
  char text[] =
      "->Mostrar Procesos: ps\n->Crear proceso loop: loop\n->Matar un proceso: "
      "kill (pid)\n->Cambiar prioridad (numero de 0 a 4) de un proceso: nice "
      "(pid) (prioridad)\n->Testear division por cero: testdivzero\n->Testear "
      "invalid op code: testinvalidopcode\n->Aumentar tamanio de fuente: "
      "increasefontsize\n->Reducir tamanio de fuente: reducefontsize";
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
  deleteConsole();
  call_drawRectangle(LIGHT_GRAY, 0, 500, call_getWidth(),
                     call_getHeight() - 500);
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
void interpretCommand(char command[]) {
  int arg1;
  int arg2;
  CommandType cmdType = getCommandType(command, &arg1, &arg2);
  switch (cmdType) {
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
    //  handler de memstate

  case CMD_PROCESS_LOOP:
    char *loopArgv[] = {"loop"};
    call_createBackgroundProcess(loop, 0, loopArgv, 0);
    break;
  case CMD_PROCESSES_STATE:
    char *psArgv[] = {"ps"};
    call_createForegroundProcess(ps, 0, psArgv, 4);
    break;
  case CMD_PROCESS_KILL:
    int wasKilled = call_kill(arg1);
    if (wasKilled) {
      call_drawStringFormatted("Process killed", RED, LIGHT_GRAY, 2);
    } else {
      call_drawStringFormatted("Process not killed", BLACK, LIGHT_GRAY, 2);
    }
    call_pipe_read(STDIN);
    return;
    break;
  case CMD_PROCESS_NICE:
    int wasChanged = call_changePriority(arg1, arg2);
    if (wasChanged) {
      call_drawStringFormatted("Process priority changed", GREEN, LIGHT_GRAY,
                               2);
    } else {
      call_drawStringFormatted("Process priority not changed", BLACK,
                               LIGHT_GRAY, 2);
    }
    call_pipe_read(STDIN);
    return;
    break;
  case CMD_PROCESS_BLOCK_TOGGLE:
    int wasBlock = call_block(arg1);
    if (!wasBlock) {
      call_unblock(arg1);
    }
    call_pipe_read(STDIN);
    return;
  case CMD_IPC_CAT:
    drawConsole();
    call_setXBuffer(0);
    call_setYBuffer(10);
    char catBuffer[255] = {0};
    int buffer_size = 1;
    char current_letter;
    int cat_active = 1;
    while(cat_active){
      current_letter = call_pipe_read(STDIN);
      switch (current_letter)
      {
      case 3:
        //TODO: Tobi checkea que esto termine este comando y no reviente todo: call_kill(call_getPid());
        cat_active = 0;
        break;
      case '\n':
        call_new_line();
        break;
      case 0:
        call_exit();
      case '\b':
      if (buffer_size <= 0) {
        buffer_size = 0;
        break;
      }
      call_deleteLetterBuffered();
      catBuffer[buffer_size--] =
          0;
      break;
    default:
      if (buffer_size >= 255) {
        break;
      }
      call_drawLetterFromChar(current_letter);
      catBuffer[buffer_size++] =
          current_letter;
      break;
      }
    }
    return;
      case CMD_IPC_PHYLO:
          char *philoArgv[] = {"phylo"};
          call_createForegroundProcess(run_Philosophers, 0, philoArgv, 4);
  case CMD_UNKNOWN:
  default:
    //  UNKNOWN HANDLER
    break;
  }
}
void runConsole() {
  drawConsole();
  call_setXBuffer(0);
  call_setYBuffer(10);

  char internalBuffer[50] = {0}; // tamaño maximo de 50 chars
  int bufferSize = 0;
  int consoleRunning = 1; // flag de corte de ejecucion
  char currentLetter;

  while (consoleRunning) {
    currentLetter = call_pipe_read(STDIN);
    switch (currentLetter) {
    case '\n':
      interpretCommand(internalBuffer);
      drawConsole();
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
