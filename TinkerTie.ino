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

CapacitiveSensor capSensor = CapacitiveSensor(3,1);
byte onAniProfile = 0;
byte onColourProfile = 0;

void setup() {
  bowtie.begin();
  bowtie.setBrightness(26);
  bowtie.show(); // Initialize all pixels to 'off'
}

void loop() {
  capButton();
  callAnimation(onAniProfile, onColourProfile);
  bowtie.show();
}

void capButton(){
  static long state_prev = capSensor.capacitiveSensor(30);
  static unsigned long timer;
  long state_curr = capSensor.capacitiveSensor(30);
  
  if (state_curr == HIGH && state_prev == LOW){
    timer = millis();
  }
  else if (state_curr == LOW && state_prev == HIGH){ //on button release
    if (millis() - timer > 800) onColourProfile = (onColourProfile + 1) % numOfColourProfiles;
    else {
      onAniProfile = (onAniProfile + 1) % numOfAniProfiles;
      atFrame = 0;
    }
  }
  state_prev = state_curr;
}
