// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <idtLoader.h>
#include <interrupts.h>
#include <lib.h>
#include <memMan.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <pipes.h>
#include <scheduler.h>
#include <sem.h>
#include <string.h>
#include <video.h>



extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;
static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void *bssAddress, uint64_t bssSize) {
  memset(bssAddress, 0, bssSize);
}

void *getStackBase() {
  return (void *)((uint64_t)&endOfKernel +
                  PageSize * 8
                  - sizeof(uint64_t)
  );
}

void *initializeKernelBinary() {
  char buffer[10];

  ncPrint("[x64BareBones]");
  ncNewline();

  ncPrint("CPU Vendor:");
  ncPrint(cpuVendor(buffer));
  ncNewline();

  ncPrint("[Loading modules]");
  ncNewline();
  void *moduleAddresses[] = {sampleCodeModuleAddress, sampleDataModuleAddress};

  loadModules(&endOfKernelBinary, moduleAddresses);
  ncPrint("[Done]");
  ncNewline();
  ncNewline();

  ncPrint("[Initializing kernel's binary]");
  ncNewline();

  clearBSS(&bss, &endOfKernel - &bss);

  ncPrint("  text: 0x");
  ncPrintHex((uint64_t)&text);
  ncNewline();
  ncPrint("  rodata: 0x");
  ncPrintHex((uint64_t)&rodata);
  ncNewline();
  ncPrint("  data: 0x");
  ncPrintHex((uint64_t)&data);
  ncNewline();
  ncPrint("  bss: 0x");
  ncPrintHex((uint64_t)&bss);
  ncNewline();

  ncPrint("[Done]");
  ncNewline();
  ncNewline();
  return getStackBase();
}

static processTable *systemProcessTable;

int main() {
  load_idt();
  initialState();
  initialize_memory();
  systemProcessTable = createPCB();
  start_sems();
  initialize_pipes();
  char *menuArgv[] = {"menu"};
  createProcess(((EntryPoint)sampleCodeModuleAddress), 0, menuArgv, 0,
                FOREGROUND, 0);
  startFirstProcess();
  _sti();
  while (1)
    ;
}
