// This is the code that handles global variables. 
// Thes are the variables necessary for the main loop
// to pass data to the State Machine
#include "globals.h"

#define DATA_BUFFER_SIZE 100000
#define DATA_BUFFER_DATA_SIZE DATA_BUFFER_SIZE + 1

// This implements a dedicated circular buffer for storing values
// from the ADC until they are read and processed by detector().
// adcBuffer_t is similar to a queue.
typedef struct {
  uint32_t indexIn;      // New values go here.
  uint32_t indexOut;     // Pull old values from here.
  uint32_t elementCount; // Number of elements in the buffer.
  uint32_t bufferLength;
  uint32_t data[DATA_BUFFER_DATA_SIZE]; // Values are stored here.
} dataBuffer_t;

volatile static dataBuffer_t dataBuffer;

// Initialize the adc buffer
void dataBufferInit() {
  uint32_t i;
  dataBuffer.indexIn = dataBuffer.indexOut = 0;
  dataBuffer.bufferLength = DATA_BUFFER_DATA_SIZE;
}

// Increment the index in and check for wrap around.
uint32_t inc_indexIn() {
  dataBuffer.indexIn++;
  // Checks the if the IndexIn has maxed out of the array and if it needs to
  // wrap around to index 0.
  if (dataBuffer.indexIn >= DATA_BUFFER_DATA_SIZE) {
    dataBuffer.indexIn = 0;
  }
  return dataBuffer.indexIn;
}

// Increment the index out and check for wrap around.
uint32_t inc_indexOut() {
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
void isr_addDataToAdcBuffer(uint32_t adcData) {
  adcBuffer.data[adcBuffer.indexIn] = adcData;
  adcBuffer.elementCount++;
  inc_indexIn();
  // Make sure that the arrays aren't empty.
  if (adcBuffer.elementCount != 0) {
    // See if the indexes are at the same place. If they are, move the Index out
    // up.
    if (adcBuffer.indexIn == adcBuffer.indexOut) {
      inc_indexOut();
      adcBuffer.elementCount--;
    }
  }
}
/*
// This removes a value from the ADC buffer.
uint32_t isr_removeDataFromAdcBuffer() {
  // Checks if there are elements in the queue.
  if (adcBuffer.elementCount == 0) {
    return 0;
  } else {
    uint32_t returnData = adcBuffer.data[adcBuffer.indexOut];
    adcBuffer.elementCount--;
    inc_indexOut();
    return returnData;
  }
}

// This returns the number of values in the ADC buffer.
uint32_t isr_adcBufferElementCount() { return adcBuffer.elementCount; }
*/