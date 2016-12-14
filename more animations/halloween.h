/*
 * File name: Animations.h
 * Author: Tyler Makaro
 * This is a second implementation of the animations.h file with colours and animations specific for halloween.
 * License:
 *  May be modified, and reused freely. Attribution is requested. I'd also love to hear about your animations on twitter @tmakaro
 */
#include <arduino.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>
#include "abstractionLayer.h"

Adafruit_NeoPixel bowtie = Adafruit_NeoPixel(28, 4, NEO_GRB + NEO_KHZ800);
int atFrame = 0;
byte onAniProfile = 0;
byte onDecayState = 1;
const byte numOfAniProfiles = 4;
const byte numOfDecayStates = 4;
const uint32_t decayStates[numOfDecayStates] = {
	0,0b10111100010011001100,0b1100000010000000110011111101,0b1000011100010100000110010011
};

colourProfile halloween = {
	0xFF6A00,0xD3D300,0x282828,0xFFD177,0x072807
};
//prototypes
bool flicker(byte Delay, byte modify);
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
  flicker(3,20);
  for (int i = 0; i < 28; i++){
	if(isDecayed(i)) bowtie.setPixelColor(i,addAccentColour(halloween.dark,halloween.primary, .1));
	else bowtie.setPixelColor(i, halloween.primary);
  }
  delay(100);
}
//profile 2
float spookyBreatheFunctionP1(float t){
  return t + .15*sin(6*PI*t);
}
float spookyBreatheFunctionP2(float t){
  return -18.85*t*t+29.63*t-10.77;
}
void spookyBreathe(){
  flicker(90, 400);
  int16_t divisions = 300;
  float t = (float)atFrame / divisions;
  float strength;
  if (t < .83) { strength = spookyBreatheFunctionP1(t); }
  else {strength = spookyBreatheFunctionP2(t); }
  uint32_t colourState = addAccentColour(halloween.dark,halloween.primary,strength);
  uint32_t colourStateDecay = addAccentColour(0x202020,halloween.primary,strength/2);
  for (int i = 0; i < 28; i++){
    if(isDecayed(i)) bowtie.setPixelColor(i,colourStateDecay);
    else bowtie.setPixelColor(i, colourState);
  }
  atFrame++;
  atFrame %= divisions;
  delay(16);
}
//profile 3
float wingsFunction(point pt, float t){
  float phi = atan((float)pt.y/abs(pt.x))*12;
  t = 12*PI/8*sin(2*PI*t);
  return 1/((phi+t)*(phi+t)+1);
}
void wings(){
  flicker(100,65);
  byte divisions = 35;
  uint32_t colourState;
  float strength;
  point pt;
  for (int i = 0; i < 28; i++){
    pt = ledNumToPoint(i);
    strength = wingsFunction(pt, (float)atFrame/divisions);
    if(isDecayed(i)) colourState = addAccentColour(halloween.pop,halloween.primary, .5*strength);
    else colourState = addAccentColour(halloween.dark, halloween.primary, strength);
    bowtie.setPixelColor(i, colourState);
  }
  atFrame++;
  atFrame %= divisions;
  delay(14);
}
//profile 4
void cross(){
  flicker(100, 70);
  byte divisions = 40;
  uint32_t colourState;
  float strength;
  for (int i = 0; i < 28; i++){
    strength = .5*sin(2*PI*(float)atFrame/divisions + (i%2==0?0:PI))+.5;
    if(isDecayed(i)) strength /= 2;
    colourState = addAccentColour(2*halloween.dark, 2*halloween.pop, strength);
    bowtie.setPixelColor(i, colourState);
  }
  atFrame++;
  atFrame %= divisions;
  delay(25);
}

void callAnimation(){
  static long timer = millis();
  long time = millis();
  if (time - timer > 93206){
    timer = time;
    onDecayState = random(numOfDecayStates);
  }
  switch (onAniProfile){
    case 0:
      off();
      break;
    case 1:
      solid();
      break;
    case 2:
      spookyBreathe();
      break;
    case 3:
      wings();
      break;
    case 4:
      cross();
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
void initializeAnimations(){
}

//add flicker to animations
bool flicker(byte Delay, byte modify){
	if (random(1000)%modify == 0){
		for (int i = 0; i < 28; i++){
			bowtie.setPixelColor(i, halloween.dark);
		}
		bowtie.show();
		delay(Delay);
		return true;
	}
	return false;
}
//is 
bool isDecayed(byte ledNum){
	return (decayStates[onDecayState] >> ledNum & 1) == 1;
}
