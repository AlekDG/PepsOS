 #include <sound.h>
 #include <stdint.h>
 #include <time.h>
 #include <playSound.h>
 //Play sound using built in speaker
 static void play_sound(uint32_t nFrequence) {
 	uint32_t Div;
 	uint8_t tmp;
 
        //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	speakerSet(Div);
 
        //And play the sound using the PC speaker
 	playback();
 }
 
 //make it shutup
 static void nosound() {
 	silence();
 }
 
 //Make a beep
 void beep(uint32_t freq,uint8_t ticks) {
 	 play_sound(1000);
 	 timer_wait(10);
 	 nosound();
          //set_PIT_2(old_frequency);
 }