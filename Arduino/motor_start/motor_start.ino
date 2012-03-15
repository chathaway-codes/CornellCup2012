//Mike Collins
//Intel Cornell Cup
//PWM Test Sketch
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

////////////////////////////////////////////////////////
//Encoder definition
Encoder myEnc(2, 3);

//Pin definitions
int limitSwitch = 7;
int motorDIR = 8;
int motorPWM = 9;

/*
  PWM Duty Cycle
  (0.xx * 255 = dutyCycle)
*/
int dutyCycle = 0;
int largeCycle = 128; //204
int smallCycle = 76; //76

//Motor specific qualities
int axisDegrees = 90;
int encoderCounts = 15000;
int acceptableDrift = 70;
////////////////////////////////////////////////////////

//Example IIC variable
int destinationDegrees = 50;
int destinationPosition = ((int) (encoderCounts / axisDegrees)) * destinationDegrees;

void setup() {
  pinMode(limitSwitch, INPUT);
  pinMode(motorDIR, OUTPUT);
  Serial.begin(115200);
  
  Serial.println(destinationPosition);
  
  //Reset routine
  reset();
}

void loop() {
  //Read encoders
  int newPosition = myEnc.read();
  Serial.println(newPosition);
  
  //Destination is down
  if(newPosition > destinationPosition) {
    digitalWrite(motorDIR, LOW);
    analogWrite(motorPWM, dutyCycle);
  }
  
  //Destination is up
  else if(newPosition < destinationPosition) {
    digitalWrite(motorDIR, HIGH);
    analogWrite(motorPWM, dutyCycle);
  }
  
  //Destination is here
  else {
    //Maintain position
    analogWrite(motorPWM, 0);
    //Serial.println("Destibation is here.");
    while(newPosition < (destinationPosition + acceptableDrift) && newPosition > (destinationPosition - acceptableDrift)) {

      //WE NEED IT FOR SOME REASON!!!
      //delayMicroseconds(1);
      //delay(200);
      Serial.println("Destibation is here.");
      
      newPosition = myEnc.read();
      dutyCycle = smallCycle;
    }
  }
}

//Bring motor to 0 and reset encoder counter
void reset() {
  dutyCycle = largeCycle;
  Serial.println("Running reset routine.");
  while(digitalRead(limitSwitch) == HIGH) {
    analogWrite(motorPWM, dutyCycle);
  }
  myEnc.write(0);
  Serial.println("Reset routine complete.");
}

