//
// Includes
//

#include <Wire.h>
#include "sonar.h"
#include "average.h"

//
// Defines
//

//#define DEBUG
#define I2C_ADDRESS        0x72

// Types
struct block_mem {
  float raw_sonar_value[NUM_SONAR_SENSORS];
  float avg_sonar_value[NUM_SONAR_SENSORS];
  unsigned long tick_period;
  char tick_count;
};

// I2C Memory
byte i2c_memory_offset = 0;
volatile struct block_mem i2c_memory;

// Sonar sensor averaging
int sensor_index = 0;
Average<float> *sensor_values[NUM_SONAR_SENSORS];

//
// Main program
//

void setup() {
  // Setup I2C
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveHandler);
  Wire.onRequest(requestHandler);
  
#ifdef DEBUG
  Serial.begin(57600);
#endif

  // Setup sonar
  sonar_init();
  
  // Setup interrupts
  encoder_init(&i2c_memory.tick_count, &i2c_memory.tick_period);
}

void loop() {
  // Ping current sensor and write to i2c memory
  i2c_memory.raw_sonar_value[sensor_index] = sonar_ping(sensor_index);
  
  // Add new value to averaging
  sensor_values[sensor_index]->pushValue(i2c_memory.raw_sonar_value[sensor_index]);
  
  // Write averaged value to i2c memory
  i2c_memory.avg_sonar_value[sensor_index] = sensor_values[sensor_index]->getAverage();
  
#ifdef DEBUG
  // Print byte to serial
  Serial.print("Sensor ");
  Serial.print(sensor_index, DEC);
  Serial.print(": ");
  Serial.println(sensor_values[sensor_index]->getAverage(), DEC);
  
  delay(250);
#endif
  
  // Increment index
  sensor_index = (sensor_index + 1) % NUM_SONAR_SENSORS;
}

// Handle request for next address
void receiveHandler(int numBytes) {
  byte *bVal = (byte *) &i2c_memory;
  
  // Read first byte and use as memory offset
  i2c_memory_offset = Wire.read();
  
  // Write bytes into memory
  while (Wire.available() > 0) {
    if (i2c_memory_offset < sizeof(block_mem)) {
      bVal[i2c_memory_offset++] = Wire.read();
    } else {
      Wire.read(); 
    }
  }
}

// Handle request for data
void requestHandler() {
  byte *bVal = (byte *) &i2c_memory;
  
  // Write current byte to i2c bus;
  if (i2c_memory_offset < sizeof(block_mem)) {
    Wire.write(bVal[i2c_memory_offset]);
  } else {
    Wire.write(0x00); 
  }
  
  // Clear tick count on read
  if (&bVal[i2c_memory_offset] == (byte *)&i2c_memory.tick_count) {
    i2c_memory.tick_count = 0;
  }
  
  // Increment offset
  i2c_memory_offset++;
}
