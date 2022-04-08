// This is the first version to try and use SPI
// There will be SPI commands sent from another arduino
// acting like the raspberry pi. This arduino will respond
// and do the appropriate action.

#include <stdint.h>
//#include <SPI.h>
// #include <SD.h>

#define SM_PERIOD 50
#define ROLLOVER_VALUE 0xc35

void setupInterrupts() {
  TCCR1A = 0;
  TCCR1B = 0; 
  OCR1A = ROLLOVER_VALUE;
  TCCR1B = (1<<WGM12) | (1<<CS12); 
  TIMSK1 = (1<<OCIE1A);
  Serial.println("TIMER1 Setup Finished.");
}

ISR (TIMER1_COMPA_vect){
  stateMachine_tick();
}

void setup() {
  Serial.begin(9600);
  stateMachine_Init();
  setupInterrupts();
}

void loop() {
  // Everything is handled in the interrupt
  // routines so this is just waiting
  while(1);
}
