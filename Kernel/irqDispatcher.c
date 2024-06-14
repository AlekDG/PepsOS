#include <keyboard.h>
#include <lib.h>
#include <memMan.h>
#include <pipes.h>
#include <scheduler.h>
#include <sem.h>
#include <sound.h>
#include <time.h>
#include <video.h>

static void int_20();
static void int_21();
static int int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx,
                  uint64_t r8, uint64_t r9);

typedef void (*InterruptHandler)(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                                 uint64_t rcx, uint64_t r8, uint64_t r9);

void irqDispatcher(uint64_t irq, uint64_t rdi, uint64_t rsi, uint64_t rdx,
                   uint64_t rcx, uint64_t r8, uint64_t r9) {
  InterruptHandler interruption[256] = {0};
  interruption[0] = &int_20;
  interruption[1] = &int_21;
  interruption[96] = (InterruptHandler)int_80;

  if (irq < 256 && interruption[irq] != 0) {
    InterruptHandler handler = interruption[irq];
    handler(rdi, rsi, rdx, rcx, r8, r9);
    return;
  }
}

void system_write(char* string);
void system_read(char* retAddress,int length);

void int_20() { timer_handler(); }

void int_21() { keyAct(); }

int int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8,
           uint64_t r9) {
  switch (rdi) {
  case 1:
    printRegs();
    break;
  case 2:
    clear();
    break;
  case 3:
    drawRectangle(rsi, rdx, rcx, r8, r9);
    break;
  case 4:
    drawCircle(rsi, rdx, rcx, r8);
    break;
  case 5:
    drawFace(rsi, rdx, rcx);
    break;
  case 6:
    return getKbChar();
    break;
  case 7:
    timer_wait(rsi);
    break;
  case 8:
    return ticks_elapsed();
    break;
  case 9:
    setSize(rsi);
    break;
  case 10:
    beep(rsi, rdx);
    break;
  case 11:
    return getFullWidth();
    break;
  case 12:
    return getFullHeight();
    break;
  case 13:
    deleteLetterBuffered();
    break;
  case 14:
    drawLetterFromChar(rsi);
    break;
  case 15:
    return getSize();
    break;
  case 16:
    setXBuffer(rsi);
    break;
  case 17:
    setYBuffer(rsi);
    break;
  case 18:
    return getXBuffer();
    break;
  case 19:
    return getYBuffer();
    break;
  case 20:
    return getBGColor();
    break;
  case 21:
    return getFGColor();
    break;
  case 22:
    return getFGColorPointer();
    break;
  case 23:
    return getXBufferPointer();
    break;
  case 24:
    return getYBufferPointer();
    break;
  case 25:
    setFGColor(rsi);
    break;
  case 26:
    setBGColor(rsi);
    break;
  case 27:
    putpixelResizable(rsi, rdx, rcx, r8);
    break;
  case 28:
    return seconds();
    break;
  case 29:
    paintScreen(rsi);
    break;
  case 30:
    printInteger(rsi);
    break;
  case 31:
    drawLetterFormatted(rsi, rdx, rcx, r8);
    break;
  case 32:
    drawStringFormatted(rsi, rdx, rcx, r8);
    break;
  case 33:
    printIntFormatted(rsi, rdx, rcx, r8);
    break;
  case 34:
    return minutes();
    break;
  case 35:
    return hours();
    break;
  case 36:
    return day();
    break;
  case 37:
    return month();
    break;
  case 38:
    return year();
    break;
  case 39:
    return allocMemory(rsi);
    break;
  case 41:
    return getPid();
    break;
  case 42:
    return createBackgroundProcess(rsi, rdx, rcx, r8, r9);
    break;
  case 43:
    return createForegroundProcess(rsi, rdx, rcx, r8, r9);
    break;
  case 44:
    exit();
    break;
  case 45:
    freeMemory(rsi);
    break;
  case 46:
    return sem_open(rsi, rdx);
    break;
  case 47:
    sem_close(rsi);
    break;
  case 48:
    return create_sem(rsi, rdx);
    break;
  case 49:
    sem_post(rsi);
    break;
  case 50:
    sem_wait(rsi);
    break;
  case 51:
    return open_pipe(rsi);
    break;
  case 52:
    close_pipe(rsi);
    break;
  case 53:
    write_to_pipe(rsi, rdi);
    break;
  case 54:
    return read_pipe(rsi);
    break;
  case 55:
    return changePriority(rsi, rdx);
    break;
  case 56:
    return getAllProcessInfo(rsi);
    break;
  case 57:
    return kill(rsi);
    break;
  case 58:
    return block(rsi);
    break;
  case 59:
    return unblock(rsi);
    break;
  case 60:
    yield();
    break;
  case 61:
    memState(rsi, rdx, rcx);
    break;
  case 62:
    printProcessInfo(rsi);
    break;
  case 63:
    sleep(rsi);
    break;
  case 64:
    printHex(rsi);
    break;
  case 65:
    print_long_long_int(rsi);
    break;
  case 66:
    return wait();
    break;
  case 67:
    gameplay_on();
    break;
  case 68:
    gameplay_off();
    break;
  case 69:
    newLine();
    break;
  case 70:
    return (get_sem_indx(rsi));
    break;
  case 71:
    system_write(rsi);
    break;
  case 72:
    system_read(rsi, rdx);
    break;
  case 73:
    colorReset();
    break;
  case 74:
    setColor(rsi,rdx);
    break;
  default:
    return 0;
  }
  return 0;
}

void system_write(char* string){
  Process *current_proc = get_current_proc();
  if(current_proc->out_pipe == STDOUT_PIPE&&currentProcType()){
    drawStringDef(string);
  } else {
    write_to_pipe(current_proc->out_pipe, string);
  }
}

void system_read(char* retAddress,int length){
  Process *current_proc = get_current_proc();
  int indx=0;
  if(current_proc->in_pipe == STDIN_PIPE&&currentProcType()){
    do{
    retAddress[indx++] = read_pipe(STDIN_PIPE);
    } while (retAddress[indx]!=NULL&&retAddress[indx]!=EOF&&indx<length);
  } else {
    while(indx<length){
      retAddress[indx++]=read_pipe(current_proc->in_pipe);
    }
  }
  return;
}