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

Adafruit_NeoPixel bowtie = Adafruit_NeoPixel(28, 4, NEO_GRB + NEO_KHZ800);
int atFrame = 0;
const byte numOfAniProfiles = 4;
const byte numOfColourProfiles = 3;
const byte primes[] = {53,59,61,67,71,73,79,83,113};

//helper function prototypes
int red(uint32_t colour);
int green(uint32_t colour);
int blue(uint32_t colour);
uint32_t addAccentColour(uint32_t baseColour, uint32_t accentColour, float strength);
uint32_t brushColour(uint32_t darkColour, uint32_t lightColour, byte shift, byte divisions);

//Colour Profiles
typedef struct {
  uint32_t primary;
  uint32_t secondary;// = strip.Color(255, 0, 255);
  uint32_t dark;
  uint32_t light;
} colourProfile;

colourProfile colourProfiles[numOfColourProfiles] = {
  {0x0000FF,0xE4FFC0,0x00009E,0x00D0FF}, //Blues
  {0xFFFA07,0xFFAFC6,0xFFC700,0xFFF772}, //Yellows
  {0xFF007D,0xFF0000,0xA3101F,0xFF7287}  //Reds,pink
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
  static float divisions = 60.;
  static int direct = 1;
  
  uint32_t colourState = addAccentColour(colour.primary, colour.secondary, atFrame / divisions);
  for (int i = 0; i < 28; i++) bowtie.setPixelColor(i, colourState);
  
  if (atFrame <= 1) direct = 1;
  else if (atFrame >= divisions) direct = -1;
  atFrame += direct;
  delay(16);
}
//profile 3
byte daig0[] = {27,0}; byte daig1[][2] = {{19,26},{1,8}}; byte daig2[][3] = {{18,20,25},{2,7,9}}; byte daig3[][4] = {{14,17,21,24},{3,6,10,13}}; byte daig4[][8] = {{0,8,9,13,15,16,22,23},{4,5,11,12,14,18,19,27}}; 
byte daig5[][4] = {{1,7,10,12},{15,17,20,26}}; byte daig6[][3] = {{2,6,11},{16,21,25}}; byte daig7[][2] = {{3,5},{22,24}}; byte daig8[] = {4,23};
void cornerWash(colourProfile colour){
  static byte divisions = 70;
  static boolean up = false;
  static byte side = 1;
  uint32_t bcolour;
  
  bcolour = brushColour(colour.dark, colour.light, 15, divisions);
  bowtie.setPixelColor(daig0[side], bcolour);
  bcolour = brushColour(colour.dark, colour.light, 20, divisions);
  for (int i : daig1[side]) bowtie.setPixelColor(i,bcolour);
  bcolour = brushColour(colour.dark, colour.light, 25, divisions);
  for (int i : daig2[side]) bowtie.setPixelColor(i,bcolour);
  bcolour = brushColour(colour.dark, colour.light, 30, divisions);
  for (int i : daig3[side]) bowtie.setPixelColor(i,bcolour);
  bcolour = brushColour(colour.dark, colour.light, 35, divisions);
  for (int i : daig4[side]) bowtie.setPixelColor(i,bcolour);
  bcolour = brushColour(colour.dark, colour.light, 40, divisions);
  for (int i : daig5[side]) bowtie.setPixelColor(i,bcolour);
  bcolour = brushColour(colour.dark, colour.light, 45, divisions);
  for (int i : daig6[side]) bowtie.setPixelColor(i,bcolour);
  bcolour = brushColour(colour.dark, colour.light, 50, divisions);
  for (int i : daig7[side]) bowtie.setPixelColor(i,bcolour);
  bcolour = brushColour(colour.dark, colour.light, 55, divisions);
  bowtie.setPixelColor(daig8[side],bcolour);

  if(!up) atFrame += -1;
  else if(up) atFrame += 1;
  if (atFrame <= 0 || atFrame >= divisions){ //At the end of a cycle, determine where the next cycle starts
    byte num = random(4);
    up = (num % 2 == 0);
    side  = (num > 1 == 1? 1: 0);
    if (up) atFrame = 0;
    else atFrame = divisions;
  }
  delay(10);
}
//profile 4
byte led[][5] = {{12,13,14,15,30},{0,1,2,8,30},{3,4,5,6,30},{7,9,10,11,30},{27,26,25,21,30},{24,23,22,16,30},{18,17,19,20,30}};
void speckles(colourProfile colour){
  static byte frames[]     = {0, 50,20,60,10,30,40};
  static float divisions[] = {53,61,79,83,53,73,89};
  static byte direct[]     = { 1, 1, 1, 1, 1, 1, 1};
  static byte animating[]  = { 0, 0, 0, 0, 0, 0, 0};
  
  for (int i = 0; i < 28; i++) bowtie.setPixelColor(i, colour.primary);
  for (int i = 0; i < 7; i++)  bowtie.setPixelColor(led[i][animating[i]], addAccentColour(colour.primary, colour.secondary, frames[i] / divisions[i]));
  
  for (int i = 0; i < 7; i++){
    if (frames[i] >= divisions[i]) direct[i] = -1;
    if (frames[i] <= 0) { 
      direct[i] = 1; 
      animating[i] = random(5); 
      divisions[i] = primes[random(9)]; 
    }
    frames[i] += direct[i];
  }
  delay(8);
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
  }
}

//helper functions
int red  (uint32_t colour) { return colour >> 16 & 0xFF; }
int green(uint32_t colour) { return colour >> 8 & 0xFF; }
int blue (uint32_t colour) { return colour & 0xFF; }

uint32_t brushColour(uint32_t darkColour, uint32_t lightColour, byte shift, byte divisions){
  float strength = ((atFrame + shift) % divisions) / (float)divisions;
  strength = 4 * (strength - .5)*(strength - .5);
  return addAccentColour(darkColour, lightColour, strength);
}

//strength 0 return the base colour, strength 1 returns the accent colour,
uint32_t addAccentColour(uint32_t baseColour, uint32_t accentColour, float strength){
  uint32_t r = constrain(  red(baseColour) + ((  red(accentColour) -   red(baseColour)) * strength), 0, 255);
  uint32_t g = constrain(green(baseColour) + ((green(accentColour) - green(baseColour)) * strength), 0, 255);
  uint32_t b = constrain( blue(baseColour) + (( blue(accentColour) -  blue(baseColour)) * strength), 0, 255);
  
  return (r << 16) | (g << 8) | b;
}
