// This is the version that saves data to the
// SD card that will store the data in a csv file
/*
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)
 ** CO2_A_INPUT_PIN - A0
 ** CO2_B_INPUT_PIN - A1
*/

#include <stdint.h>
#include <SPI.h>
#include <SD.h>

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
