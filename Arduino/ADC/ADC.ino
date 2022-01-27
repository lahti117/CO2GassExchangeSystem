// This is the first version to try and use SPI
// There will be SPI commands sent from another arduino
// acting like the raspberry pi. This arduino will respond
// and do the appropriate action.

#include <stdint.h>
#include <SPI.h>
#include "globals.h"

#define SM_PERIOD 50

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
  indx = 0; // buffer empty
  process = false;
  SPI.attachInterrupt(); // turn on interrupt
}

ISR (SPI_STC_vect) {  // SPI interrupt routine 
   byte c = SPDR; // read byte from SPI Data Register
   if (indx < sizeof buff) {
      buff [indx++] = c; // save data in the next index in the array buff
      if (c == '\0') //check for the end of the word
      process = true;
   }
}

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  stateMachine_Init();
  setupSPI();
  setupRelays();
}

void loop() {
  currentMillis = millis();
  if ((currentMillis - previousMillis) >= SM_PERIOD) {
    previousMillis = currentMillis;
    stateMachine_tick();
  }
}
