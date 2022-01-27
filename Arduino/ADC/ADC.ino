// This is the first version to try and use SPI
// There will be SPI commands sent from another arduino
// acting like the raspberry pi. This arduino will respond
// and do the appropriate action.

#include <stdint.h>
#include <SPI.h>

#define SM_PERIOD 50

uint32_t previousMillis = 0;
uint32_t currentMillis = 0;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  stateMachine_Init();
}

void loop() {
  currentMillis = millis();
  if ((currentMillis - previousMillis) >= SM_PERIOD) {
    previousMillis = currentMillis;
    stateMachine_tick();
  }
}
