#include "encoders.h"

//
// Global variables
//

// Encoder Counts
volatile long *encoder_count;
volatile unsigned long *encoder_period;

// Init encoder pins and configure interrupts
void encoder_init(volatile long *count_reg, volatile unsigned long *period_reg) {
  // Set pointer to count & period register
  encoder_count = count_reg;
  encoder_period = period_reg;
  
  // Change encoder pins to input
  pinMode(ENC_B_PIN, INPUT);
  pinMode(ENC_A_PIN, INPUT);
  
  // Attach interrupt pins to encoder functions
  attachInterrupt(ENC_A_INT, encoder_state, CHANGE);
  attachInterrupt(ENC_B_INT, encoder_state, CHANGE);
}

// Interrupt service routine for right wheel encoder
void encoder_state() {
  static EncoderState current_state = STATE_FW_A;
  static EncoderState next_state = STATE_FW_A;
  static unsigned long prev_time;
  
  // state of encoder sensor
  boolean A;
  boolean B;

  // read current state of wheel sensors
  A = digitalRead(ENC_A_PIN);
  B = digitalRead(ENC_B_PIN);
  
  // update state based on new wheel sensor reading
  switch (current_state) {
    case STATE_FW_A:
    case STATE_RV_A:
      if (!A & B) {
        next_state = STATE_FW_B;
      } else if (A & !B) {
        next_state = STATE_RV_D;
      } else {
        next_state = current_state;
      }
      
      break;
    case STATE_FW_B:
    case STATE_RV_B:
      if (!A & !B) {
        next_state = STATE_FW_C;
      } else if (A & B) {
        next_state = STATE_RV_A;
      } else {
        next_state = current_state;
      }
      
      break;
    case STATE_FW_C:
    case STATE_RV_C:
      if (A & !B) {
        next_state = STATE_FW_D;
      } else if (!A & B) {
        next_state = STATE_RV_B;
      } else {
        next_state = current_state;
      }
      
      break;
    case STATE_FW_D:
    case STATE_RV_D:
      if (A & B) {
        next_state = STATE_FW_A;
      } else if (!A & !B) {
        next_state = STATE_RV_C;
      } else {
        next_state = current_state;
      }
      
      break;
    default:
      next_state = STATE_FW_A;
  }
  
  // determine if state has changed and if moving forward or reverse
  if (next_state != current_state) {
    if ((next_state == STATE_FW_A) || (next_state == STATE_FW_B) || (next_state == STATE_FW_C) || (next_state == STATE_FW_D)) {
      *encoder_count += 1;
    } else {
      *encoder_count -= 1;
    }
    
    // Determine tick period
    *encoder_period = micros() - prev_time;
    prev_time = micros();
  } else {
    *encoder_count += 0;
  }
  
  // Change States
  current_state = next_state;
}
