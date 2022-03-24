#include <stdint.h>
unsigned int reload = 0xF424; 

ISR(TIMER1_COMPA_vect){
  
}

void setup() {
    Serial.begin(9600);
    cli();
    TCCR1A = 0;
    TCCR1B = 0; 
    OCR1A = reload;
    TCCR1B = (1<<WGM12) | (1<<CS12); 
    TIMSK1 = (1<<OCIE1A); 
    sei(); 
    Serial.println("TIMER1 Setup Finished.");
}

void loop() {
    
}
