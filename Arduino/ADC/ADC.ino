// This is the first version to try and use SPI
// There will be SPI commands sent from another arduino
// acting like the raspberry pi. This arduino will respond
// and do the appropriate action.

#include <stdint.h>
#include "globals.h"
#include "globals.c"
//#include <SPI.h>
// #include <SD.h>

// #ifdef SDCARD
// File dataFile;
// #endif


#define SM_PERIOD 50
#define ROLLOVER_VALUE 0xc35

uint32_t previousMillis = 0;
uint32_t currentMillis = 0;

#ifdef RELAYS
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
#endif

#ifdef SPI_TO_PI
void setupSPI() {
  pinMode(MISO, OUTPUT); // have to send on master in so it set as output
  SPCR |= _BV(SPE); // turn on SPI in slave mode
  SPI.attachInterrupt(); // turn on interrupt
}
#endif

#ifdef SPI_TO_PI
ISR (SPI_STC_vect) {  // SPI interrupt routine 
  byte c = SPDR; // read byte from SPI Data Register
  if (globals_bufferElementCount() < GLOBALS_DATA_BUFFER_SIZE) {
    global_addDataToBuffer(c); // save data in the next index in the array buff
    if (c == '\0') //check for the end of the word
      globals_setProcessFlag(true);
  }
}
#endif

#ifdef SDCARD
void setupSDCard () {
  if (!SD.begin(10)) {
    Serial.println("SD Card initialization failed!");
  }
  else {
    Serial.println("SD Card initialization Success!");
    dataFile = SD.open("data.txt", FILE_WRITE);
  }
}
#endif

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
  analogReference(INTERNAL);
  globals_initGlobals();
  stateMachine_Init();
  #ifdef RELAYS
  setupRelays();
  #endif
  #ifdef SPI_TO_PI
  setupSPI();
  Serial.println("SPI setup");
  #endif
  #ifdef SDCARD
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
  }
  #endif
  setupInterrupts();
}

void loop() {
  // Everything is handled in the interrupt
  // routines so this is just waiting
  while(1);
}
