#include <SPI.h>
#include <stdint.h>

#define ARRAY_LEN 60
#define SAMPLE_INTERVAL 20
#define CO2_INPUT_PIN A0
#define H2O_INPUT_PIN A1

uint16_t average(uint16_t list[], uint8_t len) {
  uint8_t i;
  uint16_t total = 0;
  for (i = 0; i < len; i++) {
    total = total + list[i];
  }
  return (total / len);
}

/*void printState() {
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
}*/

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
      // Serial.print("data logged: ");
      // Serial.println(CO2sensorValue);
      break;
    case transmitData_st:
      Serial.print("CO2Value: ");
      Serial.print(CO2sensorValue);
      Serial.print(" Average: ");
      Serial.println(average(CO2Values, ARRAY_LEN));
      Serial.print("H2OValue: ");
      Serial.print(H2OsensorValue);
      Serial.print(" Average: ");
      Serial.println(average(H2OValues, ARRAY_LEN));
      break;
    case relays_st:
      // Use this section of state actions to update the relays
      // I think it would be best to have a binary variable
      // and this just reads the states and writes the pins low or high
      // if they have changed. 
      break;
    case final_st:
      break;
    default:
      break;
  }
}
