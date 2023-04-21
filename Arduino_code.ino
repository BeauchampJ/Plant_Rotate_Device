/*

Mech 307 Plant Rotating Device Code
Created by Jacob Beauchamp 
For more information, please check out this codes repository on github 
https://github.com/beauchampJ
*/


/*
This is the variable definition section of our code
*/
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_GC9A01A.h"
#include <Stepper.h> //Motor stuff

#define TFT_DC 7
#define TFT_CS 10
//Define joystick 
#define joyX A2 //CENTER VALUE IS ~500 
#define joyY A3 //CENTER VALUE IS ~500 
#define joyButton A4 //Joystick button 
int xValue;
int yValue;
int buttonValue;
int runTime;

//Defining motor revolution information 
const int stepsPerRev = 1024;
const int rolePerMinute =6;

//Setup Sensors
const int leftSensorPin = A0; // define pin for left sensor
const int rightSensorPin = A1; // define pin for right sensor

//confirmedStatus declaration. Used in the code to decide if a confirmation has been made
int confirmedStatus=0;
int sensorVoltage;

// Hardware SPI on Feather or other boards
Adafruit_GC9A01A tft(TFT_CS, TFT_DC);

//Setup Stepper motor
Stepper myStepper(stepsPerRev, 2, 3, 6, 5); 

/* 

This is the setup section where we display the startup screen and get everything booted up!

*/
void setup() {
  Serial.begin(9600);//Open up our serial port
  tft.begin(); //Begin screen printing
  tft.setRotation(2);//Rotates the stupid screen 270 degrees
  delay(200);//gives screen some time to do its thing
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
 unsigned long start = micros();
  tft.fillScreen(GC9A01A_BLACK); //Acts like an eraser, refreshes the screen!
  tft.setCursor(70, 85);
  tft.setTextColor(GC9A01A_WHITE);  tft.setTextSize(1);
  tft.setTextSize(2);
  tft.println("Automated");
  tft.setCursor(90, 105);
  tft.println("Plant");
  tft.setCursor(70, 125);
  tft.println("Turn Table");
  myStepper.setSpeed(rolePerMinute);//sets up the stepper motor speed
  delay(3000);
  menuScreen();

  

}
/* 

This is the looping portion of our code, which is where the user navigates their choices

*/
void loop() {
  //STILL NEED THE WHOLE STUFF WHERE IT DECIDES TO START ROTATING
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);
  buttonValue = analogRead(joyButton);
  Serial.println(buttonValue);
  delay(300);
  
  // Checking if the joystick is pointing towards the top
  if (yValue >= 980) {
    confirmationScreen();
    if (confirmedStatus==1){
      progressDisplay();

    } else {
      menuScreen();
    }//end if else
  
  } //end yValueCheck
 

} //end void loop

/* 

This is the callable function section of our code!

*/

void progressDisplay() { //Just used to physically setup the UI for the progress screen
  unsigned long startTime = millis(); // record the start time
  tft.fillScreen(GC9A01A_BLACK); //Acts like an eraser, refreshes the screen!
  
  //This adds the text above the circle
  tft.setCursor(85, 15);
  tft.setTextColor(0xFFE0);  tft.setTextSize(1);
  tft.println("Plant Process");

//Portion which draws the actual circle
int diameter = min(tft.width(), tft.height()) / 1.5;  // Calculate the circle diameter as half the screen size
  int radius = diameter / 4;  // Calculate the circle radius as one quarter of the screen size
  int x = tft.width() / 2;  // Calculate the x-coordinate of the circle center
  int y = tft.height() / 2;  // Calculate the y-coordinate of the circle center
  int numSegments = 12;  // Divide the circle into 12 segments
  int delayTime = 30;  // Delay time between segment additions

  for (int i = 0; i < numSegments; i++) {
    int startAngle = i * 360 / numSegments;  // Calculate the starting angle of the current segment
    int endAngle = (i + 1) * 360 / numSegments;  // Calculate the ending angle of the current segment

    for (int angle = startAngle; angle < endAngle; angle++) { //This is the portion that draws out the actual progress bar and tells it to adjust
      float radians = angle * 3.14159 / 180;  // Convert the angle to radians
      int x1 = x + radius * cos(radians);  // Calculate the x-coordinate of the start of the line
      int y1 = y + radius * sin(radians);  // Calculate the y-coordinate of the start of the line
      int x2 = x + radius * cos(radians + 0.1);  // Calculate the x-coordinate of the end of the line
      int y2 = y + radius * sin(radians + 0.1);  // Calculate the y-coordinate of the end of the line
      tft.drawLine(x1, y1, x2, y2, 0x07E0);  // Draw the line with a green color
      
      int leftSensorValue = analogRead(leftSensorPin); // read left sensor value
      int rightSensorValue = analogRead(rightSensorPin); // read right sensor value
  
      float leftSensorVoltage = leftSensorValue * (5.0 / 1023.0); // convert left sensor value to voltage
      float rightSensorVoltage = rightSensorValue * (5.0 / 1023.0); // convert right sensor value to voltage
      //myStepper.step(stepsPerRev);

      Serial.print("Left sensor voltage: ");
      Serial.print(leftSensorVoltage); // output left sensor voltage
      Serial.print("V, Right sensor voltage: ");
      Serial.print(rightSensorVoltage); // output right sensor voltage
      Serial.println("V");
      if (rightSensorVoltage>=leftSensorVoltage){
      sensorVoltage=rightSensorVoltage;
      } else {
      sensorVoltage=leftSensorVoltage;
      }

      //if (angle==(angle/2)) {
      //rotateMotor();
      //}
/* please uncomment me :P
  //Checking if motor needs to rotate
  if (rightSensorVoltage>475 && leftSensorVoltage<474) {
  myStepper.step(stepsPerRev);
  } else if (leftSensorVoltage>3 && rightSensorVoltage<1) {
  myStepper.step(-stepsPerRev);
  }
*/

  //WE NEED TO ADD SOMETHING HERE TO ADJUST THE MOTOR IF IT READS THAT NOT ENOUGH SUNLIGHT IS GETTING THROUGH. PROBABLY THROUGH AN IF STATEMENT
  

      delay(delayTime);  // Wait before drawing the next line

    }
  }

  unsigned long endTime = millis();//Record the end timing of the script
  float runTime = endTime - startTime;//Calculate our elapsed time in total. This accounts for the time to actually GET to the function :)
  runTime = (runTime/60000); //Converts from millisecond to minutes!

Serial.print(runTime);
  delay(1000); //Just a little pause. I think it makes it look nicer!

  
  //Displays done message
  tft.fillScreen(GC9A01A_BLACK);
  tft.setCursor(85, 85);
  tft.setTextColor(GC9A01A_WHITE);  tft.setTextSize(2);
  tft.println("DONE :)");
  tft.setCursor(50, 125);
  tft.println("Press button");
  tft.setCursor(85, 145);
  tft.println("to exit");
  tft.setTextColor(GC9A01A_WHITE);  tft.setTextSize(1);
  tft.setCursor(105, 210);
   tft.println("Recap");
  tft.setCursor(107, 220);
  tft.println("Mode");
  delay(500);
  buttonValue = analogRead(joyButton);

//Display done message until the user has seen the done and hit the button  
while (buttonValue != 0) {
  buttonValue = analogRead(joyButton);
  yValue = analogRead(joyY);

  if (yValue <= 100) {//Detects if the joystick is in the bottom y direction
    statScreen();//Enters statsScreen function. Which displays how long the run time was SEE BELOW
  }
  delay(100);

}

menuScreen();//Return the user to menu screen!

}//End progressDisplay()

void menuScreen() {
//Print center text
tft.fillScreen(GC9A01A_BLACK);
tft.setCursor(85, 85);
tft.setTextColor(GC9A01A_WHITE);  tft.setTextSize(2);
tft.println("Please");
tft.setCursor(65, 105);
tft.println("select an");
tft.setCursor(85, 125);
tft.println("option");


//Print top choice
tft.setTextColor(GC9A01A_WHITE);  tft.setTextSize(1);
tft.setCursor(90, 15);
tft.println("Activate!");

}//End menuScreen()


// Confirmation Screen code for anything
void confirmationScreen() {
  //Print confirmation message
  tft.fillScreen(GC9A01A_BLACK);
  tft.setTextSize(2);
  tft.setCursor(70, 85);
  tft.println("Press to");
  tft.setCursor(70, 105);
  tft.println("Confirm!");
  
  for (int time = 0; time < 300; time++) { //Waits ~5 seconds to see if button is pushed
  buttonValue = analogRead(joyButton);
  if (buttonValue==0) {
    confirmedStatus=1;
    time=300; //Kinda cheating our way out of this if statement
    delay(150); //Makes viewing things better
  } else {
    confirmedStatus=0;
  }
  delay(10);
  }
  
} // End confirmationScreen()

void statScreen() {
  tft.fillScreen(GC9A01A_BLACK);
  tft.setTextSize(2);
  tft.setCursor(45, 105); 
  tft.println("Device on for:");
  tft.setCursor(65, 125); 
  tft.println(String(runTime));
  tft.setCursor(95, 125);
  tft.println("minutes!");
  tft.setCursor(45, 155); 
  tft.println("Press to Esc:");
  delay(3000); //Gives the script time to breathe
  while (buttonValue != 0) {
    buttonValue = analogRead(joyButton);
    delay(50);
  }//End while loop
  
  menuScreen();//Returns Back to menu Screen

} //End statsScreen

//Code to rotate the motor that's triggered via interuppt
void rotateMotor() {
myStepper.step(stepsPerRev);
} //End rotateMotor



