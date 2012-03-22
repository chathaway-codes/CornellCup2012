//Mike Collins
//Intel Cornell Cup
//PWM Test Sketch
#define ENCODER_OPTIMIZE_INTERRUPTS

//Pin definitions
#define limitSwitch 7
#define motorDIR 8
#define motorPWM 9

//PWM limits
#define limitHigh 204
#define limitLow 99

//Motor specific qualities
#define axisDegrees 130
#define encoderCounts 21000
#define acceptableDrift 70
#define DOWN LOW
#define UP HIGH

#include <Encoder.h>

//Encoder definition
Encoder myEnc(2, 3);

//P Control
int dutyCycle = 0;
int startPosition;
int currentPosition;
int travelDistance;
int rampTopDuty;
int rampDownStart;
float rampFactor;
boolean rampingUp = true;
boolean firstRamp = true;
boolean travelDirection = true;

//Serial variables
unsigned int destinationDegrees;
int destinationPosition;
byte commandFirstByte = B00000000;
byte commandLastByte = B11111111;
boolean resetCommand = false;

void setup() {
  //Calculate ramp factor for motor
  rampFactor = (float) (limitHigh - limitLow) / (float) (encoderCounts / 4);
  
  //Initialize controller
  pinMode(limitSwitch, INPUT);
  pinMode(motorDIR, OUTPUT);
  Serial.begin(115200);
  resetMotor();
}

void loop() {
  //Read encoders
  currentPosition = myEnc.read();
  
  //Get duty cycle
  dutyCycle = proportionalControl();
  
  //Destination is down
  if(currentPosition > destinationPosition) {
    digitalWrite(motorDIR, DOWN);
    analogWrite(motorPWM, dutyCycle);
  }
  
  //Destination is up
  else if(currentPosition < destinationPosition) {
    digitalWrite(motorDIR, UP);
    analogWrite(motorPWM, dutyCycle);
  }
  
  //Destination is here
  else {
    //Alert daemon, maintain position
    analogWrite(motorPWM, 0);
    Serial.write(destinationDegrees);
    while(currentPosition < (destinationPosition + acceptableDrift) && currentPosition > (destinationPosition - acceptableDrift)) {
      
      //Break to read serial
      if(Serial.available()) {
        break;
      }

      //WE NEED IT FOR SOME REASON!!!
      delayMicroseconds(1);
      currentPosition = myEnc.read();
    }
  }
}

//Called "inbetween" each loop()
void serialEvent() {
  byte inByte;
  int inDegrees;
  boolean commandComplete = false;
  
  //Decode serial protocol
  while (Serial.available()) {
    inByte = (byte) Serial.read();
  
    //Reset byte
    if((inByte & B11110000) == B11000000) {
      commandFirstByte = inByte;
      resetCommand = true;
    }
  
    //First goto byte
    else if((inByte & B11110000) == B11110000) {
      commandFirstByte = inByte;
    }
    
    //Last byte
    else if((inByte & B11000000) == B00000000 && commandFirstByte != B00000000) {
      commandLastByte = inByte;
      commandComplete = true;
      
      //Decode value
      if(resetCommand != true) {
        commandFirstByte = commandFirstByte & B00001111;
        inDegrees = (commandFirstByte << 6) | commandLastByte;
        if(inDegrees > 0 && inDegrees <= axisDegrees) {
          destinationDegrees = inDegrees;
        }
      }
    }
  }
    
  //Reset motor
  if(commandComplete && resetCommand) {
    resetMotor();
    resetCommand = false;
    commandComplete = false;
    commandFirstByte = B00000000;
    commandLastByte = B11111111;
  }
  
  //Calculate destination
  else if(commandComplete) {
    startPosition = currentPosition;
    destinationPosition = (int) ((float) (encoderCounts / axisDegrees) * destinationDegrees);
    travelDistance = destinationPosition - startPosition;
    
    //Calculate when to start ramp down
    if(abs((travelDistance / 2)) > (encoderCounts / 4)) {
      //forwards
      if(travelDistance >= 0) {
        travelDirection = true;
        rampDownStart = (int) (abs(travelDistance) - (encoderCounts / 4) + startPosition);
      }
      //backwards
      else {
        travelDirection = false;
        rampDownStart = (int) (destinationPosition + (encoderCounts / 4));
      }
    }
    else {
      //forwards
      if(travelDistance >= 0) {
        travelDirection = true;
        rampDownStart = (int) (travelDistance / 2) + startPosition;
      }
      //backwards
      else {
        travelDirection = false;
        rampDownStart = (int) destinationPosition + abs(travelDistance/2);
      }
    }
    
    //Reset conditions
    commandFirstByte = B00000000;
    commandLastByte = B11111111;
    dutyCycle = 0;
    firstRamp = true;
    commandComplete = false;
  }
}

//Proportional control
int proportionalControl() {
  int pDuty = 0;
  
  //Switch to ramp-down halfway through travel forwards
  if(rampDownStart < currentPosition && travelDirection) {
    rampingUp = false;
    if(firstRamp) {
      firstRamp = false;
      rampTopDuty = dutyCycle;
    }
  }
  //Switch to ramp-down halfway through travel backwards
  else if(rampDownStart > currentPosition && travelDirection != true) {
    rampingUp = false;
    if(firstRamp) {
      firstRamp = false;
      rampTopDuty = dutyCycle;
    }
  }
  //Still ramping up
  else {
    rampingUp = true;
  }
  
  //Calculate duty cycle
  if(rampingUp) {
    pDuty = (int) (abs(currentPosition - startPosition) * rampFactor) + limitLow;
  }
  else {
    pDuty = rampTopDuty - (int) (abs(currentPosition - rampDownStart) * rampFactor);
  }
  
  //Enforce limits
  if(pDuty <= limitLow) {
    pDuty = limitLow;
  }
  else if(pDuty >= limitHigh) {
    pDuty = limitHigh;
  }
  return pDuty;
}

//Bring motor to 0 and reset encoder counter
void resetMotor() {
  digitalWrite(motorDIR, DOWN);
  
  //Drive motors until limit switch enabled
  dutyCycle = 200;
  while(digitalRead(limitSwitch) == HIGH) {
    analogWrite(motorPWM, dutyCycle);
  }
  
  //Reset encoder counter, stop motor
  delay(50);
  myEnc.write(0);
  analogWrite(motorPWM, 0);
  dutyCycle = 0;
  startPosition = 0;
  destinationPosition = 0;
  destinationDegrees = 0;
  
  //Alert daemon
  Serial.write(destinationDegrees);
}

