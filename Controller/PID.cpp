#include "PID.h"

PID::PID(uint8_t kp, uint8_t ki, uint8_t kd){
  pinMode(MOTOR_OUT, OUTPUT);
  analogWrite(MOTOR_OUT, MOTOR_STOP); 
  error = new uint8_t[3];
  outputPID = new uint8_t[3];
}

PID::~PID(){
  delete [] error;
}

uint8_t PID::ComputePID(uint16_t desiredPosition, uint16_t currentPosition){    
  error[CURRENT] = desiredPosition - currentPosition;  
  output[CURRENT] = PWN_MID + ((kp*error[CURRENT]) >> 5);
  output[CURRENT] = constrain(output[CURRENT], PWM_MIN, PWM_MAX);

  return output[CURRENT];
}

