#include "PID.h"

PID::PID(uint8_t kp, uint8_t ki, uint8_t kd) {
  pinMode(MOTOR_OUT, OUTPUT);
  analogWrite(MOTOR_OUT, MOTOR_STOP);
  error = new long[3];
  outputPID = new long[3];

  memset(error, 0, sizeof(error) / sizeof(*error));
  memset(outputPID, 0, sizeof(outputPID) / sizeof(*outputPID));
}

PID::~PID() {
  delete [] error;
}

long int PID::ComputePID(int desiredPosition, int currentPosition) {
  int Kp = 500;
  int Ki = 0;

  dt = (int) (millis() - timeBefore);
  error[CURRENT] = (desiredPosition - currentPosition);
  error[PREVIOUS] += error[CURRENT];

  error[PREVIOUS] = constrain(error[PREVIOUS], -10000, 10000);

  long outputKP = Kp * error[CURRENT];
  long outputKi = Ki * error[PREVIOUS] * dt;
  long output = outputKP + outputKi;
  outputPID[PREVIOUS] = output/1000;
  outputPID[PREVIOUS] = constrain(outputPID[PREVIOUS], -PWM_MID, PWM_MID);

  int midPWM = 128;
  if(outputPID[PREVIOUS] > 0){
    midPWM = 128 + 20;
  } else if (outputPID[PREVIOUS] < 0){
    midPWM = 128 - 20;
  } else {
    midPWM = 128;
  }
  outputPID[CURRENT] = midPWM + outputPID[PREVIOUS];
  outputPID[CURRENT] = constrain(outputPID[CURRENT], PWM_MIN, PWM_MAX);
  timeBefore = millis();

  //  error[CURRENT] = (desiredPosition - currentPosition);
  //  error[PREVIOUS] += (error[CURRENT] * 5);
  //  //error[PREVIOUS] = constrain(error[PREVIOUS], -20, 20);
  //  error[PREVIOUS_2] = (error[PREVIOUS]);
  ////  error[PREVIOUS_2] = constrain(error[PREVIOUS_2], -20.0f, 20.0f);
  //  float pidOut = (2000*error[CURRENT] + 1000*error[PREVIOUS_2])/1000;
  //  pidOut = constrain(pidOut, -PWM_MID, PWM_MID);
  //  outputPID[CURRENT] = PWM_MID + ((int)pidOut);
  //  outputPID[CURRENT] = constrain(outputPID[CURRENT], PWM_MIN, PWM_MAX);
  //  Serial.print(error[CURRENT]);
  //  Serial.print("\t");
  //  Serial.print(pidOut);
  //  Serial.print("\t");
  //  Serial.print(currentPosition);
  //  Serial.print("\t");
  //  Serial.println(desiredPosition);

  return outputPID[CURRENT];
}

