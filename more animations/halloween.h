/*
 * File name: Animations.h
 * Author: Tyler Makaro
 * This is a second implementation of the animations.h file with colours and animations specific for halloween.
 * License:
 *  May be modified, and reused freely. Attribution is requested. I'd also love to hear about your animations on twitter @tmakaro
 */
#include <arduino.h>
#include <Adafruit_NeoPixel.h>
#include "abstractionLayer.h"

Adafruit_NeoPixel bowtie = Adafruit_NeoPixel(28, 4, NEO_GRB + NEO_KHZ800);
int atFrame = 0;
byte onAniProfile = 0;
byte onDecayState = 1;
const byte numOfAniProfiles = 1;
const byte numOfDecayStates = 2;
const uint32_t decayStates[numOfDecayStates] = {
	0,0b10111100010011000000
};

colourProfile halloween = {
	0xFF6A00,0xD3D300,0x282828,0xFFD177,0x992100
};
//prototypes
bool flicker();
bool isDecayed(byte ledNum);

//animation profiles
//profile 0
void off(){
  for (int i = 0; i < 28; i++){
    bowtie.setPixelColor(i, 0);
  }
  delay(100);
}
//profile 1
void solid(){
  flicker();
  for (int i = 0; i < 28; i++){
	if(isDecayed(i)) bowtie.setPixelColor(i,addAccentColour(halloween.dark,halloween.primary, .2));
	else bowtie.setPixelColor(i, halloween.primary);
  }
  delay(100);
}

void callAnimation(){
  switch (onAniProfile){
    case 0:
      off();
      break;
    case 1:
      solid();
      break;
  }
}
//Here we will use colour profile to change decayed state instead
void nextColourProfile(){
	onDecayState = random(numOfDecayStates);
}
void nextAnimationProfile(){
  onAniProfile = (onAniProfile + 1) % (numOfAniProfiles + 1);
  nextColourProfile();
  atFrame = 0;
}

//add flicker to animations
bool flicker(){
	if (random(100)%20 == 0){
		for (int i = 0; i < 28; i++){
			bowtie.setPixelColor(i, halloween.dark);
		}
		bowtie.show();
		delay(3);
		return true;
	}
	return false;
}
//is 
bool isDecayed(byte ledNum){
	return (decayStates[onDecayState] >> ledNum & 1) == 1;
}
