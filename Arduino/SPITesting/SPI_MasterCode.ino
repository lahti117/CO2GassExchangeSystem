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
void transmitData(char command) {
  digitalWrite(SLAVE_SELECT_PIN, LOW);
  delay(100);
  // Send command
  char data = 0x01;
  SPI.transfer(command); // command
  SPI.transfer(data);
  SPI.transfer(data);
  SPI.transfer(data);
  SPI.transfer(data);
  Serial.println("commandSent/Data");
  delay(100);
  digitalWrite(SLAVE_SELECT_PIN, HIGH);
}

void SPI_Setup() {
  pinMode(SLAVE_SELECT_PIN, OUTPUT);
  digitalWrite(SLAVE_SELECT_PIN, HIGH);
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
  // put your main code here, to run repeatedly:
  delay(2000);
  transmitData('1');
}*/
#include <SPI.h>

void setup (void) {
   Serial.begin(9600); //set baud rate to 115200 for usart
   digitalWrite(SS, HIGH); // disable Slave Select
   SPI.begin ();
   SPI.setClockDivider(SPI_CLOCK_DIV8);//divide the clock by 8
}

void loop (void) {
   char c;
   digitalWrite(SS, LOW); // enable Slave Select
   // send test string
   //SPI.transfer ('1');
   //Serial.println('1');
   for (const char * p = "Hello, world!\r" ; c = *p; p++) 
   {
      SPI.transfer (c);
      Serial.print(c);
   }
   digitalWrite(SS, HIGH); // disable Slave Select
   Serial.println();
   delay(2000);
}
