/*
 * File name: TinkerTie.ino
 * Author: Tyler Makaro
 * How to use:
 *  modify the animations.h file to make the desired animations.
 * License:
 *  May be modified, and reused freely. Attribution is requested. I'd also love to hear about your animations on twitter @tmakaro
 */
#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>
#include "animations.h"

#define buttonPin 12
#define buttonPowerPin 13
#define groundPin 10
#define seedPin A2

CapacitiveSensor capSensor = CapacitiveSensor(3, 1);
bool emergency = false;

void setup() {
  bowtie.begin();
  bowtie.setBrightness(6);
  bowtie.show(); // Initialize all pixels to 'off'
  
  //For the extra button
  pinMode(buttonPin, INPUT);
  pinMode(buttonPowerPin, OUTPUT);
  pinMode(groundPin, OUTPUT);
  digitalWrite(buttonPowerPin, HIGH);
  digitalWrite(groundPin, LOW);
  
  emergency = (digitalRead(buttonPin) == HIGH);
  if (emergency){
    bowtie.setBrightness(255);
  }
  
  randomSeed(analogRead(seedPin));
  initializeAnimations();
}

void loop() {
  aniProfileButton();
  colourProfileButton();
  
  callAnimation();
  bowtie.show();
  
  //testColours();
}

void aniProfileButton() {
  static long state_prev = capSensor.capacitiveSensor(30);
  long state_curr = capSensor.capacitiveSensor(30);

  if ((state_curr <= 40) && (state_prev >= 50)) { //on short button release
    nextAnimationProfile();
  }
  state_prev = state_curr;
}

void colourProfileButton(){
  static int state_prev = digitalRead(buttonPin);
  int state_curr = digitalRead(buttonPin);
  
  if (state_curr == LOW and state_prev == HIGH){
    nextColourProfile();
  }
  state_prev = state_curr;
}

void testColours(){
  static long prevtime = millis();
  if(millis()-prevtime > 5000){
    prevtime = millis();
    nextColourProfile();
  }
}
