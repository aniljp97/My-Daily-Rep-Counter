#include <NewPing.h>
#include <Wire.h>
#include "RTClib.h"

// Pins for digits in number display
int digit4 = 6; //PWM Display most left display
int digit3 = 9; //PWM Display second left
int digit2 = 10; //PWM Display second right display
int digit1 = 11; //PWM Display most right display

// Pins for individual segments in number display
int segA = 2; 
int segB = 3; 
int segC = 4; 
int segD = 5; 
int segE = A0; 
int segF = 7; 
int segG = 8; 

// Pins for ultrasonic sensor
int trigPin = 12;    // Trigger
int echoPin = 13;    // Echo
int maxDistance = 60; // distance before ping gives up on return aka distance in cm that a rep will be read within (anything closer than 60 cm will be a rep)

// important changing numbers
int display_num = 0; // reps to be display
int distance; // distance from ultrasonic sensor in inches

// for keeping delay between for ability to increment between reps
int justAddedDelayCurr = 0;
int betweenRepDelayTime = 300;

// Class objects
RTC_DS1307 RTC; //define DS1307 object for keeping real time
NewPing sonar(trigPin, echoPin, maxDistance); // NewPing object for ultrasonic sensor. If maxDistance is reached, sonar returns a distance of 0

void setup() {
  // Serial for number checking
  Serial.begin(9600);
  
  // define display pins                
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);

  // define and set current time to RTC module
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

bool justAdded = false; // true if a rep has "just" (in the last .5 secs) been made

void loop() {
  // First checks if it is 4am. If so reset the display number to 0.
  DateTime now = RTC.now();
  //printTime(now); // should be commented in proper use (slows down loop time).
  if(now.hour() == 4) {
    display_num = 0;
  }

  // Get distance from the ultrasonic sensor is centimeters. If it is past the set max distance it will return 0.
  distance = sonar.ping_cm();

  // Applies a loop delay on being able to increment the total amount of reps based on global definitions
  // The ability to increment reps based on the bool variable justAdded
  if(justAddedDelayCurr > betweenRepDelayTime) {
    justAdded = false;
    justAddedDelayCurr = 0;
  }

  // If the distance is not 0, that means the distance is within the max distance
    // We can count this as a rep as long as a rep hasn't just been added defined by justAdded variable. justAdded is assigned as true
  // If a rep has been just added, we can increment the timer for the rep delay
  if(distance != 0) {
    if(!justAdded) {  
      display_num++;
      justAdded = true;
    }
  } else if(justAdded) {
    justAddedDelayCurr++;
  }

  // Through every loop display the current display number in the display
  lightNumber(display_num);
}


////////// Methods for DISPLAY. main:: lightNumber(int num) //////////

void lightNumber(int num) {
  String str_num = String(num);
  int digit_pos = 1;
  for(int i = str_num.length() - 1; i >= 0; i--) {
    lightDigit(str_num[i] - '0', digit_pos++);
  }
}

void lightDigit(int digitToDisplay, int digitPosition) {

  #define DIGIT_ON  LOW
  #define DIGIT_OFF  HIGH

  switch (digitPosition) {
  case 1:
    digitalWrite(digit1, DIGIT_ON);
    break;
  case 2:
    digitalWrite(digit2, DIGIT_ON);
    break;
  case 3:
    digitalWrite(digit3, DIGIT_ON);
    break;
  case 4:
    digitalWrite(digit4, DIGIT_ON);
    break;
  default:
    break;
  }

  #define SEGMENT_ON  HIGH
  #define SEGMENT_OFF LOW

  switch (digitToDisplay){

  case 0:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 1:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 2:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 3:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 4:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 5:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 6:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 7:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 8:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 9:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;
  }

  // delay to keep brightness of display up
  delayMicroseconds(1000);

  // turn full display off
  digitalWrite(segA, SEGMENT_OFF);
  digitalWrite(segB, SEGMENT_OFF);
  digitalWrite(segC, SEGMENT_OFF);
  digitalWrite(segD, SEGMENT_OFF);
  digitalWrite(segE, SEGMENT_OFF);
  digitalWrite(segF, SEGMENT_OFF);
  digitalWrite(segG, SEGMENT_OFF);
  digitalWrite(digit1, DIGIT_OFF);
  digitalWrite(digit2, DIGIT_OFF);
  digitalWrite(digit3, DIGIT_OFF);
  digitalWrite(digit4, DIGIT_OFF);
}


////////// Methods for RTC DS1307 module. main:: printTime() //////////
void printTime(DateTime now)
{
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
}
