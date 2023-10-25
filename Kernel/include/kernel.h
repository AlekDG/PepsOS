#ifndef KERNEL_H
#define KERNEL_H
#include <stdbool.h>
    void addToBuffer(char c);
    char getKbChar(void);
    void keyPressed(bool state);
    bool keyIsPressed(void);
#endif