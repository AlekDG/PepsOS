include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
OBJECTS=$(SOURCES:.C=.O)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
all: $(MODULE)

$(MODULE): $(SOURCES) $(OBJECTS_ASM)
	$(GCC) $(GCCFLAGS) -I./include -T sampleCodeModule.ld _loader.c $(SOURCES) $(OBJECTS_ASM) -o ../$(MODULE)
	$(GCC) $(GCCFLAGS) -I./include -T  sampleCodeModule.ld -Wl,--oformat=elf64-x86-64,-no-pie _loader.c $(SOURCES) $(OBJECTS_ASM) -o ../0000-sampleCodeModule.elf

	
%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@


%.o:%.asm
	$(ASM) $(ASMFLAGS) $< -o $@


clean:
	rm -rf *.o

.PHONY: all clean print
