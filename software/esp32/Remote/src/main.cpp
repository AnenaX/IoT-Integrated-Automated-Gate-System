#include <Arduino.h>

int buttonState = 0;
const int ledPin = 19; 
const int buttonPin = 18;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    // turn LED on:
    Serial.println("Pressé");
  } else {
    // turn LED off:
    Serial.println("Pas pressé");
  }
  delay(1000);
}
