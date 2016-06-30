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
const byte numOfAniProfiles = 3;
const byte numOfColourProfiles = 3;

//helper function prototypes
int red(uint32_t colour);
int green(uint32_t colour);
int blue(uint32_t colour);

//Colour Profiles
typedef struct {
  uint32_t primary;
  uint32_t secondary;// = strip.Color(255, 0, 255);
  uint32_t dark;
  uint32_t light;
} colourProfile;

colourProfile colourProfiles[numOfColourProfiles] = {
  {0x0000FF,0x8300FF,0x00009E,0x00D0FF}, //Blues
  {0xFFFA07,0xFFAFC6,0xFFC700,0xFFF772}, //Yellows
  {0xFF0000,0xFF007D,0xA3101F,0xFF7287}  //Reds,pink
}; //fill with hexidecimal RGB colour codes

//animation profiles
//profile 0
void off(){
  for (int i = 0; i < 28; i++){
    bowtie.setPixelColor(i, 0);
  }
  delay(500);
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
  static float divisions = 40.;
  static int direct = 1;
  int r = red(colour.primary) + (red(colour.primary) - red(colour.secondary)) / divisions * atFrame;
  int g = green(colour.primary) + (green(colour.primary) - green(colour.secondary)) / divisions * atFrame;
  int b = blue(colour.primary) + (blue(colour.primary) - blue(colour.secondary)) / divisions * atFrame;
  
  for (int i = 0; i < 28; i++){
    bowtie.setPixelColor(i, r, g, b);
  }
  if (atFrame <= 0) direct = 1;
  else if (atFrame >= divisions) direct = -1;
  atFrame += direct;
  delay(40);
}
//profile 3
void wash(colourProfile colour){
  //animation code
  atFrame = (atFrame + 1) % 20;
  delay(15);
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
      wash(colourProfiles[colourProfile]);
      break;
  }
}

//helper functions
int red(uint32_t colour){
  return colour >> 16 & 0xFF;
}
int green(uint32_t colour){
  return colour >> 8 & 0xFF;
}
int blue(uint32_t colour){
  return colour & 0xFF;
}
