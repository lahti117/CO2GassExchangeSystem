#include <stdint.h>

#define LEDPIN 13

static bool output;
unsigned int reload = 0xC35; 
// https://microcontrollerslab.com/arduino-timer-interrupts-tutorial/
ISR(TIMER1_COMPA_vect){
  if (output) {
    digitalWrite(LEDPIN, LOW);
  } else {
    digitalWrite(LEDPIN, HIGH);
  }
  output = !output;
  //Serial.println(output);
}

void setup() {
    Serial.begin(9600);
    pinMode(LEDPIN, OUTPUT);
    digitalWrite(LEDPIN, LOW);
    output = false;
    TCCR1A = 0;
    TCCR1B = 0; 
    OCR1A = reload;
    TCCR1B = (1<<WGM12) | (1<<CS12); 
    TIMSK1 = (1<<OCIE1A); 
    Serial.println("TIMER1 Setup Finished.");
}

void loop() {
    while(1) {
      Serial.println(output);
    }
}
