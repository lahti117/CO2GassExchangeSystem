// This is the first version to try and use SPI
// There will be SPI commands sent from another arduino
// acting like the raspberry pi. This arduino will respond
// and do the appropriate action.

#include <stdint.h>
#include <SPI.h>
#include "globals.h"
#include "globals.c"

#define SM_PERIOD 50
#define ROLLOVER_VALUE 0xc35

uint32_t previousMillis = 0;
uint32_t currentMillis = 0;

void setupRelays() {
  pinMode(relay_0, OUTPUT);
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relay_4, OUTPUT);
  pinMode(relay_5, OUTPUT);
  pinMode(relay_6, OUTPUT);
  pinMode(relay_7, OUTPUT);
}

void setupSPI() {
  pinMode(MISO, OUTPUT); // have to send on master in so it set as output
  SPCR |= _BV(SPE); // turn on SPI in slave mode
  SPI.attachInterrupt(); // turn on interrupt
}

void setupInterrupts() {
  TCCR1A = 0;
  TCCR1B = 0; 
  OCR1A = ROLLOVER_VALUE;
  TCCR1B = (1<<WGM12) | (1<<CS12); 
  TIMSK1 = (1<<OCIE1A);
  Serial.println("TIMER1 Setup Finished.");
}

ISR (SPI_STC_vect) {  // SPI interrupt routine 
  byte c = SPDR; // read byte from SPI Data Register
  if (globals_bufferElementCount() < GLOBALS_DATA_BUFFER_SIZE) {
    global_addDataToBuffer(c); // save data in the next index in the array buff
    if (c == '\0') //check for the end of the word
      globals_setProcessFlag(true);
  }
}

ISR (TIMER1_COMPA_vect){
  stateMachine_tick();
}

void setup() {
  Serial.begin(9600);
  stateMachine_Init();
  setupRelays();
  globals_initGlobals();
  setupSPI();
  setupInterrupts();
}

void loop() {
  // Everything is handled in the interrupt
  // routines so this is just waiting
  while(1);
}
