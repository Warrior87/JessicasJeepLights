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

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define outerRingPin    2
#define innerRingPin    3

// How many NeoPixels are attached to the Arduino?
#define outerRingLEDCount 24
#define innerRingLEDCount 16

// Declare our NeoPixel strip object:
Adafruit_NeoPixel outerRing(outerRingLEDCount, outerRingPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel innerRing(innerRingLEDCount, innerRingPin, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// setup() function -- runs once at startup --------------------------------

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  outerRing.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  outerRing.show();            // Turn OFF all pixels ASAP
  outerRing.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)
  innerRing.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  innerRing.show();            // Turn OFF all pixels ASAP
  innerRing.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
//  outerTheaterChase(outerRing.Color(127, 127, 127), 50); // White, half brightness  
//  innerTheaterChase(innerRing.Color(127, 127, 127), 50); // White, half brightness

  rainbow(10);             // Flowing rainbow cycle along the whole strip
//  innerRainbow(10);             // Flowing rainbow cycle along the whole strip
  //theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.

void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<outerRing.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / outerRing.numPixels());
      outerRing.setPixelColor(i, outerRing.gamma32(outerRing.ColorHSV(pixelHue)));
    }
    for(int i=0; i<innerRing.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / innerRing.numPixels());
      innerRing.setPixelColor(i, innerRing.gamma32(innerRing.ColorHSV(pixelHue)));
    }
    outerRing.show(); // Update strip with new contents
    innerRing.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
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
//void innerTheaterChase(uint32_t color, int wait) {
//  for(int a=0; a<10; a++) {  // Repeat 10 times...
//    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
//      innerRing.clear();         //   Set all pixels in RAM to 0 (off)
//      // 'c' counts up from 'b' to end of strip in steps of 3...
//      for(int c=b; c<innerRing.numPixels(); c += 3) {
//        innerRing.setPixelColor(c, color); // Set pixel 'c' to value 'color'
//      }
//      innerRing.show(); // Update strip with new contents
//      delay(wait);  // Pause for a moment
//    }
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
