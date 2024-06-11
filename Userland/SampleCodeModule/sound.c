#include <UserSyscalls.h>
#include <stdint.h>

void playSound(uint32_t freq, uint32_t ticks) { call_beep(freq, ticks); }

void playC2(uint8_t ticks) { playSound(65, ticks); }
void playCSharp2(uint8_t ticks) { playSound(69, ticks); }
void playD2(uint8_t ticks) { playSound(73, ticks); }
void playDSharp2(uint8_t ticks) { playSound(78, ticks); }
void playE2(uint8_t ticks) { playSound(82, ticks); }
void playF2(uint8_t ticks) { playSound(87, ticks); }
void playFSharp2(uint8_t ticks) { playSound(92, ticks); }
void playG2(uint8_t ticks) { playSound(98, ticks); }
void playGSharp2(uint8_t ticks) { playSound(104, ticks); }
void playA2(uint8_t ticks) { playSound(110, ticks); }
void playASharp2(uint8_t ticks) { playSound(117, ticks); }
void playB2(uint8_t ticks) { playSound(123, ticks); }
void playC3(uint8_t ticks) { playSound(131, ticks); }
void playCSharp3(uint8_t ticks) { playSound(139, ticks); }
void playD3(uint8_t ticks) { playSound(147, ticks); }
void playDSharp3(uint8_t ticks) { playSound(156, ticks); }
void playE3(uint8_t ticks) { playSound(165, ticks); }
void playF3(uint8_t ticks) { playSound(175, ticks); }
void playFSharp3(uint8_t ticks) { playSound(185, ticks); }
void playG3(uint8_t ticks) { playSound(196, ticks); }
void playGSharp3(uint8_t ticks) { playSound(208, ticks); }
void playA3(uint8_t ticks) { playSound(220, ticks); }
void playASharp3(uint8_t ticks) { playSound(233, ticks); }
void playB3(uint8_t ticks) { playSound(247, ticks); }
void playC4(uint8_t ticks) { playSound(262, ticks); }
void playCSharp4(uint8_t ticks) { playSound(277, ticks); }
void playD4(uint8_t ticks) { playSound(294, ticks); }
void playDSharp4(uint8_t ticks) { playSound(311, ticks); }
void playE4(uint8_t ticks) { playSound(330, ticks); }
void playF4(uint8_t ticks) { playSound(349, ticks); }
void playFSharp4(uint8_t ticks) { playSound(370, ticks); }
void playG4(uint8_t ticks) { playSound(392, ticks); }
void playGSharp4(uint8_t ticks) { playSound(415, ticks); }
void playA4(uint8_t ticks) { playSound(440, ticks); }
void playASharp4(uint8_t ticks) { playSound(466, ticks); }
void playB4(uint8_t ticks) { playSound(494, ticks); }
void playC5(uint8_t ticks) { playSound(523, ticks); }
void playCSharp5(uint8_t ticks) { playSound(554, ticks); }
void playD5(uint8_t ticks) { playSound(587, ticks); }
void playDSharp5(uint8_t ticks) { playSound(622, ticks); }
void playE5(uint8_t ticks) { playSound(659, ticks); }
void playF5(uint8_t ticks) { playSound(698, ticks); }
void playFSharp5(uint8_t ticks) { playSound(740, ticks); }
void playG5(uint8_t ticks) { playSound(784, ticks); }
void playGSharp5(uint8_t ticks) { playSound(831, ticks); }
void playA5(uint8_t ticks) { playSound(880, ticks); }
void playASharp5(uint8_t ticks) { playSound(932, ticks); }
void playB5(uint8_t ticks) { playSound(988, ticks); }
void playC6(uint8_t ticks) { playSound(1047, ticks); }
void playCSharp6(uint8_t ticks) { playSound(1109, ticks); }
void playD6(uint8_t ticks) { playSound(1175, ticks); }
void playDSharp6(uint8_t ticks) { playSound(1245, ticks); }
void playE6(uint8_t ticks) { playSound(1319, ticks); }
void playF6(uint8_t ticks) { playSound(1397, ticks); }
void playFSharp6(uint8_t ticks) { playSound(1480, ticks); }
void playG6(uint8_t ticks) { playSound(1568, ticks); }
void playGSharp6(uint8_t ticks) { playSound(1661, ticks); }
void playA6(uint8_t ticks) { playSound(1760, ticks); }
void playASharp6(uint8_t ticks) { playSound(1865, ticks); }
void playB6(uint8_t ticks) { playSound(1976, ticks); }
void playC7(uint8_t ticks) { playSound(2093, ticks); }

void playEndingSound() {
  playFSharp4(9);
  playF4(9);
  playE4(9);
  playDSharp4(18);
}

// c5 e5 g5 c6 g5 c6

void playStartingSound() {
  playC5(4);
  playE5(4);
  playG5(4);
  playC6(9);
  playG5(4);
  playC6(9);
}

// megalovania
// d4 d5 a4 g#4 g4 f4 d4 f4 g4
void playMegalovania() {
  playD4(2);
  playD4(2);
  call_wait(2);
  playD5(2);
  call_wait(2);
  playA4(4);
  call_wait(2);
  playGSharp4(2);
  call_wait(2);
  playG4(2);
  call_wait(2);
  playF4(4);
  playD4(2);
  playF4(2);
  playG4(2);
}
