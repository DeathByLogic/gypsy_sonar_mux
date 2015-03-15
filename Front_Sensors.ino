//
// Includes
//
#include <Wire.h>
#include "Average.h"

//
// Defines
//

//#define DEBUG
#define I2C_ADDRESS        0x72

#define NUM_SONAR_SENSORS  2
#define AVG_COUNT          5

#define ENC_LEFT_A         1
#define ENC_LEFT_B         2
#define ENC_RIGHT_A        3
#define ENC_RIGHT_B        4

//
// Types
//
struct block_mem {
  float raw_sonar_value[NUM_SONAR_SENSORS];
  float avg_sonar_value[NUM_SONAR_SENSORS];
  float left_speed;
  float right_speed;
  byte left_tick_count;
  byte right_tick_count;
};

//
// Global Variables
//
const int sensor_rx[] = {2, 9};
const int sensor_pw[] = {3, 10};

byte i2c_memory_offset = 0;
volatile struct block_mem i2c_memory;
Average<float> *sensor_values[NUM_SONAR_SENSORS];

void setup() {
  // Setup I2C
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveHandler);
  Wire.onRequest(requestHandler);
  
#ifdef DEBUG
  Serial.begin(57600);
#endif

  // Configure sonar pins
  for (int i = 0; i < NUM_SONAR_SENSORS; i++) {
    digitalWrite(sensor_rx[i], LOW);
    pinMode(sensor_rx[i], OUTPUT);
    pinMode(sensor_pw[i], INPUT);
  }
  
  // Configure wheel encoder pins
  pinMode(ENC_LEFT_A, INPUT);
  pinMode(ENC_LEFT_B, INPUT);
  pinMode(ENC_RIGHT_A, INPUT);
  pinMode(ENC_RIGHT_B, INPUT);
  
  // Setup interrupts
  
  // Setup sonar sensors
  for (int j = 0; j < NUM_SONAR_SENSORS; j++) {
    // Create averge objects
    sensor_values[j] = new Average<float>(AVG_COUNT);
    
    // Turn on sensor pinging
    digitalWrite(sensor_rx[j], HIGH);
    
    // Allow sensors to calibrate and take first reading
    delay(450);
    
    // Turn off sensor pinging
    digitalWrite(sensor_rx[j], LOW);
  }
}

void loop() {
  static int sensor_index = 0;
  
  // Ping current sensor and write to i2c memory
  i2c_memory.raw_sonar_value[sensor_index] = sonar_ping(sensor_rx[sensor_index], sensor_pw[sensor_index]);
  
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

// Re-verify our range to target... one ping only.
float sonar_ping(int rxPin, int pwPin) {
  unsigned long range_time;
  	
  // Enable pinging
  digitalWrite(rxPin, HIGH);
  	
  // Read pulse time
  range_time = pulseIn(pwPin, HIGH);
  
  // Disable pinging
  digitalWrite(rxPin, LOW);
  
  return range_time / 147.0;
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
    Wire.write(bVal[i2c_memory_offset++]);
  } else {
    Wire.write(0x00); 
  }
}
