include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
OBJECTS=$(SOURCES:.C=.O)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
all: $(MODULE)

$(MODULE): $(SOURCES) $(OBJECTS_ASM)
	$(GCC) $(GCCFLAGS) -I./include -T sampleCodeModule.ld _loader.c $(SOURCES) $(OBJECTS_ASM) -o ../$(MODULE)

	
%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@


%.o:%.asm
	$(ASM) $(ASMFLAGS) $< -o $@


clean:
	rm -rf *.o

.PHONY: all clean print
