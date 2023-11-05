#include <UserSyscalls.h>
#include <stdint.h>

void playSound(uint32_t freq, uint32_t ticks)
{
    call_beep(freq, ticks);
}

void playFSharp4(uint8_t ticks){
    playSound(370, ticks);
}
void playF4(uint8_t ticks){
    playSound(349, ticks);
}
void playE4(uint8_t ticks){
    playSound(330, ticks);
}
void playDSharp4(uint8_t ticks){
    playSound(311, ticks);
}

void playEndingSound(){
    playFSharp4(9);
    playF4(9);
    playE4(9);
    playDSharp4(18);
}
