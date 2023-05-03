/*
Code used to test out our motor. Incredibly bare bones.
*/
#include <Stepper.h>

const int stepsPerRev = 2048;
const int rolePerMinute =15;

Stepper myStepper(stepsPerRev, 4, 6, 5, 3); 
//Stepper myStepper(stepsPerRev, 8, 10, 9, 11); WORKING ONE


void setup() {
  myStepper.setSpeed(rolePerMinute);
char incomingChar = 1;
  Serial.begin(9600);
  

}

void loop() {
myStepper.step(stepsPerRev);


}

    



