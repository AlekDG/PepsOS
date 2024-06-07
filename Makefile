all:  bootloader kernel userland image

buddy:  bootloader buddy_kernel userland image

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

buddy_kernel:
	cd Kernel; make buddy

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

# Additional target to build with BuddyMemoryManager
buddy_kernel:
	cd Kernel; make CFLAGS+=-DUSE_BUDDY all
