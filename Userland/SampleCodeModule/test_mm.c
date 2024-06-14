#include <test_util.h>
#include <UserSyscalls.h>
#include <string.h>
#include <user_lib.h>

#define MAX_BLOCKS 128

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;

uint64_t test_mm(uint64_t argc, char *argv[]) {
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;
    int testokflag = 0;

  if (argc != 1)
    return -1;

  if ((max_memory = hexToULL(argv[0])) <= 0)
    return -1;

  while (1) {
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = call_malloc((size_t)mm_rqs[rq].size);
        call_print_long_long_int(mm_rqs[rq].size);
        call_drawStringFormatted("\n", BLACK, WHITE, 2);

        if (mm_rqs[rq].address) {
            total += mm_rqs[rq].size;
            call_printInteger(mm_rqs[rq].address);
            call_drawStringFormatted(" ", BLACK, WHITE, 2);
            call_print_long_long_int(total);
            call_drawStringFormatted("\n", BLACK, WHITE, 2);
            rq++;
            call_drawStringFormatted("request ok\n", BLACK, WHITE, 2);
        } else {
            call_drawStringFormatted("Allocation failed - Block too large or not enough space\n", RED, WHITE, 2);
            //break;
        }

        call_wait(20);
    }
    // Set
      call_drawStringFormatted("Setting memory...\n", BLACK, WHITE, 2);
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
      call_drawStringFormatted("Checking memory...\n", BLACK, WHITE, 2);
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
            call_drawStringFormatted("test_mm ERROR\n",
                                     WHITE, BLACK, 2);
          return -1;
        }

    // Free
      call_drawStringFormatted("Freeing memory...\n", BLACK, WHITE, 2);
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        call_free(mm_rqs[i].address);

    if(!testokflag){
        call_drawStringFormatted("MM TEST OK\n", BLACK, WHITE, 2);
        testokflag = 1;
    }
      call_wait(40);
      call_setXBuffer(0);
      call_setYBuffer(0);
      call_paintScreen(LIGHT_GRAY);
  }
}
