#include "PID.h"

PID::PID(uint8_t kp, uint8_t ki, uint8_t kd){
  pinMode(MOTOR_OUT, OUTPUT);
  analogWrite(MOTOR_OUT, MOTOR_STOP); 
  error = new int[3];
  outputPID = new uint16_t[3];

  memset(error, 0, sizeof(error)/sizeof(*error));
  memset(outputPID, 0, sizeof(outputPID)/sizeof(*outputPID));
}

PID::~PID(){
  delete [] error;
}

int PID::ComputePID(int desiredPosition, int currentPosition){    
  error[CURRENT] = desiredPosition - currentPosition;  
  int pidOut = (kp*error[CURRENT]);
  pidOut = constrain(pidOut, -PWM_MID, PWM_MID);
  outputPID[CURRENT] = PWM_MID + (pidOut);
  outputPID[CURRENT] = constrain(outputPID[CURRENT], PWM_MIN, PWM_MAX);
//  Serial.print(error[CURRENT]);
//  Serial.print("\t");
//  Serial.print(pidOut);
//  Serial.print("\t");
//  Serial.print(currentPosition);
//  Serial.print("\t");
//  Serial.println(desiredPosition);
  
  return outputPID[CURRENT];
}

