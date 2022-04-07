// This is the code that handles global variables. 
// Thes are the variables necessary for the main loop
// to pass data to the State Machine
#include "globals.h"

#define DATA_BUFFER_SIZE GLOBALS_DATA_BUFFER_SIZE
#define DATA_BUFFER_DATA_SIZE DATA_BUFFER_SIZE + 1



// This implements a dedicated circular buffer for storing values
// from the ADC until they are read and processed by detector().
// adcBuffer_t is similar to a queue.
typedef struct {
  uint16_t indexIn;      // New values go here.
  uint16_t indexOut;     // Pull old values from here.
  uint16_t elementCount; // Number of elements in the buffer.
  uint16_t bufferLength;
  uint8_t data[DATA_BUFFER_DATA_SIZE]; // Values are stored here.
} dataBuffer_t;

volatile static dataBuffer_t dataBuffer;

// Initialize the adc buffer
void dataBufferInit() {
  uint16_t i;
  dataBuffer.indexIn = dataBuffer.indexOut = 0;
  dataBuffer.bufferLength = DATA_BUFFER_DATA_SIZE;
}

void globals_initGlobals() {
    dataBufferInit();
    process = false;
}

// Increment the index in and check for wrap around.
uint16_t inc_indexIn() {
  dataBuffer.indexIn++;
  // Checks the if the IndexIn has maxed out of the array and if it needs to
  // wrap around to index 0.
  if (dataBuffer.indexIn >= DATA_BUFFER_DATA_SIZE) {
    dataBuffer.indexIn = 0;
  }
  return dataBuffer.indexIn;
}

// Increment the index out and check for wrap around.
uint16_t inc_indexOut() {
  dataBuffer.indexOut++;
  // Checks if IndexOut has reached the max buffer size and if it needs to wrap
  // back to index 0.
  if (dataBuffer.indexOut >= DATA_BUFFER_DATA_SIZE) {
    dataBuffer.indexOut = 0;
  }
  return dataBuffer.indexOut;
}

// This adds data to the ADC queue. Data are removed from this queue and used by
// the detector.
void global_addDataToBuffer(uint8_t data) {
  dataBuffer.data[dataBuffer.indexIn] = data;
  dataBuffer.elementCount++;
  inc_indexIn();
  // Make sure that the arrays aren't empty.
  if (dataBuffer.elementCount != 0) {
    // See if the indexes are at the same place. If they are, move the Index out
    // up.
    if (dataBuffer.indexIn == dataBuffer.indexOut) {
      inc_indexOut();
      dataBuffer.elementCount--;
    }
  }
}

// This removes a value from the ADC buffer.
uint8_t globals_removeDataFromBuffer() {
  // Checks if there are elements in the queue.
  if (dataBuffer.elementCount == 0) {
    return 0;
  } else {
    uint16_t returnData = dataBuffer.data[dataBuffer.indexOut];
    dataBuffer.elementCount--;
    inc_indexOut();
    return returnData;
  }
}

// This returns the number of values in the ADC buffer.
uint16_t globals_bufferElementCount() { return dataBuffer.elementCount; }

bool globals_getProcessFlag() { return process; }

void globals_setProcessFlag(bool state) { process = state; }
