/* 
  Auto chicken door opener
  author: Nathanial Myers
  email: nmyers322@gmail.com

  You will need to adjust the following values:

  - dayTime: The value at which the light sensor reading will trigger the door to open

  - nightTime: The value at which the light sensor reading will trigger the door to close

  - loopTime: The number of seconds between each light sensor reading

  - motorSpeed: How fast the motor will spin to open the door. Should be between 40 and 100

  - motorStrength: How much power the motor will spin with. If it sounds really loud, turn this down. Must be between 0 and 255

  - doorLength: The duration to spin the motor until the door completely opens or closes

  - doorDirection: if the door opens when it's supposed to close and vice versa, change the direction to 1
*/

// Include the Stepper library:
#include <Stepper.h>


// Adjust values here
const int dayTime = 28;
const int nightTime = 24;
const int loopTime = 5;
const int motorSpeed = 60;
const int motorStrength = 180;
const int doorLength = 1220;
const int doorDirection = -1;

// Define number of steps per revolution:
const int stepsPerRevolution = 200;

// Give the motor control pins names:
const int pwmA = 3;
const int pwmB = 11;
const int dirA = 12;
const int dirB = 13;

// Light sensor stuff
int lightLevel = 0;
const int photoResistorPin = A0;

// Door boolean
int doorOpened = 0;

// Initialize the stepper library on the motor shield:
Stepper stepper = Stepper(stepsPerRevolution, dirA, dirB);

void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing auto door sequence");

  // Set the PWM so that the direction pins can be used to control the motor:
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);

  analogWrite(pwmA, 0);
  analogWrite(pwmB, 0);

  // Set the motor speed (RPMs):
  stepper.setSpeed(motorSpeed);

  //Setup photo resistor
  pinMode(photoResistorPin, INPUT);
}

void openDoor() {
  Serial.println("Opening door");
  analogWrite(pwmA, motorStrength);
  analogWrite(pwmB, motorStrength);
  stepper.step(doorLength * doorDirection);
  analogWrite(pwmA, 0);
  analogWrite(pwmB, 0);
}

void closeDoor() {
  Serial.println("Closing door");
  analogWrite(pwmA, motorStrength);
  analogWrite(pwmB, motorStrength);
  stepper.step(doorLength * doorDirection * -1);
  analogWrite(pwmA, 0);
  analogWrite(pwmB, 0);
}

void loop()
{
  // Read light level
  lightLevel = analogRead(photoResistorPin);
  Serial.print("light level: ");
  Serial.println(lightLevel);

  if(lightLevel >= dayTime && doorOpened == 0) {
    openDoor();
  } else if(lightLevel <= nightTime && doorOpened == 1){
    closeDoor();
  }

  delay(loopTime * 1000);
}
