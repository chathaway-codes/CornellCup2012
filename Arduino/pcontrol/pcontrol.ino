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
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//PWM boundries
int limitHigh = 204;
int limitLow = 77;
int dutyCycle = 0;

//Motor specific qualities
int axisDegrees = 90;
int encoderCounts = 15000;
int acceptableDrift = 70;
float rampFactor = 0.0;
boolean rampUp = true;
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//Example IIC variable
int startPosition = 0;
int destinationDegrees = 50;
int destinationPosition = ((int) (encoderCounts / axisDegrees)) * destinationDegrees;
int travel = destinationPosition - startPosition;
////////////////////////////////////////////////////////

boolean msg = true;

void setup() {
  //Calculate ramp factor for motor
  rampFactor = ((limitHigh - limitLow) / (encoderCounts / 4));
  
  //Initialize controller
  pinMode(limitSwitch, INPUT);
  pinMode(motorDIR, OUTPUT);
  Serial.begin(115200);
  reset();
}

void loop() {
  //Read encoders
  int newPosition = myEnc.read();
  
  //Get duty cycle
  dutyCycle = pcontrol(newPosition);
  
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
    Serial.println("Destination is here, maintaining position.");
    while(newPosition < (destinationPosition + acceptableDrift) && newPosition > (destinationPosition - acceptableDrift)) {

      //WE NEED IT FOR SOME REASON!!!
      delayMicroseconds(1);
      
      newPosition = myEnc.read();
    }
  }
}

//Proportional control
int pcontrol(int newPosition) {
  int pDuty;
  
  //Switch to ramp-down halfway through travel
  if((travel / 2) < (newPosition - startPosition)) {
    rampUp = false;
    
    if(msg) {
      Serial.println("Ramping down.");
      msg = false;
    }
  }
  else {
    rampUp = true;
  }
  
  //Calculate duty cycle
  if(rampUp) {
    pDuty = ((newPosition - startPosition) * rampFactor) + limitLow;
    if(pDuty >= limitHigh) {
      pDuty = limitHigh;
    }
  }
  else {
    pDuty = limitHigh - ((newPosition - startPosition) * rampFactor);
   if(pDuty <= limitLow) {
      pDuty = limitLow;
    }
  }
  return pDuty;
}

//Bring motor to 0 and reset encoder counter
void reset() {
  dutyCycle = 204;
  Serial.println("Running reset routine.");
  while(digitalRead(limitSwitch) == HIGH) {
    analogWrite(motorPWM, dutyCycle);
  }
  myEnc.write(0);
  Serial.println("Reset routine complete.");
  Serial.print("Traveling to destination: ");
  Serial.println(destinationDegrees);
  Serial.print("Distance to destination (ticks): ");
  Serial.println(travel);
  Serial.println("Ramping up.");
  dutyCycle = 0;
}
