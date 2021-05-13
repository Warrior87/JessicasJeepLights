// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define outerLeftRingPin    2
#define innerLeftRingPin    3
#define outerRightRingPin    5
#define innerRightRingPin    6
#define leftStripPin    10
#define rightStripPin    11

#define outerLeftRingLEDCount 24
#define innerLeftRingLEDCount 16
#define outerRightRingLEDCount 24
#define innerRightRingLEDCount 16
#define rightStripLEDCount 75
#define leftStripLEDCount 75

Adafruit_NeoPixel outerLeftRing(outerLeftRingLEDCount, outerLeftRingPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel innerLeftRing(innerLeftRingLEDCount, innerLeftRingPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel outerRightRing(outerRightRingLEDCount, outerRightRingPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel innerRightRing(innerRightRingLEDCount, innerRightRingPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rightStrip(rightStripLEDCount, rightStripPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leftStrip(leftStripLEDCount, leftStripPin, NEO_GRB + NEO_KHZ800);

unsigned long currentTime;
unsigned long rainbow_lastServiceTime;
unsigned long rainbow_serviceInterval = 1;
long firstPixelHue;
unsigned long theaterChase_lastServiceTime;
unsigned long theaterChase_serviceInterval = 50;
int innerTheaterChase_a;
int innerTheaterChase_b;
int outerTheaterChase_a;
int outerTheaterChase_b;
byte innerLeftRingState = 1;
byte outerLeftRingState = 1;
byte innerRightRingState = 2;
byte outerRightRingState = 2;
unsigned long cycleFeature_lastServiceTime;
unsigned long cycleFeature_serviceInterval = 2000;
byte stripBrightness = 200;
byte ringBrightness = 20;

uint32_t blue = outerLeftRing.gamma32(outerLeftRing.ColorHSV(43690));
uint32_t red = outerLeftRing.gamma32(outerLeftRing.ColorHSV(0));

/*
 * state table:
 * 0 - off
 * 1 - rainbow CCW
 * 2 - rainbow CW
 * 3 - theater chase
 */

void setup() {
  outerLeftRing.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  outerLeftRing.show();            // Turn OFF all pixels ASAP
  outerLeftRing.setBrightness(ringBrightness); // Set BRIGHTNESS (max = 255)
  innerLeftRing.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  innerLeftRing.show();            // Turn OFF all pixels ASAP
  innerLeftRing.setBrightness(ringBrightness); // Set BRIGHTNESS (max = 255)
  outerRightRing.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  outerRightRing.show();            // Turn OFF all pixels ASAP
  outerRightRing.setBrightness(ringBrightness); // Set BRIGHTNESS (max = 255)
  innerRightRing.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  innerRightRing.show();            // Turn OFF all pixels ASAP
  innerRightRing.setBrightness(ringBrightness); // Set BRIGHTNESS (max = 255)
  leftStrip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  leftStrip.show();            // Turn OFF all pixels ASAP
  leftStrip.setBrightness(stripBrightness); // Set BRIGHTNESS (max = 255)
  rightStrip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  rightStrip.show();            // Turn OFF all pixels ASAP
  rightStrip.setBrightness(stripBrightness); // Set BRIGHTNESS (max = 255)
}


void loop() {
  currentTime = millis();
//  outerTheaterChase(outerRing.Color(127, 127, 127), 50); // White, half brightness  
//  innerTheaterChase(innerRing.Color(127, 127, 127), 50); // White, half brightness

  //rainbow(10);             // Flowing rainbow cycle along the whole strip
  //outerRainbow(10);             // Flowing rainbow cycle along the whole strip
  //theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
  if(outerLeftRingState == 0){
    outerLeftRing.clear();
    outerLeftRing.show();
  }
  if(innerLeftRingState == 0){
    innerLeftRing.clear();
    innerLeftRing.show();
  }

  //rainbow
  if((currentTime - rainbow_lastServiceTime) >= rainbow_serviceInterval){
    rainbow_lastServiceTime = currentTime;
    rainbow();
  }

//  cycle (demo mode 1)
//  if((currentTime - cycleFeature_lastServiceTime) >= cycleFeature_serviceInterval){
//    cycleFeature_lastServiceTime = currentTime;
//    innerRingState++;
//    outerRingState++;
//    if(innerRingState >= 4){
//      innerRingState = 0;
//    }
//    if(outerRingState >= 4){
//      outerRingState = 0;
//    }
//  }

//  //theater chase
//  if((currentTime - theaterChase_lastServiceTime) >= theaterChase_serviceInterval){
//    theaterChase_lastServiceTime = currentTime;
//    if(innerRingState == 3){
//      innerTheaterChase(innerRing.Color(127, 127, 127)); // White, half brightness
//    }
//    if(outerRingState == 3){
//      outerTheaterChase(innerRing.Color(127, 127, 127)); // White, half brightness
//    }
//  }
}

void rainbow() {
  if(firstPixelHue < 5*65536) {    
    //for outer ring rainbow CCW
    if(outerLeftRingState == 1){
      for(int i=0; i<outerLeftRing.numPixels(); i++) {
        int pixelHue = firstPixelHue + (i * 65536L / outerLeftRing.numPixels());
        outerLeftRing.setPixelColor(i, outerLeftRing.gamma32(outerLeftRing.ColorHSV(pixelHue)));
      }
    outerLeftRing.show(); // Update strip with new contents
    }
    
    //for outer ring rainbow CW
    if(outerLeftRingState == 2){
      for(int i=0; i<outerLeftRing.numPixels(); i++) {
        int pixelHue = firstPixelHue + (i * 65536L / outerLeftRing.numPixels());
        outerLeftRing.setPixelColor((outerLeftRing.numPixels() - i - 1), outerLeftRing.gamma32(outerLeftRing.ColorHSV(pixelHue)));
      }
    outerLeftRing.show(); // Update strip with new contents
    }
    
    //for inner ring rainbow CCW
    if(innerLeftRingState == 1){
      for(int i=0; i<innerLeftRing.numPixels(); i++) {
        int pixelHue = firstPixelHue + (i * 65536L / innerLeftRing.numPixels());
        innerLeftRing.setPixelColor((innerLeftRing.numPixels() - i - 1), innerLeftRing.gamma32(innerLeftRing.ColorHSV(pixelHue)));    //reverses the inner ring direction
      }
    innerLeftRing.show(); // Update strip with new contents
    }
    
    //for inner ring rainbow CW
    if(innerLeftRingState == 2){
      for(int i=0; i<innerLeftRing.numPixels(); i++) {
        int pixelHue = firstPixelHue + (i * 65536L / innerLeftRing.numPixels());
        innerLeftRing.setPixelColor(i, innerLeftRing.gamma32(innerLeftRing.ColorHSV(pixelHue)));
      }
    innerLeftRing.show(); // Update strip with new contents
    }    
    //for outer ring rainbow CCW
    if(outerRightRingState == 1){
      for(int i=0; i<outerRightRing.numPixels(); i++) {
        int pixelHue = firstPixelHue + (i * 65536L / outerRightRing.numPixels());
        outerRightRing.setPixelColor(i, outerRightRing.gamma32(outerRightRing.ColorHSV(pixelHue)));
      }
    outerRightRing.show(); // Update strip with new contents
    }
    
    //for outer ring rainbow CW
    if(outerRightRingState == 2){
      for(int i=0; i<outerRightRing.numPixels(); i++) {
        int pixelHue = firstPixelHue + (i * 65536L / outerRightRing.numPixels());
        outerRightRing.setPixelColor((outerRightRing.numPixels() - i - 1), outerRightRing.gamma32(outerRightRing.ColorHSV(pixelHue)));
      }
    outerRightRing.show(); // Update strip with new contents
    }
    
    //for inner ring rainbow CCW
    if(innerRightRingState == 1){
      for(int i=0; i<innerRightRing.numPixels(); i++) {
        int pixelHue = firstPixelHue + (i * 65536L / innerRightRing.numPixels());
        innerRightRing.setPixelColor((innerRightRing.numPixels() - i - 1), innerRightRing.gamma32(innerRightRing.ColorHSV(pixelHue)));    //reverses the inner ring direction
      }
    innerRightRing.show(); // Update strip with new contents
    }
    
    //for inner ring rainbow CW
    if(innerRightRingState == 2){
      for(int i=0; i<innerRightRing.numPixels(); i++) {
        int pixelHue = firstPixelHue + (i * 65536L / innerRightRing.numPixels());
        innerRightRing.setPixelColor(i, innerRightRing.gamma32(innerRightRing.ColorHSV(pixelHue)));
      }
    innerRightRing.show(); // Update strip with new contents
    }
    for(int i=0; i<leftStrip.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / leftStrip.numPixels());
      leftStrip.setPixelColor(i, leftStrip.gamma32(leftStrip.ColorHSV(pixelHue)));
    }
    leftStrip.show(); // Update leftStrip with new contents
    for(int i=0; i<rightStrip.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / rightStrip.numPixels());
      rightStrip.setPixelColor(i, rightStrip.gamma32(rightStrip.ColorHSV(pixelHue)));
    }
    rightStrip.show(); // Update rightStrip with new contents
    firstPixelHue += 256;
  }
  else{
    firstPixelHue = 0;
  }
}


// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
//void colorWipe(uint32_t color, int wait) {
//  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
//    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
//    strip.show();                          //  Update strip to match
//    delay(wait);                           //  Pause for a moment
//  }
//}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
//void outerTheaterChase(uint32_t color, int wait) {
//  for(int a=0; a<10; a++) {  // Repeat 10 times...
//    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
//      outerRing.clear();         //   Set all pixels in RAM to 0 (off)
//      // 'c' counts up from 'b' to end of strip in steps of 3...
//      for(int c=b; c<outerRing.numPixels(); c += 3) {
//        outerRing.setPixelColor(c, color); // Set pixel 'c' to value 'color'
//      }
//      outerRing.show(); // Update strip with new contents
//      delay(wait);  // Pause for a moment
//    }
//  }
//}
//
//void innerTheaterChase(uint32_t color) {
//  if(innerTheaterChase_a<10) {  // Repeat 10 times...
//    if(innerTheaterChase_b<3) { //  'b' counts from 0 to 2...
//      innerRing.clear();         //   Set all pixels in RAM to 0 (off)
//      // 'c' counts up from 'b' to end of strip in steps of 3...
//      for(int c=innerTheaterChase_b; c<innerRing.numPixels(); c += 3) {
//        innerRing.setPixelColor(c, color); // Set pixel 'c' to value 'color'
//      }
//      innerRing.show(); // Update strip with new contents
//      //delay(wait);  // Pause for a moment
//      innerTheaterChase_b++;
//    }
//    else{
//      innerTheaterChase_b=0;
//    }
//    innerTheaterChase_a++;
//  }
//  else{
//    innerTheaterChase_a=0;
//  }
//}
//
//void outerTheaterChase(uint32_t color) {
//  if(outerTheaterChase_a<10) {  // Repeat 10 times...
//    if(outerTheaterChase_b<3) { //  'b' counts from 0 to 2...
//      outerRing.clear();         //   Set all pixels in RAM to 0 (off)
//      // 'c' counts up from 'b' to end of strip in steps of 3...
//      for(int c=outerTheaterChase_b; c<outerRing.numPixels(); c += 3) {
//        outerRing.setPixelColor(c, color); // Set pixel 'c' to value 'color'
//      }
//      outerRing.show(); // Update strip with new contents
//      //delay(wait);  // Pause for a moment
//      outerTheaterChase_b++;
//    }
//    else{
//      outerTheaterChase_b=0;
//    }
//    outerTheaterChase_a++;
//  }
//  else{
//    outerTheaterChase_a=0;
//  }
//}

//// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
//void outerRainbow(int wait) {
//  // Hue of first pixel runs 5 complete loops through the color wheel.
//  // Color wheel has a range of 65536 but it's OK if we roll over, so
//  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
//  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
//  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
//    for(int i=0; i<outerRing.numPixels(); i++) { // For each pixel in strip...
//      // Offset pixel hue by an amount to make one full revolution of the
//      // color wheel (range of 65536) along the length of the strip
//      // (strip.numPixels() steps):
//      int pixelHue = firstPixelHue + (i * 65536L / outerRing.numPixels());
//      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
//      // optionally add saturation and value (brightness) (each 0 to 255).
//      // Here we're using just the single-argument hue variant. The result
//      // is passed through strip.gamma32() to provide 'truer' colors
//      // before assigning to each pixel:
//      outerRing.setPixelColor(i, outerRing.gamma32(outerRing.ColorHSV(pixelHue)));
//    }
//    outerRing.show(); // Update strip with new contents
//    delay(wait);  // Pause for a moment
//  }
//}
//
//void innerRainbow(int wait) {
//  // Hue of first pixel runs 5 complete loops through the color wheel.
//  // Color wheel has a range of 65536 but it's OK if we roll over, so
//  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
//  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
//  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
//    for(int i=0; i<innerRing.numPixels(); i++) { // For each pixel in strip...
//      // Offset pixel hue by an amount to make one full revolution of the
//      // color wheel (range of 65536) along the length of the strip
//      // (strip.numPixels() steps):
//      int pixelHue = firstPixelHue + (i * 65536L / innerRing.numPixels());
//      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
//      // optionally add saturation and value (brightness) (each 0 to 255).
//      // Here we're using just the single-argument hue variant. The result
//      // is passed through strip.gamma32() to provide 'truer' colors
//      // before assigning to each pixel:
//      innerRing.setPixelColor(i, innerRing.gamma32(innerRing.ColorHSV(pixelHue)));
//    }
//    innerRing.show(); // Update strip with new contents
//    delay(wait);  // Pause for a moment
//  }
//}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
//void theaterChaseRainbow(int wait) {
//  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
//  for(int a=0; a<30; a++) {  // Repeat 30 times...
//    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
//      strip.clear();         //   Set all pixels in RAM to 0 (off)
//      // 'c' counts up from 'b' to end of strip in increments of 3...
//      for(int c=b; c<strip.numPixels(); c += 3) {
//        // hue of pixel 'c' is offset by an amount to make one full
//        // revolution of the color wheel (range 65536) along the length
//        // of the strip (strip.numPixels() steps):
//        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
//        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
//        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
//      }
//      strip.show();                // Update strip with new contents
//      delay(wait);                 // Pause for a moment
//      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
//    }
//  }
//}
