#include <SPI.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "globals.h"

#define ARRAY_LEN 60
#define SAMPLE_INTERVAL 20
#define CO2_INPUT_PIN A0
#define H2O_INPUT_PIN A1
#define DATA_FORMAT_STRING "CO2: %d, H2O:%d\n"
#define NUM_RELAYS 8

uint16_t average(uint16_t list[], uint8_t len) {
  uint8_t i;
  uint16_t total = 0;
  for (i = 0; i < len; i++) {
    total = total + list[i];
  }
  return (total / len);
}

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

uint8_t relayPorts[] = {relay_0, relay_1, relay_2, relay_3,
                       relay_4, relay_5, relay_6, relay_7 };

// Function for sending data over the SPI connection
void sendData(uint16_t CO2, uint16_t H2O) {
  char data [25];
  sprintf(data, DATA_FORMAT_STRING, CO2, H2O);
  int len = strlen(data);
  uint16_t i;
  for (i = 0; i < len; i++) {
    Serial.print(data[i]);
    //SPI.transfer (data[i]);
  }
}

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

enum stateMachine_st_t {
  init_st,
  waitInstructions_st,
  readData_st,
  transmitData_st,
  relays_st,
  final_st
};

static enum stateMachine_st_t currentState, previousState;
static uint16_t CO2sensorValue, H2OsensorValue;
static uint16_t CO2Values[ARRAY_LEN];
static uint16_t H2OValues[ARRAY_LEN];
static uint8_t iterator;
static uint16_t counter;

void stateMachine_Init() {
  currentState = previousState = init_st;
  CO2sensorValue = 0;
  H2OsensorValue = 0;
  for (int i = 0; i < ARRAY_LEN; i++) {
    CO2Values[i] = 0;
    H2OValues[i] = 0;
  }
  counter = 0;
}

void stateMachine_tick() {
  // State transitions
  // printState();
  counter++;
  switch (currentState) {
    case init_st:
      currentState = waitInstructions_st;
      break;
    case waitInstructions_st:
      if (Serial.available() > 0) {
        if(Serial.read() == '1') {
          currentState = transmitData_st;
        }
        if(Serial.read() == '2') {
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
      /* This is the code for decoding the SPI instructions
      if (process) {
        process = false; //reset the process
        // TODO: Parse SPI Input Instructions
        indx = 0; //reset button to zero
      }
      */
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
      CO2sensorValue = analogRead(CO2_INPUT_PIN);
      H2OsensorValue = analogRead(H2O_INPUT_PIN);
      iterator++;
      if (iterator >= ARRAY_LEN) {
        iterator = 0;
      }
      CO2Values[iterator] = CO2sensorValue;
      H2OValues[iterator] = H2OsensorValue;
      break;
    case transmitData_st:
      sendData(average(CO2Values, ARRAY_LEN), average(H2OValues, ARRAY_LEN));
      break;
    case relays_st:
      // Use this section of state actions to update the relays
      // I think it would be best to have a binary variable
      // and this just reads the states and writes the pins low or high
      // if they have changed. 
      // updateRelays();
      break;
    case final_st:
      break;
    default:
      break;
  }
}
