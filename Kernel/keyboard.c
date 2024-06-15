#include <keyboard.h>
#include <lib.h>
#include <pipes.h>
#include <stdbool.h>
#include <stdint.h>

#define STDIN_PIPE 0

const unsigned char kbArr[4][128] = {
    {0,    27,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',  '=',
     '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',  ']',
     '\n', 0,    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
     0,    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,    '*',
     0,    ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
     0,    0,    17,  0,   0,   18,  0,   19,  0,   0,   20,  0,   0,    0,
     '-',  0,    0,   '+', 0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
     0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
     0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
    {0,    27,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
     '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
     '\n', 0,    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '|',
     0,    '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,   '*',
     0,    ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     0,    0,    17,  0,   0,   18,  0,   19,  0,   0,   20,  0,   0,   0,
     '-',  0,    0,   '+', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
    {0,    27,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
     '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}',
     '\n', 0,    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', '|',
     0,    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 0,   '*',
     0,    ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     0,    0,    17,  0,   0,   18,  0,   19,  0,   0,   20,  0,   0,   0,
     '-',  0,    0,   '+', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
    {0,    27,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',  '=',
     '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[',  ']',
     '\n', 0,    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',
     0,    '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0,    '*',
     0,    ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
     0,    0,    17,  0,   0,   18,  0,   19,  0,   0,   20,  0,   0,    0,
     '-',  0,    0,   '+', 0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
     0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
     0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0}};

bool shift = false;
bool blockMayus = false;
bool ctrl = false;
bool gameplay = false;

void regCheckSave(uint64_t * regs) {
  if (getKey() == 0x38)
    saveRegsBuffer(regs);
}

char hexToChar(uint8_t hex) {
  uint8_t shiftState;
  if (shift && blockMayus)
    shiftState = 2;
  else if (shift)
    shiftState = 1;
  else if (blockMayus)
    shiftState = 3;
  else
    shiftState = 0;
  return kbArr[shiftState][hex];
}

void gameplay_on() { gameplay = true; }

void gameplay_off() { gameplay = false; }

void keyAct(void) {
  uint8_t keyHex = getKey();
  char keyChar;
  if (keyHex < 0x81) {
    keyPressed(true);
    switch (keyHex) {
    case 0x2A:
    case 0x36:
      shift = true;
      break;
    case 0x3A:
      blockMayus = !blockMayus;
      break;
    case 0x1D:
      ctrl = true;
      break;
    default:
      if (ctrl) {
        switch (keyHex) {
        case 0x2E:
          keyChar = KILL_SIGNAL;
          break;
        case 0x20:
          keyChar = EOF;
          break;
        }
      } else 
        keyChar = hexToChar(keyHex);
      if (keyHex < 0x53) {
        if (!gameplay) {
          if(!(keyChar<21&&keyChar>16)){
            char to_write[2];
            to_write[0] = keyChar;
            to_write[1] = '\0';
            write_to_pipe(STDIN_PIPE, to_write);
          }
        } else {
          addToBuffer(keyChar);
        }
      }
    }
  } else {
    keyPressed(false);
    switch (keyHex) {
    case 0xAA:
    case 0xB6:
      shift = false;
      break;
    case 0x9D:
      ctrl = false;
      break;
    default:
      break;
    }
  }
}