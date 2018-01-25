#include <TimerOne.h>
#include <SPI.h>
#include <EEPROM.h>
#define SS 10
#define MISO 12
#define CLK 13
#define PWMOUT 5
#define ADC_BITS 12
#define ZERO_POS 128
#define MAXPWM 220
#define MINPWM 40
#define LOWPASS 0.1f
#define HIGHPASS (1.0f - LOWPASS)
#define COUNTPERU 0.01320451944240645634629493763756f
#define RAWMAXHEIGHTDIFF 3066.75f
#define ERROR_DEADZONE 0.5f
#define PWM_DEADZONE_CHECK 10

uint8_t adcCounter[ADC_BITS];
float adcRaw;
float adcValue;
float adcCalibrated;
uint8_t pulseCounter;
bool timeToRead = true;
uint8_t timerCounter;
long timeBefore;
bool saveToEEProm = false;
bool saveTemp = false;
float adcOffset;
float rackHeigth;
float calibratedMaxHeight;
float RAWMAXHEIGHT;
float initialOffset;
bool initialSave = false;
uint8_t pwmOut = 127;
float integralError;
uint8_t calibrationStatus;
uint8_t delayTimer;
bool userTimeCounterCheck = false;
float kp = 100.0f, kd = 0.0f, ki = 0.0f;
float desiredPosition = 0.0f;
char incomingChars[2];
bool userInputTimeOut = false;
long timeBeforeForUserTimeOut = 0L;
long timeForUserTimeOut = 0L;
char input[10];
bool toggle;
uint8_t debugMode = 0;
typedef union Float2Byte {
  float f;
  uint8_t b[4];
};

Float2Byte whatWeReceived;
Float2Byte iFloat2Bytes;
Float2Byte Float2Bytes;
Float2Byte maximumHeightCommand;

void SPIRead() {
  digitalWrite(SS, LOW);
  uint8_t result1 = SPI.transfer(0xFF);
  uint8_t result2 = SPI.transfer(0xFF);
  unsigned int resultRaw = (result1 << 8 | result2);
  unsigned int result = resultRaw >> 4;

  adcRaw = ((HIGHPASS * adcRaw) + (LOWPASS * (float)result));
  adcValue = adcRaw - initialOffset;
  adcCalibrated = adcValue - adcOffset;

  rackHeigth = adcCalibrated * COUNTPERU;
  calibratedMaxHeight = (RAWMAXHEIGHT - (adcOffset + initialOffset)) * COUNTPERU;

  digitalWrite(SS, HIGH);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(PWMOUT, OUTPUT);
  pinMode(SS, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(CLK, OUTPUT);
  digitalWrite(SS, LOW);
  delay(5);
  digitalWrite(SS, HIGH);
  digitalWrite(CLK, LOW);
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  delay(5);

  for (uint8_t i = 0; i < 4; ++i) {
    Float2Bytes.b[i] = EEPROM.read(i);
    iFloat2Bytes.b[i] = EEPROM.read(i + 4);
  }

  adcOffset = Float2Bytes.f;
  initialOffset = iFloat2Bytes.f;

  for (uint8_t i = 0; i < 100; ++i) {
    SPIRead();
  }

  desiredPosition = rackHeigth;
}

int PID(float desiredPos, float sensor, float dT, float kp, float kd, float ki) {
  int outputRaw, output;
  float error = desiredPos - sensor;
  integralError += error * dT;
  integralError = constrain(integralError, -100.0f, 100.0f);

  output = ZERO_POS + ((int)((kp * error) + (ki * integralError)));
  output = constrain(output, MINPWM, MAXPWM);
  if (sensor <= 0.0f && desiredPos <= 0.0f) {
    output = ZERO_POS;
  }  

  return output;
}

void loop() {
  if (debugMode == 1) {
    if (Serial.available()) {
      char incoming = Serial.read();      
      switch (incoming) {
        case 'A':
          saveToEEProm = true;
          break;
        case 'S':
          saveTemp = true;
          break;
        case 'P':
          initialSave = true;
          break;
        case 'q':
          pwmOut += 1;
          if (pwmOut > 254) {
            pwmOut = 255;
          }
          break;
        case 'e':
          pwmOut -= 1;
          if (pwmOut < 1) {
            pwmOut = 0;
          }
          break;
        case 'w':
          pwmOut = ZERO_POS;
          break;

        case 'u':
          desiredPosition += 1.0f;
          break;
        case 'i':
          desiredPosition -= 1.0f;
          break;
        case 'k':
          kp += 0.1f;
          break;
        case 'l':
          kp -= 0.1f;
          break;
        case 'n':
          ki += 0.05f;
          break;
        case 'm':
          ki -= 0.05f;
          break;
        default:
          break;
      }
    }
    Serial.print(rackHeigth);
    Serial.print("\t");
    Serial.print(adcValue);
    Serial.print("\t");
    Serial.print(pwmOut);
    Serial.print("\t");
    Serial.print(adcOffset);
    Serial.print("\t");
    Serial.print(adcRaw);
    Serial.print("\t");
    Serial.println(calibratedMaxHeight);
  }

  if (millis() - timeBefore >= 5) {
    RAWMAXHEIGHT = RAWMAXHEIGHTDIFF + initialOffset;
    digitalWrite(SS, LOW);
    uint8_t result1 = SPI.transfer(0xFF);
    uint8_t result2 = SPI.transfer(0xFF);
    unsigned int resultRaw = (result1 << 8 | result2);
    unsigned int result = resultRaw >> 4;

    adcRaw = ((HIGHPASS * adcRaw) + (LOWPASS * (float)result));
    adcValue = adcRaw - initialOffset;
    adcCalibrated = adcValue - adcOffset;

    rackHeigth = adcCalibrated * COUNTPERU;
    calibratedMaxHeight = (RAWMAXHEIGHT - (adcOffset + initialOffset)) * COUNTPERU;

    if (desiredPosition >= calibratedMaxHeight) {
      desiredPosition = calibratedMaxHeight;
    }

    if (desiredPosition <= 0.0f) {
      desiredPosition = 0.0;
    }

    long delta = millis() - timeBefore;
    pwmOut = PID(desiredPosition, rackHeigth, (float)(delta / 1000.0f), kp, kd, ki);

    digitalWrite(SS, HIGH);

    if (debugMode == 0) {
      if (++delayTimer >= 10) {
        Serial.write('a');
        if (Serial.available()) {
          while (Serial.read() != 0xFF);
          for (uint8_t i = 0; i < 4; ++i) {
            //input[i] =
            whatWeReceived.b[i] = Serial.read();
          }
          calibrationStatus = Serial.read();
          desiredPosition = whatWeReceived.f;
        }
        delayTimer = 0;
      }
      if (calibrationStatus == 4 || calibrationStatus == 5 || calibrationStatus == 6)
        analogWrite(PWMOUT, pwmOut);
      else
        analogWrite(PWMOUT, 128);
    } else if (debugMode == 1){
      analogWrite(PWMOUT, pwmOut); 
    }
    
    timeBefore = millis();    
  }

  if (calibrationStatus == 1 || calibrationStatus == 5) {
    saveToEEProm = true;
    calibrationStatus = 0;
  } else if (calibrationStatus == 2 || calibrationStatus == 6) {
    saveTemp = true;
    calibrationStatus = 0;
  }

  if (saveToEEProm == true) {
    desiredPosition = 0.0f;
    Float2Bytes.f = adcValue;
    for (uint8_t i = 0; i < 4; ++i) {
      EEPROM.write(i, Float2Bytes.b[i]);
    }

    delay(1);
    for (uint8_t i = 0; i < 4; ++i) {
      Float2Bytes.b[i] = EEPROM.read(i);
    }

    adcOffset = Float2Bytes.f;
    saveToEEProm = false;

    calibratedMaxHeight = (RAWMAXHEIGHT - (adcOffset + initialOffset)) * COUNTPERU;
    maximumHeightCommand.f = calibratedMaxHeight;
    Serial.write('s');
    Serial.write(0xFF);
    for (uint8_t i = 0; i < 4; ++i) {
      Serial.write(maximumHeightCommand.b[i]);
    }
  }

  if (saveTemp == true) {
    desiredPosition = adcOffset * COUNTPERU + rackHeigth;
    for (uint8_t i = 0; i < 4; ++i) {
      EEPROM.write(i, 0);
      //EEPROM.write(i + 4, 0);
    }
    delay(1);
    for (uint8_t i = 0; i < 4; ++i) {
      Float2Bytes.b[i] = EEPROM.read(i);
    }
    adcOffset = Float2Bytes.f;
    saveTemp = false;

    calibratedMaxHeight = (RAWMAXHEIGHT - (adcOffset + initialOffset)) * COUNTPERU;
    maximumHeightCommand.f = calibratedMaxHeight;
    Serial.write('s');
    Serial.write(0xFF);
    for (uint8_t i = 0; i < 4; ++i) {
      Serial.write(maximumHeightCommand.b[i]);
    }
  }

  if (initialSave == true) {
    iFloat2Bytes.f = adcRaw;
    for (uint8_t i = 4; i < 8; ++i) {
      EEPROM.write(i, iFloat2Bytes.b[i - 4]);
    }

    delay(1);
    for (uint8_t i = 4; i < 8; ++i) {
      iFloat2Bytes.b[i - 4] = EEPROM.read(i);
    }

    initialOffset = iFloat2Bytes.f;
    adcOffset = 0;
    initialSave = false;
  }
}
