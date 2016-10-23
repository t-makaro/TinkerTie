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
const byte numOfAniProfiles = 1;
const byte numOfColourProfiles = 1;

colourProfile halloween ={
	{0x0},{0x0},{0x0},{0x0},{0x0}
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
void solid(colourProfile colour){
  for (int i = 0; i < 28; i++){
    bowtie.setPixelColor(i, colour.primary);
  }
  delay(100);
}

void callAnimation(byte aniProfile, byte colourProfile){
  switch (aniProfile){
    case 0:
      off();
      break;
    case 1:
      solid(halloween);
      break;
  }
}