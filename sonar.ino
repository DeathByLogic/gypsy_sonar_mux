//
// Includes
//

#include "sonar.h"


//
// Global Variables
//

const int sensor_rx[] = {4, 9};
const int sensor_pw[] = {5, 10};

void sonar_init() {
  // Configure sonar pins
  for (int i = 0; i < NUM_SONAR_SENSORS; i++) {
    digitalWrite(sensor_rx[i], LOW);
    pinMode(sensor_rx[i], OUTPUT);
    pinMode(sensor_pw[i], INPUT);
  }
  
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

// Re-verify our range to target... one ping only.
float sonar_ping(int index) {
  unsigned long range_time;
  	
  // Enable pinging
  digitalWrite(sensor_rx[index], HIGH);
  	
  // Read pulse time
  range_time = pulseIn(sensor_pw[index], HIGH);
  
  // Disable pinging
  digitalWrite(sensor_rx[index], LOW);
  
  return range_time / 147.0;
}
