/*
 * File name: Animations.h
 * Author: Tyler Makaro
 * How to use: 
 *  The variables atFrame, numOfAniProfiles, and numOfColourProfiles MUST exist as they are used in the tinkertie.ino code.
 *  The NeoPixel bowtie object is used in the tinkertie.ino code and MUST remain unmodified to display the animations to the bowtie.
 *  The function callAnimation MUST exist with the number of the wanted animation and colour profile as parameters.
 *  The structure of the colour profiles and animation profiles may be modified for the desired effect.
 * Notes:
 *  The animation functions should act sort of like iterators. They should iterate the atFrame variable to prepare for the
 *  next time that they are called, and the delay function should be used to control framerate to the desired effect.
 * License:
 *  May be modified, and reused freely. Attribution is requested. I'd also love to hear about your animations on twitter @tmakaro
 */
#include <arduino.h>
#include <Adafruit_NeoPixel.h>
#include "abstractionLayer.h"

Adafruit_NeoPixel bowtie = Adafruit_NeoPixel(28, 4, NEO_GRB + NEO_KHZ800);
int atFrame = 0;
const byte numOfAniProfiles = 5;
const byte numOfColourProfiles = 4;
const byte primes[] = {53,59,61,67,71,73,79,83,113};

//structure function prototypes
void animateFunction( strengthFunction f, byte divisions, byte Delay, uint32_t colour1, uint32_t colour);
void animateFunction2( strengthFunction2 f, byte divisions, byte Delay, uint32_t colour1, uint32_t colour2, int param);

colourProfile colourProfiles[numOfColourProfiles] = {
  {0x0000EF,0xC9E0AA,0x00009E,0x00D0FF,0xFFC700}, //Blues
  {0xEAE307,0xFFAFC6,0xEEB700,0xFFF772,0xFFAAFF}, //Yellows
  {0x9F0000,0xFF46E4,0xA3101F,0xFF0FDF,0xFFC700}, //Reds,pink
  {0x008748,0xE5E500,0x007102,0x00FF77,0xE4ECC6} //Greens
}; //fill with hexidecimal RGB colour codes

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
//profile 2
void breathe(colourProfile colour){
  static float divisions = 120.;
  
  uint32_t colourState = addAccentColour(colour.primary, colour.secondary, -.5*cos(2*PI*atFrame/divisions)+.5);
  for (int i = 0; i < 28; i++) bowtie.setPixelColor(i, colourState);
  
  atFrame++;
  atFrame %= (int)divisions;
  delay(16);
}
//profile 3
float cornerWashFunction(point pt, float frame, float divisions, int sel){
  float x = (pt.x * .3926 * (sel>1?1:-1) + pt.y * .9197) * .75;
  float t = 8*frame/divisions - 4;
  float p = x + (sel%2==0?1:-1)*t;
  if(p>4) p-= 8;
  else if(p<-4) p += 8;
  return p*p/16;
}
void cornerWash(colourProfile colour){
  static int sel;
  if(atFrame == 30) sel = random(4);
  animateFunction2( cornerWashFunction, 60, 20, colour.dark, colour.light, sel);
}
//profile 4
byte led[][5] = {{12,13,14,15,30},{0,1,2,8,30},{3,4,5,6,30},{7,9,10,11,30},{27,26,25,21,30},{24,23,22,16,30},{18,17,19,20,30}};
void speckles(colourProfile colour){
  static byte frames[]     = {0, 50,20,60,10,30,40};
  static float divisions[] = {53,61,79,83,53,73,89};
  static byte animating[]  = { 0, 0, 0, 0, 0, 0, 0};
  
  for (int i = 0; i < 28; i++) bowtie.setPixelColor(i, colour.primary);
  for (int i = 0; i < 7; i++)  bowtie.setPixelColor(led[i][animating[i]], addAccentColour(colour.dark, colour.pop, -.5*cos(2*PI*frames[i] / divisions[i])+.5));
  
  for (int i = 0; i < 7; i++){
    frames[i] %= (int)divisions[i];
    if (frames[i] <= 0) { 
      animating[i] = random(5); 
      divisions[i] = primes[random(9)]; 
    }
    frames[i]++;
  }
  delay(16);
}
//profile 5
float rippleFunction(point pt, float frame, float divisions){
  return sin(-2*PI/divisions*frame+sqrt((pt.x*pt.x+5*pt.y*pt.y)))/2+.5;
}
void ripples(colourProfile colour){
  animateFunction(rippleFunction, 60, 10, colour.dark, colour.light);
}

void callAnimation(byte aniProfile, byte colourProfile){
  switch (aniProfile){
    case 0:
      off();
      break;
    case 1:
      solid(colourProfiles[colourProfile]);
      break;
    case 2:
      breathe(colourProfiles[colourProfile]);
      break;
    case 3:
      cornerWash(colourProfiles[colourProfile]);
      break;
    case 4:
      speckles(colourProfiles[colourProfile]);
      break;
    case 5:
      ripples(colourProfiles[colourProfile]);
      break;
  }
}

//structure functions
void animateFunction( strengthFunction f, byte divisions, byte Delay, uint32_t colour1, uint32_t colour2){
  point pt;
  uint32_t colourState;
  float strength;
  
  for (int i = 0; i < 28; i++){
    pt = ledNumToPoint(i);
    strength = f(pt, atFrame, divisions);
    colourState = addAccentColour(colour1, colour2, strength);
    bowtie.setPixelColor(i, colourState);
  }
  atFrame++;
  atFrame %= divisions;
  delay(Delay);
}
void animateFunction2( strengthFunction2 f, byte divisions, byte Delay, uint32_t colour1, uint32_t colour2, int param){
  point pt;
  uint32_t colourState;
  float strength;
  
  for (int i = 0; i < 28; i++){
    pt = ledNumToPoint(i);
    strength = f(pt, atFrame, divisions, param);
    colourState = addAccentColour(colour1, colour2, strength);
    bowtie.setPixelColor(i, colourState);
  }
  atFrame++;
  atFrame %= divisions;
  delay(Delay);
}
