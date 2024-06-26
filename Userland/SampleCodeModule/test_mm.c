// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <UserSyscalls.h>
#include <string.h>
#include <test_util.h>
#include <stddef.h>

#define MAX_BLOCKS 128
#define A_USEFUL_COEFFICIENT 10000
#define MEGABYTES_COEFFICIENT 1048576
#define MINIMUM_MEMORY 32

int64_t satoi2(char *str) {
  uint64_t i = 0;
  int64_t res = 0;
  int8_t sign = 1;

  if (!str)
    return 0;

  if (str[i] == '-') {
    i++;
    sign = -1;
  }

  for (; str[i] != '\0'; ++i) {
    if (str[i] < '0' || str[i] > '9')
      return 0;
    res = res * 10 + str[i] - '0';
  }

  return res * sign;
}

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;

void test_mm(uint64_t argc, char *argv[]) {
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;
  int testokflag = 0;
  if (argc != 1)
    call_exit();
  int numberOfMB = satoi2(argv[1]);
  call_drawStringFormatted("Received Memory: ", BLACK, LIGHT_GRAY, 2);
  call_printInteger(numberOfMB);
  call_drawStringFormatted("MB\n", BLACK, LIGHT_GRAY, 2);
  call_wait(20);
  if (numberOfMB < MINIMUM_MEMORY) {
    call_drawStringFormatted("The amount of memory provided is too small!!\n",
                             BLACK, LIGHT_GRAY, 2);
    call_exit();
  }
  uint64_t aux1 = (uint64_t)numberOfMB * MEGABYTES_COEFFICIENT;
  max_memory = aux1;
  while (1) {
    rq = 0;
    total = 0;
    while (rq < MAX_BLOCKS && total < max_memory) {
      mm_rqs[rq].size =
          GetUniform(max_memory - total - 1) / A_USEFUL_COEFFICIENT + 1;
      mm_rqs[rq].address = call_malloc((size_t)mm_rqs[rq].size);
      call_drawStringFormatted("Size requested: ", BLACK, LIGHT_GRAY, 2);
      call_print_long_long_int(mm_rqs[rq].size);
      call_drawStringFormatted("\n", BLACK, LIGHT_GRAY, 2);

      if (mm_rqs[rq].address) {
        total += mm_rqs[rq].size;
        call_drawStringFormatted("Address requested: ", BLACK, LIGHT_GRAY, 2);
        call_print_long_long_int((unsigned long long int)mm_rqs[rq].address);
        call_drawStringFormatted("\n", BLACK, LIGHT_GRAY, 2);
        rq++;
        call_drawStringFormatted("\nrequest ok\n", BLACK, LIGHT_GRAY, 2);
        call_wait(10);
      } else {
        call_drawStringFormatted(
            "Allocation failed - Block too large or not enough space\n", RED,
            LIGHT_GRAY, 2);
      }
      call_setXBuffer(0);
      call_setYBuffer(0);
      call_paintScreen(LIGHT_GRAY);
      call_wait(5);
    }
    call_drawStringFormatted("Setting memory...\n", BLACK, LIGHT_GRAY, 2);
    uint32_t i;
    for (i = 0; i < rq; i++) {
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);
    }
    call_drawStringFormatted("Checking memory...\n", BLACK, LIGHT_GRAY, 2);
    for (i = 0; i < rq; i++) {
      if (mm_rqs[i].address) {
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          call_drawStringFormatted("test_mm ERROR\n", WHITE, BLACK, 2);
          call_exit();
        }
      }
    }

    call_drawStringFormatted("Freeing memory...\n", BLACK, LIGHT_GRAY, 2);
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        call_free(mm_rqs[i].address);

    if (!testokflag) {
      call_drawStringFormatted("MM TEST OK\n", BLACK, LIGHT_GRAY, 2);
      testokflag = 1;
    }
    call_wait(40);
    call_setXBuffer(0);
    call_setYBuffer(0);
    call_paintScreen(LIGHT_GRAY);
  }
}
