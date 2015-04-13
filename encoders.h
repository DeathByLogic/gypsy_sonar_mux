#ifndef _ENCODERS_H
#define _ENCODERS_H

// Constant Definitions
#define ENC_A_PIN   2
#define ENC_B_PIN   3

#define ENC_A_INT   0
#define ENC_B_INT   1

// Type Definitions
typedef enum {
  STATE_FW_A,
  STATE_FW_B,
  STATE_FW_C,
  STATE_FW_D,
  STATE_RV_A,
  STATE_RV_B,
  STATE_RV_C,
  STATE_RV_D
} EncoderState;

// Function Prototypes
void encoder_init(volatile long *, volatile unsigned long *);
void encoder_state();

#endif
