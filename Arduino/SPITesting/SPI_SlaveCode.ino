/*#include <SPI.h>
#define SLAVE_SELECT_PIN 10
const int cs = SLAVE_SELECT_PIN;

// Pin 10 is Slave Select pin
// To communicate bring slave select low
// send data
// then bring slave select high
// Pin 13 is SCK
// Pin 12 is MI-SO
// Pin 11 is MO-SI

void SPI_Setup() {
  pinMode(SLAVE_SELECT_PIN, INPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  // This means that data is read on the rising edge of the clock signal
  SPI.setDataMode(SPI_MODE0);
}

void setup() {
  // put your setup code here, to run once:
  SPI_Setup();
  Serial.begin(9600);
}

void loop() {
  if (!digitalRead(SLAVE_SELECT_PIN)) {
    S
  }
}*/
#include <SPI.h>
char buff [50];
volatile byte indx;
volatile boolean process;

void setup (void) {
   Serial.begin (9600);
   pinMode(MISO, OUTPUT); // have to send on master in so it set as output
   SPCR |= _BV(SPE); // turn on SPI in slave mode
   indx = 0; // buffer empty
   process = false;
   SPI.attachInterrupt(); // turn on interrupt
}

ISR (SPI_STC_vect) // SPI interrupt routine 
{ 
   byte c = SPDR; // read byte from SPI Data Register
   if (indx < sizeof buff) {
      buff [indx++] = c; // save data in the next index in the array buff
      if (c == '\r') //check for the end of the word
      process = true;
   }
}

void loop (void) {
   if (process) {
      process = false; //reset the process
      Serial.println (buff); //print the array on serial monitor
      indx= 0; //reset button to zero
   }
}
