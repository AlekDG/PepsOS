#include <stdint.h>
#include <stdbool.h>
#include "video.h"


const unsigned char kbArr[2][128] = {
    {0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd',
    'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S', 'D',
    'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '|', 0, '\\', 'Z', 'X', 'C', 'V', 'B', 'N',
    'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

bool shift = false;
bool blockMayus = false;

char hexToChar(uint8_t hex){
    uint8_t shiftState = shift?(blockMayus?0:1):(blockMayus?1:0);
    return kbArr[shiftState][hex];
}

void writeKbInput(){
	uint8_t keyHex = getKey();
    char keyChar;
    if(keyHex<0x82){
        switch(keyHex){
            case 0x2A: case 0x36:
                shift=true;
                break;
            case 0x3A:
                blockMayus = !blockMayus;
                break;
            //case 0x0E: case 0xE0:
                //deleteLetterBuffered();
                //break;
            default:
                keyChar = hexToChar(keyHex);
    		    if(keyChar!=0){
			        drawLetterBuffered(keyChar);
		        }
        }
    } else {
        switch (keyHex)
        {
        case 0xAA: case 0xB6:
            shift=false;
            break;
        default:
            break;
        }
    }
}
