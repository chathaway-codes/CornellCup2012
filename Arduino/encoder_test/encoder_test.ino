//Mike Collins
//Intel Cornell Cup
//PWM Test Sketch
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

//Encoder definition
Encoder myEnc(2, 3);

//Pin definitions
int limitSwitch = 7;
int motorDIR = 8;
int motorPWM = 9;

//PWM Suty Cycle
/*
  0.xx * 255 = dutyCycle;
*/
int dutyCycle = 128;

void setup() {
  pinMode(limitSwitch, INPUT);
  pinMode(motorDIR, OUTPUT);
  Serial.begin(115200);
  Serial.println("Basic Encoder Test - %50 Duty Cycle:");
  
  //Power the motor
  analogWrite(motorPWM, dutyCycle);
}

long oldPosition  = -999;

void loop() {
  //Read encoders
  long newPosition = myEnc.read();
  
  //Find limit
  if(digitalRead(limitSwitch) == HIGH) {
    Serial.println("FOUND LIMIT");
    Serial.println(newPosition);
    analogWrite(motorPWM, 0);
    
    //Change direction
    digitalWrite(motorDIR, HIGH);
    
    //Travel 500 ticks up and maintain position
    while(myEnc.read() < (newPosition + 10000)) { 
      analogWrite(motorPWM, dutyCycle);
    }
    
    //Stop
    analogWrite(motorPWM, 0);
    Serial.println("DONE");
  }
}
