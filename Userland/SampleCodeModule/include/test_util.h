#include <stdint.h>
#include <io.h>

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
void bussy_wait(uint64_t n);
void endless_loop();
void endless_loop_print(uint64_t wait);

