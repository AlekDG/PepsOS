include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(filter-out BlockMemoryManager.c BuddyMemoryManager.c, $(wildcard *.c))
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

# Define additional sources based on the global variable
ifeq ($(USE_BUDDY), 1)
    MEMORYMANAGER=BuddyMemoryManager.c
else
    MEMORYMANAGER=BlockMemoryManager.c
endif

MEMORYMANAGER_OBJECT=$(MEMORYMANAGER:.c=.o)
OBJECTS += $(MEMORYMANAGER_OBJECT)

# Pass the USE_BUDDY variable as a preprocessor macro
GCCFLAGS += -DUSE_BUDDY=$(USE_BUDDY)

all: $(KERNEL)

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
