/*------------------------------------------------------------------------
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

#ifndef _DOTSTAR_HPP_
#define _DOTSTAR_HPP_

#include <stdint.h>

#define Data_Port DOTSTAR_DATA_GPIO_Port
#define Data_Pin DOTSTAR_DATA_Pin
#define Clock_Port DOTSTAR_CLK_GPIO_Port
#define Clock_Pin DOTSTAR_CLK_Pin

// Color-order flag for LED pixels (optional extra parameter to constructor):
// Bits 0,1 = R index (0-2), bits 2,3 = G index, bits 4,5 = B index
#define DOTSTAR_RGB (0 | (1 << 2) | (2 << 4))
#define DOTSTAR_RBG (0 | (2 << 2) | (1 << 4))
#define DOTSTAR_GRB (1 | (0 << 2) | (2 << 4))
#define DOTSTAR_GBR (2 | (0 << 2) | (1 << 4))
#define DOTSTAR_BRG (1 | (2 << 2) | (0 << 4))
#define DOTSTAR_BGR (2 | (1 << 2) | (0 << 4))
#define DOTSTAR_MONO 0 // Single-color strip WIP DO NOT USE YET

class DotStar {

 public:

    DotStar(uint8_t n, uint8_t o=DOTSTAR_BRG);
   ~DotStar(void);
  void
    begin(void),                            // Prime pins/SPI for output
    clear(),                                // Set all pixel data to zero
    setBrightness(uint8_t),                 // Set global brightness 0-255
    setPixelColor(uint8_t n, uint32_t c),
    setPixelColor(uint8_t n, uint8_t r, uint8_t g, uint8_t b),
    show(void),                             // Issue color data to strip
    updateLength(uint8_t n);               // Change length
  uint32_t
    Color(uint8_t r, uint8_t g, uint8_t b), // R,G,B to 32-bit color
    getPixelColor(uint8_t n) const;        // Return 32-bit pixel color
  uint8_t
    numPixels(void);                        // Return number of pixels
  uint8_t
    getBrightness(void) const,              // Return global brightness
   *getPixels(void) const;                  // Return pixel data pointer

 private:

  uint8_t
    numLEDs;                                // Number of pixels
  uint8_t
    brightness,                             // Global brightness setting
   *pixels,                                 // LED RGB values (3 bytes ea.)
    rOffset,                                // Index of red in 3-byte pixel
    gOffset,                                // Index of green byte
    bOffset;                                // Index of blue byte
  void
    sw_spi_init(void),                      // Start bitbang SPI
    sw_spi_out(uint8_t n),                  // Bitbang SPI write
    sw_spi_end(void);                       // Stop bitbang SPI

};

#endif // _DOTSTAR_HPP_
