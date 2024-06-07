include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(filter-out BlockMemoryManager.c BuddyMemoryManager.c, $(wildcard *.c))
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

# Define additional sources based on the target
ifeq ($(MAKECMDGOALS), buddy)
    MEMORYMANAGER=BuddyMemoryManager.c
else
    MEMORYMANAGER=BlockMemoryManager.c
endif

MEMORYMANAGER_OBJECT=$(MEMORYMANAGER:.c=.o)
OBJECTS += $(MEMORYMANAGER_OBJECT)

all: $(KERNEL)

buddy: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf  $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin *.elf

.PHONY: all clean buddy
