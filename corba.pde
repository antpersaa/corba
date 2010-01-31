#include <Servo.h>
#include "engine.h"
#include "navigation.h"

const int buttonPin = 2;
const int ledPin =  8;
const int servoPin = 9;

Servo servo;
engine engine(&servo);
navigation navigation(&engine);

void waitButton(){
  digitalWrite(ledPin, HIGH);
  while(true)
    if (digitalRead(buttonPin) == HIGH)
      break;
  digitalWrite(ledPin, LOW);
}

void setup(){
  Serial.begin(9600);

  pinMode(buttonPin, INPUT); 
  pinMode(ledPin, OUTPUT);
  servo.attach(servoPin);

  //waitButton();
  //navigation.calibrate();

  engine.straight();

  waitButton();
  delay(2000);

}

void loop(){
  navigation.steer();
  delay(70);
  //delay(1000);
}
