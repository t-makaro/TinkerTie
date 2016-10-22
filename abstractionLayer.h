/*
 * File name: Animations.h
 * Author: Tyler Makaro
 * How to use: 
 *  Use the ledNumToPoint to use turn the LEDs into a cartesian plane, and thus also for 3D functions z=f(x,f) for animation. Use the strengthFunction
 *  typedefs to pass these functions into a common structure for display it.
 * Notes:
 * 
 * License:
 *  May be modified, and reused freely. Attribution is requested. I'd also love to hear about your animations on twitter @tmakaro
 */
typedef struct {
  float x; //in centimetres
  float y; //
} point;

typedef float (*strengthFunction)(point, float, float);
typedef float (*strengthFunction2)(point, float, float, int);

typedef struct {
  uint32_t primary;
  uint32_t secondary;
  uint32_t dark;
  uint32_t light;
  uint32_t pop;
} colourProfile;

point ledNumToPoint(byte ledNum){
  switch (ledNum){
    case 0:
      return {4.1,1.75};
    case 1:
      return {4.1,.9};
    case 2:
      return {4.1,0};
    case 3:
      return {4.1,-.9};
    case 4:
      return {4.1,-1.75};
    case 5:
      return {3.1,-1.35};
    case 6:
      return {3.1,-.5};
    case 7:
      return {3.1,.5};
    case 8:
      return {3.1,1.35};
    case 9:
      return {2.25,.9};
    case 10:
      return {2.25,0};
    case 11:
      return {2.25,-.9};
    case 12:
      return {1.35,-.5};
    case 13:
      return {1.35,.5};
    case 14:
      return {-1.35,.5};
    case 15:
      return {-1.35,-.5};
    case 16:
      return {-2.25,-.9};
    case 17:
      return {-2.25,0};
    case 18:
      return {-2.25,.9};
    case 19:
      return {-3.1,1.35};
    case 20:
      return {-3.1,.5};
    case 21:
      return {-3.1,-.5};
    case 22:
      return {-3.1,-1.35};
    case 23:
      return {-4.1,-1.75};
    case 24:
      return {-4.1,-.9};
    case 25:
      return {-4.1,0};
    case 26:
      return {-4.1,.9};
    case 27:
      return {-4.1,1.75};
  }
}

//helper functions
int red  (uint32_t colour) { return colour >> 16 & 0xFF; }
int green(uint32_t colour) { return colour >> 8 & 0xFF; }
int blue (uint32_t colour) { return colour & 0xFF; }

//strength 0 return the base colour, strength 1 returns the accent colour,
uint32_t addAccentColour(uint32_t baseColour, uint32_t accentColour, float strength){
  uint32_t r = constrain(  red(baseColour) + ((  red(accentColour) -   red(baseColour)) * strength), 0, 255);
  uint32_t g = constrain(green(baseColour) + ((green(accentColour) - green(baseColour)) * strength), 0, 255);
  uint32_t b = constrain( blue(baseColour) + (( blue(accentColour) -  blue(baseColour)) * strength), 0, 255);
  
  return (r << 16) | (g << 8) | b;
}
