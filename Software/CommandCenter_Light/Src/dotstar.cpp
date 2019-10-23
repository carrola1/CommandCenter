/*------------------------------------------------------------------------
  Arduino library to control Adafruit Dot Star addressable RGB LEDs.

  Written by Limor Fried and Phil Burgess for Adafruit Industries.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  ------------------------------------------------------------------------
  This file is part of the Adafruit Dot Star library.

  Adafruit Dot Star is free software: you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License
  as published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  Adafruit Dot Star is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with DotStar.  If not, see <http://www.gnu.org/licenses/>.
  ------------------------------------------------------------------------*/

#include "dotstar.hpp"
#include "gpio.h"
#include <stdlib.h>

// Constructor for 'soft' (bitbang) SPI -- any two pins can be used
DotStar::DotStar(uint8_t n, uint8_t o) :
 brightness(0), pixels(NULL), rOffset(o & 3), gOffset((o >> 2) & 3),
 bOffset((o >> 4) & 3)
{
  updateLength(n);
  ringIndex = 0;
}

DotStar::~DotStar(void) { // Destructor
  sw_spi_end();
}

void DotStar::begin(void) { // Initialize SPI
  sw_spi_init();
}

// Length can be changed post-constructor for similar reasons (sketch
// config not hardcoded).  But DON'T use this for "recycling" strip RAM...
// all that reallocation is likely to fragment and eventually fail.
// Instead, set length once to longest strip.
void DotStar::updateLength(uint8_t n) {
	uint16_t bytes = (rOffset == gOffset) ?
	    n + ((n + 3) / 4) : // MONO: 10 bits/pixel, round up to next byte
	    n * 3;              // COLOR: 3 bytes/pixel
	pixels = (uint8_t *)malloc(bytes);
	numLEDs = n;
	clear();
}

// SPI STUFF ---------------------------------------------------------------

void DotStar::sw_spi_init(void) { // Init 'soft' (bitbang) SPI
  HAL_GPIO_WritePin(Data_Port, Data_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Data_Port, Clock_Pin, GPIO_PIN_RESET);
}

void DotStar::sw_spi_end() { // Stop 'soft' SPI
  HAL_GPIO_WritePin(Data_Port, Data_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Clock_Port, Clock_Pin, GPIO_PIN_RESET);
}

void DotStar::sw_spi_out(uint8_t n) { // Bitbang SPI write
  for(uint8_t i=8; i--; n <<= 1) {
    if(n & 0x80) HAL_GPIO_WritePin(Data_Port, Data_Pin, GPIO_PIN_SET);
    else         HAL_GPIO_WritePin(Data_Port, Data_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Clock_Port, Clock_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Clock_Port, Clock_Pin, GPIO_PIN_RESET);
  }
}

/* ISSUE DATA TO LED STRIP -------------------------------------------------

  Although the LED driver has an additional per-pixel 5-bit brightness
  setting, it is NOT used or supported here because it's a brain-dead
  misfeature that's counter to the whole point of Dot Stars, which is to
  have a much faster PWM rate than NeoPixels.  It gates the high-speed
  PWM output through a second, much slower PWM (about 400 Hz), rendering
  it useless for POV.  This brings NOTHING to the table that can't be
  already handled better in one's sketch code.  If you really can't live
  without this abomination, you can fork the library and add it for your
  own use, but any pull requests for this will NOT be merged, nuh uh!
*/

void DotStar::show(void) {

  if(!pixels) return;

  uint8_t *ptr = pixels, i;            // -> LED data
  uint8_t n   = numLEDs;              // Counter
  uint16_t b16 = (uint16_t)brightness; // Type-convert for fixed-point math

  for(i=0; i<4; i++) sw_spi_out(0);    // Start-frame marker
  if(brightness) {                     // Scale pixel brightness on output
    do {                               // For each pixel...
      sw_spi_out(0xFF);                //  Pixel start
      for(i=0; i<3; i++) sw_spi_out((*ptr++ * b16) >> 8); // Scale, write
    } while(--n);
  } else {                             // Full brightness (no scaling)
    do {                               // For each pixel...
      sw_spi_out(0xFF);                //  Pixel start
      for(i=0; i<3; i++) sw_spi_out(*ptr++); // R,G,B
    } while(--n);
  }
  for(i=0; i<((numLEDs + 15) / 16); i++) sw_spi_out(0xFF); // End-frame marker (see note above)
}

void DotStar::incrRing(RGB_VALS rgb) {
  if (ringIndex == numLEDs-1) {
    ringIndex = 0;
  } else {
    ringIndex++;
  }
  setPixelColor(ringIndex, rgb.r, rgb.g, rgb.b);
  show();
}

void DotStar::decrRing(RGB_VALS rgb) {
  if (ringIndex == 0) {
    ringIndex = numLEDs-1;
  } else {
    ringIndex--;
  }
  setPixelColor(ringIndex, rgb.r, rgb.g, rgb.b);
  show();
}

void DotStar::clear() { // Write 0s (off) to full pixel buffer
  //memset(pixels, 0, (rOffset == gOffset) ?
    //numLEDs + ((numLEDs + 3) / 4) : // MONO: 10 bits/pixel
    //numLEDs * 3);                   // COLOR: 3 bytes/pixel
}

// Set pixel color, separate R,G,B values (0-255 ea.)
void DotStar::setPixelColor(
 uint8_t n, uint8_t r, uint8_t g, uint8_t b) {
  if(n < numLEDs) {
    uint8_t *p = &pixels[n * 3];
    p[rOffset] = r;
    p[gOffset] = g;
    p[bOffset] = b;
  }
}

// Set pixel color, 'packed' RGB value (0x000000 - 0xFFFFFF)
void DotStar::setPixelColor(uint8_t n, uint32_t c) {
  if(n < numLEDs) {
    uint8_t *p = &pixels[n * 3];
    p[rOffset] = (uint8_t)(c >> 16);
    p[gOffset] = (uint8_t)(c >>  8);
    p[bOffset] = (uint8_t)c;
  }
}

// Convert separate R,G,B to packed value
uint32_t DotStar::Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

// Read color from previously-set pixel, returns packed RGB value.
uint32_t DotStar::getPixelColor(uint8_t n) const {
  if(n >= numLEDs) return 0;
  uint8_t *p = &pixels[n * 3];
  return ((uint32_t)p[rOffset] << 16) |
         ((uint32_t)p[gOffset] <<  8) |
          (uint32_t)p[bOffset];
}

uint8_t DotStar::numPixels(void) { // Ret. strip length
  return numLEDs;
}

// Set global strip brightness.  This does not have an immediate effect;
// must be followed by a call to show().  Not a fan of this...for various
// reasons I think it's better handled in one's sketch, but it's here for
// parity with the NeoPixel library.  Good news is that brightness setting
// in this library is 'non destructive' -- it's applied as color data is
// being issued to the strip, not during setPixel(), and also means that
// getPixelColor() returns the exact value originally stored.
void DotStar::setBrightness(uint8_t b) {
  // Stored brightness value is different than what's passed.  This
  // optimizes the actual scaling math later, allowing a fast 8x8-bit
  // multiply and taking the MSB.  'brightness' is a uint8_t, adding 1
  // here may (intentionally) roll over...so 0 = max brightness (color
  // values are interpreted literally; no scaling), 1 = min brightness
  // (off), 255 = just below max brightness.
  brightness = b + 1;
}

uint8_t DotStar::getBrightness(void) const {
  return brightness - 1; // Reverse above operation
}

// Return pointer to the library's pixel data buffer.  Use carefully,
// much opportunity for mayhem.  It's mostly for code that needs fast
// transfers, e.g. SD card to LEDs.  Color data is in BGR order.
uint8_t *DotStar::getPixels(void) const {
  return pixels;
}
