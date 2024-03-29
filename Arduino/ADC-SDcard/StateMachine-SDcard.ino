#include <stdint.h>
#include <stdio.h>
#include <string.h>

// #define DEBUG

#define ARRAY_LEN 60
#define SAMPLE_INTERVAL 20
#define LOG_DATA_INTERVAL 60
#define CO2_A_INPUT_PIN A0
#define C02_B_INPUT_PIN A1

// #define DATA_FORMAT_STRING "CO2A: %d, CO2B:%d\n"
#define COLUMN_DATA_TITLES "CO2 Cell A, CO2 Cell B, Vcc(mV)\n"
#define DATA_FORMAT_STRING "%d,%d,%d\n"
#define NUM_RELAYS 8

#define SEND_DATA_BUFFER_SIZE 30

#define SEND_DATA_MSG '1'
#define UPDATE_RELAYS_MSG '2'

#define FILE_NAME "data.csv"
#define CHIP_SELECT_PIN 4
#define FILE_NAME_SIZE 12

enum stateMachine_st_t {
  init_st,
  waitInstructions_st,
  readData_st,
  transmitData_st,
  relays_st,
  final_st
};

static enum stateMachine_st_t currentState, previousState;
static uint16_t CO2ASensorValue, CO2BSensorValue;
static uint16_t CO2AValues[ARRAY_LEN];
static uint16_t CO2BValues[ARRAY_LEN];
static uint8_t iterator;
static uint8_t counter;
static uint8_t logDataCounter;
static uint16_t minuteCounter;
char fileName [FILE_NAME_SIZE]; //Use this file name later for dynamic naming

//File dataFile;

#ifdef DEBUG
void printState() {
  if (previousState != currentState) {
    previousState = currentState;
    switch (currentState) {
      case init_st:
        Serial.println("init_st");
        break;
      case waitInstructions_st:
        Serial.println("waitInstructions_st");
        break;
      case readData_st:
        Serial.println("readData_st");
        break;
      case transmitData_st:
        Serial.println("transmitData_st");
        break;
      case relays_st:
        Serial.println("relays_st");
        break;
      case final_st:
        Serial.println("final_st");
        break;
      default:
        break;
    }
  }
}
#endif

// Function that uses registers and the internal 1.1V reference
// to read the supply voltage. Used for calculating exact value
// read by the analog inputs. Output is in (mV)
// There is a delay in this function so it should only be run
// once at setup.
uint16_t readVcc() {
  uint16_t result;
  // Read 1.1V reference against AVcc
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
  delay(2); // Wait for VCC to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  result = (1126400L / result)+100; // Calculate Vcc (in mV); 1126400 = 1.1*1024*1000 and 100mV offset correction
  return result;
}

// Function that computes the average if the given array
uint16_t average(uint16_t list[], uint8_t len) {
  uint8_t i;
  uint16_t total = 0;
  for (i = 0; i < len; i++) {
    total = total + list[i];
  }
  return (total / len);
}

// Function for sending data over the SD connection
void sendData(uint16_t CO2, uint16_t H2O) {
  char data [SEND_DATA_BUFFER_SIZE];
  sprintf(data, DATA_FORMAT_STRING, CO2, H2O, readVcc());
  int len = strlen(data);
  uint16_t i;
  File dataFile;
  dataFile = SD.open(FILE_NAME, FILE_WRITE);
  for (i = 0; i < len; i++) {
    dataFile.print(data[i]);
    Serial.print(data[i]);
  }
  dataFile.close();
}

// Function that reads the data and logs it in a table
void readData() {
  CO2ASensorValue = analogRead(CO2_A_INPUT_PIN);
  CO2BSensorValue = analogRead(C02_B_INPUT_PIN);
  iterator++;
  if (iterator >= ARRAY_LEN) {
    iterator = 0;
  }
  CO2AValues[iterator] = CO2ASensorValue;
  CO2BValues[iterator] = CO2BSensorValue;
}

void setupSDCard () {
  if (!SD.begin(CHIP_SELECT_PIN)) {
    Serial.println("SD Card initialization failed!");
  }
  else {
    Serial.println("SD Card initialization Success!");
    if (SD.exists(FILE_NAME)){
      Serial.println("data.csv exists.");
      Serial.println("removing data.csv and creating new one");
      SD.remove(FILE_NAME);
    }
    else {
      Serial.println("data.csv doesn't exist.");
      Serial.println("creating data.csv");
    }
    char titleString[40];
    sprintf(titleString, "%s", COLUMN_DATA_TITLES);
    uint8_t len = strlen(titleString);
    File dataFile;
    dataFile = SD.open(FILE_NAME, FILE_WRITE);
    for (uint8_t i = 0; i < len; i++) {
      dataFile.print(titleString[i]);
    }
    dataFile.close();
  }
}

void stateMachine_Init() {
  currentState = previousState = init_st;
  CO2ASensorValue = 0;
  CO2BSensorValue = 0;
  for (int i = 0; i < ARRAY_LEN; i++) {
    CO2AValues[i] = 0;
    CO2BValues[i] = 0;
  }
  counter = 0;
  logDataCounter = 0;
  minuteCounter = 0;
  setupSDCard ();
}

void stateMachine_tick() {
  // State transitions
  #ifdef DEBUG
  printState();
  #endif
  counter++;
  switch (currentState) {
    case init_st:
      currentState = waitInstructions_st;
      break;

    case waitInstructions_st:
      if (Serial.available() > 0) {
        if(Serial.read() == SEND_DATA_MSG) {
          currentState = transmitData_st;
        }
        if(Serial.read() == UPDATE_RELAYS_MSG) {
          currentState = relays_st;
        }
      }
      else if (counter >= SAMPLE_INTERVAL) {
        currentState = readData_st;
        logDataCounter++;
        counter = 0;
      }
      else {
        currentState = waitInstructions_st;
      }
      break;

    case readData_st:
      if (logDataCounter >= 60) {
        logDataCounter = 0;
        currentState = transmitData_st;
      }
      else {
        currentState = waitInstructions_st;
      }
      break;

    case transmitData_st:
      currentState = waitInstructions_st;
      break;
      
    case relays_st:
      currentState = waitInstructions_st;
      break;

    case final_st:
      break;

    default:
      break;
  }
  // State Actions
  switch (currentState) {
    case init_st:
      break;

    case waitInstructions_st:
      break;

    case readData_st:
      readData();
      break;

    case transmitData_st:
      sendData(average(CO2AValues, ARRAY_LEN), average(CO2BValues, ARRAY_LEN));
      break;

    case relays_st:
      break;

    case final_st:
      break;

    default:
      break;
  }
}
