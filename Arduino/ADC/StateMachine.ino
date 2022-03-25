#include <SPI.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "globals.h"

// Uncomment these lines to compile with the given options
// #define DEBUG 1  // Debug printout
// #define SPI 1  // Communicate over SPI
#define SERIAL 1  // Communicate over serial
// #define RELAYS 1  // Should run with relay updates

#define ARRAY_LEN 60
#define SAMPLE_INTERVAL 20
#define CO2_A_INPUT_PIN A0
#define C02_B_INPUT_PIN A1
#define DATA_FORMAT_STRING "CO2A: %d, CO2B:%d\n"
#define NUM_RELAYS 8

#define SEND_DATA_BUFFER_SIZE 30

#define SEND_DATA_MSG '1'
#define UPDATE_RELAYS_MSG '2'

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
static uint16_t counter;

const uint8_t relayPorts[] = {relay_0, relay_1, relay_2, relay_3,
                       relay_4, relay_5, relay_6, relay_7 };

// Function that computes the average if the given array
uint16_t average(uint16_t list[], uint8_t len) {
  uint8_t i;
  uint16_t total = 0;
  for (i = 0; i < len; i++) {
    total = total + list[i];
  }
  return (total / len);
}

// Function for sending data over the SPI connection
void sendData(uint16_t CO2, uint16_t H2O) {
  char data [SEND_DATA_BUFFER_SIZE];
  sprintf(data, DATA_FORMAT_STRING, CO2, H2O);
  int len = strlen(data);
  uint16_t i;
  for (i = 0; i < len; i++) {
    #ifdef SERIAL
    Serial.print(data[i]);
    #endif
    #ifdef SPI
    SPI.transfer (data[i]);
    #endif
  }
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

#ifdef RELAYS
void updateRelays(char* relays) {
  // Format of the string will be 8 1's and 0's saying the state
  // of each relay
  uint8_t i;
  for (i = 0; i < NUM_RELAYS; i++) {
    if (relays[i] == '1') {
      digitalWrite(relayPorts[i], HIGH);
    }
    else if (relays[i] == '0') {
      digitalWrite(relayPorts[i], LOW);
    }
    else {
      digitalWrite(relayPorts[i], LOW);
    }
  }
}
#endif

void stateMachine_Init() {
  currentState = previousState = init_st;
  CO2ASensorValue = 0;
  CO2BSensorValue = 0;
  for (int i = 0; i < ARRAY_LEN; i++) {
    CO2AValues[i] = 0;
    CO2BValues[i] = 0;
  }
  counter = 0;
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
      #ifdef SERIAL
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
        counter = 0;
      }
      else {
        currentState = waitInstructions_st;
      }
      #endif
      #ifdef SPI
      if (globals_getProcessFlag()) {
        char SPIMSG[GLOBALS_DATA_BUFFER_SIZE];
        uint8_t elementCount = globals_bufferElementCount();
        for (uint8_t i = 0; i < elementCount; i++) {
          SPIMSG[i] = globals_removeDataFromBuffer();
        }

        if(SPIMSG[0] == SEND_DATA_MSG) {
          currentState = transmitData_st;
        }
        if(SPIMSG[0] == UPDATE_RELAYS_MSG) {
          currentState = relays_st;
        }
        globals_setProcessFlag(false);
      }
      else if (counter >= SAMPLE_INTERVAL) {
        currentState = readData_st;
        counter = 0;
      }
      else {
        currentState = waitInstructions_st;
      }
      #endif
      break;

    case readData_st:
      currentState = waitInstructions_st;
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
      #ifdef RELAYS
      updateRelays();
      #endif
      break;

    case final_st:
      break;

    default:
      break;
  }
}
