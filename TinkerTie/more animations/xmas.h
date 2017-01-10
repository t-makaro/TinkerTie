/*
 * File name: Animations.h
 * Author: Tyler Makaro
 * This is a third implementation of the animations.h file with colours and animations specific for xmas.
 * License:
 *  May be modified, and reused freely. Attribution is requested. I'd also love to hear about your animations on twitter @tmakaro
 */
#include <arduino.h>
#include <Adafruit_NeoPixel.h>
#include "abstractionLayer.h"
#include "gameOfLife.h"

Adafruit_NeoPixel bowtie = Adafruit_NeoPixel(28, 4, NEO_GRB + NEO_KHZ800);
int atFrame = 0;
byte onAniProfile = 0;
const byte numOfAniProfiles = 5;

colourProfile xmas = {
  0x058727/*green*/, 0xB20C00/*red*/, 0x00279E/*blue*/, 0xC4C4C4/*white*/, 0xE2C926/*yellow*/
};

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
  for (int i = 0; i < 28; i++){
    bowtie.setPixelColor(i, ledNumToPoint(i).y < 0 ? xmas.primary : xmas.light);
  }
  delay(100);
}
//profile 2
void triBreathe(){
  static float divisions = 120.;
  float strength;
  uint32_t colourState;
  strength = (atFrame % ((int) divisions / 3)) / (divisions / 3);
  
  if( atFrame < divisions/3)        colourState = addAccentColour(xmas.primary, xmas.secondary, sqrt(strength)*(-.5*cos(PI*strength)+.5));
  else if (atFrame < divisions*2/3) colourState = addAccentColour(xmas.secondary, xmas.pop, -.5*cos(PI*strength)+.5);
  else                              colourState = addAccentColour(xmas.pop, xmas.primary, -.5*cos(PI*strength)+.5);
  
  for (int i = 0; i < 28; i++) bowtie.setPixelColor(i, colourState);
  
  atFrame++;
  atFrame %= (int)divisions;
  delay(16);
}
//Profile 3
void snowflake(){
  for(byte i = 0; i < 28; i++){
    if(isAlive(i)) bowtie.setPixelColor(i, xmas.light);
    else bowtie.setPixelColor(i, xmas.dark);
  }
  iterateGame();
  delay(80);
}
//Profile 4
float candyState(point pt, float t){
  float y = pt.y/20.; float x = pt.x/20.;
  x = PI*floor((x * .3926 + y * .9197) / 3.219135 * 5);
  return -.5*cos(2*PI*t)*cos(x) + .5;
}
void candyCane(){
  static float divisions = 100;
  
  for(int i = 0; i < 28; i++){
    float strength = candyState(ledNumToPoint(i), atFrame/divisions);
    bowtie.setPixelColor(i, addAccentColour(xmas.secondary, xmas.light, strength));
  }
  
  atFrame = (atFrame + 1) % (int)divisions;
  delay(16);
}
//Profile 5
void rudolph(){
  static float divisions = 100;
  float strength;
  uint32_t colour;
  
  for(byte i = 0; i < 28; i++){
    if(i >= 12 && i <= 15){
      bowtie.setPixelColor(i, addAccentColour(0x3D1E00, xmas.secondary/*0xCC0000*/, -.5*cos(2*PI*atFrame/divisions)+.5));
    }
    else bowtie.setPixelColor(i, 0x56451E);
  }
  
  atFrame = (atFrame + 1) % (byte)divisions;
  delay(20);
}
//vital functions
void callAnimation(){
  switch (onAniProfile){
    case 0:
      off();
      break;
    case 1:
      solid();
      break;
    case 2:
      triBreathe();
      break;
    case 3:
      snowflake();
      break;
    case 4:
      candyCane();
      break;
    case 5:
      rudolph();
      break;
  }
}
void nextAnimationProfile(){
  onAniProfile = (onAniProfile + 1) % (numOfAniProfiles + 1);
  atFrame = 0;
}
void nextColourProfile(){
  //do nothing. We only have christmas colours here
}
void initializeAnimations(){
  byte flake[5] = {
    0b00000000,
    0b00000000,
    0b10100000,
    0b01100000,
    0b01000000
  };
  initializeBoard(flake, 0);
}
