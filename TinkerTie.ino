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
#include "Animations.h"

#define buttonPin 12
#define buttonPowerPin 10
#define seedPin A2

CapacitiveSensor capSensor = CapacitiveSensor(3, 1);
byte onAniProfile = 0;
byte onColourProfile = 0;

void setup() {
  bowtie.begin();
  bowtie.setBrightness(6);
  bowtie.show(); // Initialize all pixels to 'off'
  
  //pinMode(buttonPin, INPUT);
  //pinMode(buttonPowerPin, OUTPUT);
  //digitalWrite(buttonPowerPin, HIGH);
  
  randomSeed(analogRead(seedPin));
}

void loop() {
  aniProfileButton();
  //colourProfileButton();
  
  callAnimation(onAniProfile, onColourProfile);
  bowtie.show();
  
  //testColours();
}

void aniProfileButton() {
  static long state_prev = capSensor.capacitiveSensor(30);
  long state_curr = capSensor.capacitiveSensor(30);

  if ((state_curr <= 20) && (state_prev >= 50)) { //on short button release
    onAniProfile = (onAniProfile + 1) % (numOfAniProfiles + 1);
    atFrame = 0;
  }
  state_prev = state_curr;
}

//To be added later when a second button is installed onto the tinker tie.
/*void colourProfileButton(){
  static int state_prev = digitalRead(buttonPin);
  int state_curr = digitalRead(buttonPin);
  
  if (state_curr == LOW){
    onColourProfile = (onColourProfile + 1) % numOfColourProfiles;
  }
  state_prev = state_curr;
}*/

void testColours(){
  static long prevtime = millis();
  if(millis()-prevtime > 5000){
    prevtime = millis();
    onColourProfile = (onColourProfile + 1) % numOfColourProfiles;
  }
}
