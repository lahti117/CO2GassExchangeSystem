// This is the code that handles global variables. 
// Thes are the variables necessary for the main loop
// to pass data to the State Machine
#include <stdint.h>
#include <stdbool.h>
#ifndef GLOBALS_H
#define GLOBALS_H

#define GLOBALS_DATA_BUFFER_SIZE 60

// This is the current index in the character buffer
uint8_t indx;

// This is a boolean flag saying that there is new 
// data available in the buffer
bool process;

// Pin numbers for each of the relays
const uint8_t relay_0 = 2;
const uint8_t relay_1 = 3;
const uint8_t relay_2 = 4;
const uint8_t relay_3 = 5;
const uint8_t relay_4 = 6;
const uint8_t relay_5 = 7;
const uint8_t relay_6 = 8;
const uint8_t relay_7 = 9;

void globals_initGlobals();

void global_addDataToBuffer(uint8_t data);

uint8_t globals_removeDataFromBuffer();

uint16_t globals_bufferElementCount();

bool globals_getProcessFlag();

void globals_setProcessFlag(bool state);

#endif