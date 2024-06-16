# Define the global variable
USE_BUDDY=0

all: USE_BUDDY=0
all: bootloader kernel userland image

buddy: USE_BUDDY=1
buddy: bootloader buddy_kernel userland image

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all USE_BUDDY=$(USE_BUDDY)

buddy_kernel:
	cd Kernel; make buddy USE_BUDDY=$(USE_BUDDY)

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

.PHONY: bootloader image collections kernel userland all clean buddy buddy_kernel
