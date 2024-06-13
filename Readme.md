### PepOS
PepsOS is the future of Operating Systems built for the AMD64 architecture.

The final goal of the project is to provide a simple feature-complete operating systems with all the development tools anyone could realistically need on minimum hardware.

Environment setup:

1- Install the following packages before building the Toolchain and Kernel:

nasm qemu gcc make

2- Build the Toolchain

Execute the following commands on the PepsOS project directory:
  user@linux:$ cd Toolchain
  user@linux:$ make all

3- Build the Kernel

From the PepsOS project directory run:

  user@linux:$ make all

4- Run the kernel

From the PepsOS project directory run:

  user@linux:$ ./run.sh

- Author: Alejo Padula Morillo
- Collaborators: Tobias Pugliano, Joaquin Locioli, Julian Bogado, Nicanor Porto
- Built on top of the x64BareBones project by Rodrigo Rearden and Augusto Nizzo
