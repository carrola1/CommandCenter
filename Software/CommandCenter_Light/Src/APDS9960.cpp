/*!
 *  @file APDS9960.cpp
 *
 *  @mainpage Adafruit APDS9960 Proximity, Light, RGB, and Gesture Sensor
 *
 *  @section author Author
 *
 *  Ladyada, Dean Miller (Adafruit Industries)
 *
 *  @section license License
 *
 *  Software License Agreement (BSD License)
 *
 *  Copyright (c) 2017, Adafruit Industries
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holders nor the
 *  names of its contributors may be used to endorse or promote products
 *  derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <math.h>
#include "i2c.h"
#include "main.h"
#include "APDS9960.h"

/*!
 *  @brief  Implements missing powf function
 *  @param  x
 *          Base number
 *  @param  y
 *          Exponent
 *  @return x raised to the power of y
 */
float powf(const float x, const float y) {
  float d = 0;
  return d;
  //return (float)(pow((double)x, (double)y));
}

/*!
 *  @brief  Enables the device
 *          Disables the device (putting it in lower power sleep mode)
 *  @param  en
 *          Enable (True/False)
 */
void APDS9960::enable(bool en) {
  _enable.PON = en;
  this->write8(APDS9960_ENABLE, _enable.get());
}

/*!
 *  @brief  Initializes I2C and configures the sensor
 *  @param  iTimeMS
 *          Integration time
 *  @param  aGain
 *          Gain
 *  @param  addr
 *          I2C address
 *  @param  *theWire
 *          Wire object
 *  @return True if initialization was successful, otherwise false.
 */
bool APDS9960::begin(uint16_t iTimeMS, apds9960AGain_t aGain,
                                 uint8_t addr) {
  _i2caddr = addr;

  /* Make sure we're actually connected */
  uint8_t x = read8(APDS9960_ID);
  if (x != 0xAB) {
    return false;
  }

  /* Set default integration time and gain */
  setADCIntegrationTime(iTimeMS);
  setADCGain(aGain);

  // disable everything to start
  enableGesture(false);
  enableProximity(false);
  enableColor(false);

  disableColorInterrupt();
  disableProximityInterrupt();
  clearInterrupt();

  /* Note: by default, the device is in power down mode on bootup */
  enable(false);
  HAL_Delay(10);
  enable(true);
  HAL_Delay(10);

  // default to all gesture dimensions
  setGestureDimensions(APDS9960_DIMENSIONS_ALL);
  setGestureFIFOThreshold(APDS9960_GFIFO_4);
  setGestureGain(APDS9960_GGAIN_4);
  setGestureProximityThreshold(50);
  resetCounts();

  _gpulse.GPLEN = APDS9960_GPULSE_32US;
  _gpulse.GPULSE = 9; // 10 pulses
  this->write8(APDS9960_GPULSE, _gpulse.get());

  for (uint8_t i=0; i<len_color_filt; i++) {
    color_filt[i] = UNKNOWN;
  }

  return true;
}

/*!
 *  @brief  Sets the integration time for the ADC of the APDS9960, in millis
 *  @param  iTimeMS
 *          Integration time
 */
void APDS9960::setADCIntegrationTime(uint16_t iTimeMS) {
  float temp;

  // convert ms into 2.78ms increments
  temp = iTimeMS;
  temp /= 2.78;
  temp = 256 - temp;
  if (temp > 255)
    temp = 255;
  if (temp < 0)
    temp = 0;

  /* Update the timing register */
  write8(APDS9960_ATIME, (uint8_t)temp);
}

/*!
 *  @brief  Returns the integration time for the ADC of the APDS9960, in millis
 *  @return Integration time
 */
float APDS9960::getADCIntegrationTime() {
  float temp;

  temp = read8(APDS9960_ATIME);

  // convert to units of 2.78 ms
  temp = 256 - temp;
  temp *= 2.78;
  return temp;
}

/*!
 *  @brief  Adjusts the color/ALS gain on the APDS9960 (adjusts the sensitivity
 *          to light)
 *  @param  aGain
 *          Gain
 */
void APDS9960::setADCGain(apds9960AGain_t aGain) {
  _control.AGAIN = aGain;

  /* Update the timing register */
  write8(APDS9960_CONTROL, _control.get());
}

/*!
 *  @brief  Returns the ADC gain
 *  @return ADC gain
 */
apds9960AGain_t APDS9960::getADCGain() {
  return (apds9960AGain_t)(read8(APDS9960_CONTROL) & 0x03);
}

/*!
 *  @brief  Adjusts the Proximity gain on the APDS9960
 *  @param  pGain
 *          Gain
 */
void APDS9960::setProxGain(apds9960PGain_t pGain) {
  _control.PGAIN = pGain;

  /* Update the timing register */
  write8(APDS9960_CONTROL, _control.get());
}

/*!
 *  @brief  Returns the Proximity gain on the APDS9960
 *  @return Proxmity gain
 */
apds9960PGain_t APDS9960::getProxGain() {
  return (apds9960PGain_t)(read8(APDS9960_CONTROL) & 0x0C);
}

/*!
 *  @brief  Sets number of proxmity pulses
 *  @param  pLen
 *          Pulse Length
 *  @param  pulses
 *          Number of pulses
 */
void APDS9960::setProxPulse(apds9960PPulseLen_t pLen, uint8_t pulses) {
  if (pulses < 1)
    pulses = 1;
  if (pulses > 64)
    pulses = 64;
  pulses--;

  _ppulse.PPLEN = pLen;
  _ppulse.PPULSE = pulses;

  write8(APDS9960_PPULSE, _ppulse.get());
}

/*!
 *  @brief  Enable proximity readings on APDS9960
 *  @param  en
 *          Enable (True/False)
 */
void APDS9960::enableProximity(bool en) {
  _enable.PEN = en;

  write8(APDS9960_ENABLE, _enable.get());
}

/*!
 *  @brief  Enable proximity interrupts
 */
void APDS9960::enableProximityInterrupt() {
  _enable.PIEN = 1;
  write8(APDS9960_ENABLE, _enable.get());
  clearInterrupt();
}

/*!
 *  @brief  Disable proximity interrupts
 */
void APDS9960::disableProximityInterrupt() {
  _enable.PIEN = 0;
  write8(APDS9960_ENABLE, _enable.get());
}

/*!
 *  @brief  Set proxmity interrupt thresholds
 *  @param  low
 *          Low threshold
 *  @param  high
 *          High threshold
 *  @param  persistance
 *          Persistance
 */
void APDS9960::setProximityInterruptThreshold(uint8_t low,
                                                       uint8_t high,
                                                       uint8_t persistance) {
  write8(APDS9960_PILT, low);
  write8(APDS9960_PIHT, high);

  if (persistance > 7)
    persistance = 7;
  _pers.PPERS = persistance;
  write8(APDS9960_PERS, _pers.get());
}

/*!
 *  @brief  Returns proxmity interrupt status
 *  @return True if enabled, false otherwise.
 */
bool APDS9960::getProximityInterrupt() {
  _status.set(this->read8(APDS9960_STATUS));
  return _status.PINT;
};

/*!
 *  @brief  Read proximity data
 *  @return Proximity
 */
uint8_t APDS9960::readProximity() { return read8(APDS9960_PDATA); }

/*!
 *  @brief  Returns validity status of a gesture
 *  @return Status (True/False)
 */
bool APDS9960::gestureValid() {
  _gstatus.set(this->read8(APDS9960_GSTATUS));
  return _gstatus.GVALID;
}

/*!
 *  @brief  Sets gesture dimensions
 *  @param  dims
 *          Dimensions (APDS9960_DIMENSIONS_ALL, APDS9960_DIMENSIONS_UP_DOWM,
 *          APDS9960_DIMENSIONS_UP_DOWN, APGS9960_DIMENSIONS_LEFT_RIGHT)
 */
void APDS9960::setGestureDimensions(uint8_t dims) {
  _gconf3.GDIMS = dims;
  this->write8(APDS9960_GCONF3, _gconf3.get());
}

/*!
 *  @brief  Sets gesture FIFO Threshold
 *  @param  thresh
 *          Threshold (APDS9960_GFIFO_1, APDS9960_GFIFO_4, APDS9960_GFIFO_8,
 *          APDS9960_GFIFO_16)
 */
void APDS9960::setGestureFIFOThreshold(uint8_t thresh) {
  _gconf1.GFIFOTH = thresh;
  this->write8(APDS9960_GCONF1, _gconf1.get());
}

/*!
 *  @brief  Sets gesture sensor gain
 *  @param  gain
 *          Gain (APDS9960_GAIN_1, APDS9960_GAIN_2, APDS9960_GAIN_4,
 *          APDS9960_GAIN_8)
 */
void APDS9960::setGestureGain(uint8_t gain) {
  _gconf2.GGAIN = gain;
  this->write8(APDS9960_GCONF2, _gconf2.get());
}

/*!
 *  @brief  Sets gesture sensor threshold
 *  @param  thresh
 *          Threshold
 */
void APDS9960::setGestureProximityThreshold(uint8_t thresh) {
  this->write8(APDS9960_GPENTH, thresh);
}

/*!
 *  @brief  Sets gesture sensor offset
 *  @param  offset_up
 *          Up offset
 *  @param  offset_down
 *          Down offset
 *  @param  offset_left
 *          Left offset
 *  @param  offset_right
 *          Right offset
 */
void APDS9960::setGestureOffset(uint8_t offset_up, uint8_t offset_down,
                                         uint8_t offset_left,
                                         uint8_t offset_right) {
  this->write8(APDS9960_GOFFSET_U, offset_up);
  this->write8(APDS9960_GOFFSET_D, offset_down);
  this->write8(APDS9960_GOFFSET_L, offset_left);
  this->write8(APDS9960_GOFFSET_R, offset_right);
}

/*!
 *  @brief  Enable gesture readings on APDS9960
 *  @param  en
 *          Enable (True/False)
 */
void APDS9960::enableGesture(bool en) {
  if (!en) {
    _gconf4.GMODE = 0;
    write8(APDS9960_GCONF4, _gconf4.get());
  }
  _enable.GEN = en;
  write8(APDS9960_ENABLE, _enable.get());
  resetCounts();
}

/*!
 *  @brief  Resets gesture counts
 */
void APDS9960::resetCounts() {
  gestCnt = 0;
  UCount = 0;
  DCount = 0;
  LCount = 0;
  RCount = 0;
}


/*!
 *  @brief  Set LED brightness for proximity/gesture
 *  @param  drive
 *          LED Drive
 *  @param  boost
 *          LED Boost
 */
void APDS9960::setLED(apds9960LedDrive_t drive,
                               apds9960LedBoost_t boost) {
  // set BOOST
  _config2.LED_BOOST = boost;
  write8(APDS9960_CONFIG2, _config2.get());

  _control.LDRIVE = drive;
  write8(APDS9960_CONTROL, _control.get());
}

/*!
 *  @brief  Enable proximity readings on APDS9960
 *  @param  en
 *          Enable (True/False)
 */
void APDS9960::enableColor(bool en) {
  _enable.AEN = en;
  write8(APDS9960_ENABLE, _enable.get());
}

/*!
 *  @brief  Returns status of color data
 *  @return True if color data ready, False otherwise
 */
bool APDS9960::colorDataReady() {
  _status.set(this->read8(APDS9960_STATUS));
  return _status.AVALID;
}

/*!
 *  @brief  Reads the raw red, green, blue and clear channel values
 *  @param  *r
 *          Red value
 *  @param  *g
 *          Green value
 *  @param  *b
 *          Blue value
 *  @param  *c
 *          Clear channel value
 */
void APDS9960::getColorData(uint16_t *r, uint16_t *g, uint16_t *b,
                                     uint16_t *c) {

  *c = read16R(APDS9960_CDATAL);
  *r = read16R(APDS9960_RDATAL);
  *g = read16R(APDS9960_GDATAL);
  *b = read16R(APDS9960_BDATAL);
}

/*!
 *  @brief  Converts the raw R/G/B values to color temperature in degrees Kelvin
 *  @param  r
 *          Red value
 *  @param  g
 *          Green value
 *  @param  b
 *          Blue value
 *  @return Color temperature
 */
uint16_t APDS9960::calculateColorTemperature(uint16_t r, uint16_t g,
                                                      uint16_t b) {
  float X, Y, Z; /* RGB to XYZ correlation      */
  float xc, yc;  /* Chromaticity co-ordinates   */
  float n;       /* McCamy's formula            */
  float cct;

  /* 1. Map RGB values to their XYZ counterparts.    */
  /* Based on 6500K fluorescent, 3000K fluorescent   */
  /* and 60W incandescent values for a wide range.   */
  /* Note: Y = Illuminance or lux                    */
  X = (-0.14282F * r) + (1.54924F * g) + (-0.95641F * b);
  Y = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
  Z = (-0.68202F * r) + (0.77073F * g) + (0.56332F * b);

  /* 2. Calculate the chromaticity co-ordinates      */
  xc = (X) / (X + Y + Z);
  yc = (Y) / (X + Y + Z);

  /* 3. Use McCamy's formula to determine the CCT    */
  n = (xc - 0.3320F) / (0.1858F - yc);

  /* Calculate the final CCT */
  cct =
      (449.0F * powf(n, 3)) + (3525.0F * powf(n, 2)) + (6823.3F * n) + 5520.33F;

  /* Return the results in degrees Kelvin */
  return (uint16_t)cct;
}

/*!
 *  @brief  Calculate ambient light values
 *  @param  r
 *          Red value
 *  @param  g
 *          Green value
 *  @param  b
 *          Blue value
 *  @return LUX value
 */
uint16_t APDS9960::calculateLux(uint16_t r, uint16_t g, uint16_t b) {
  float illuminance;

  /* This only uses RGB ... how can we integrate clear or calculate lux */
  /* based exclusively on clear since this might be more reliable?      */
  illuminance = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);

  return (uint16_t)illuminance;
}

/*!
 *  @brief  Enables color interrupt
 */
void APDS9960::enableColorInterrupt() {
  _enable.AIEN = 1;
  write8(APDS9960_ENABLE, _enable.get());
}

/*!
 *  @brief  Disables color interrupt
 */
void APDS9960::disableColorInterrupt() {
  _enable.AIEN = 0;
  write8(APDS9960_ENABLE, _enable.get());
}

/*!
 *  @brief  Clears interrupt
 */
void APDS9960::clearInterrupt() {
  this->write(APDS9960_AICLEAR, 0, 1);
}

/*!
 *  @brief  Sets interrupt limits
 *  @param  low
 *          Low limit
 *  @param  high
 *          High limit
 */
void APDS9960::setIntLimits(uint16_t low, uint16_t high) {
  write8(APDS9960_AILTIL, low & 0xFF);
  write8(APDS9960_AILTH, low >> 8);
  write8(APDS9960_AIHTL, high & 0xFF);
  write8(APDS9960_AIHTH, high >> 8);
}

/*!
 *  @brief  Writes specified value to given register
 *  @param  reg
 *          Register to write to
 *  @param  value
 *          Value to write
 */
void APDS9960::write8(uint8_t reg, uint8_t value) {
  this->write(reg, value, 2);
}

/*!
 *  @brief  Reads 8 bits from specified register
 *  @param  reg
 *          Register to write to
 *  @return Value in register
 */
uint8_t APDS9960::read8(uint8_t reg) {
  uint8_t ret;
  this->read(reg, &ret, 1);

  return ret;
}

/*!
 *  @brief  Reads 32 bits from specified register
 *  @param  reg
 *          Register to write to
 *  @return Value in register
 */
uint32_t APDS9960::read32(uint8_t reg) {
  uint8_t ret[4];
  this->read(reg, ret, 4);

  return (ret[0] << 24) | (ret[1] << 16) | (ret[2] << 8) | ret[3];
}

/*!
 *  @brief  Reads 16 bites from specified register
 *  @param  reg
 *          Register to write to
 *  @return Value in register
 */
uint16_t APDS9960::read16(uint8_t reg) {
  uint8_t ret[2];
  this->read(reg, ret, 2);

  return (ret[0] << 8) | ret[1];
}

/*!
 *  @brief  Reads 16 bites from specified register
 *  @param  reg
 *          Register to write to
 *  @return Value in register
 */
uint16_t APDS9960::read16R(uint8_t reg) {
  uint8_t ret[2];
  this->read(reg, ret, 2);

  return (ret[1] << 8) | ret[0];
}


/*!
 *  @brief  Reads num bytes from specified register into a given buffer
 *  @param  reg
 *          Register
 *  @param  *buf
 *          Buffer
 *  @param  num
 *          Number of bytes
 *  @return Position after reading
 */
void APDS9960::read(uint8_t reg, uint8_t *buf, uint8_t num) {
    HAL_I2C_Master_Transmit(&hi2c1, APDS9960_ADDRESS, &reg, 1, HAL_MAX_DELAY);
    HAL_Delay(5);
    HAL_I2C_Master_Receive(&hi2c1, APDS9960_ADDRESS, &tempbuf[0], num, HAL_MAX_DELAY);
    for (uint8_t i=0; i<num; i++) {
      buf[i] = tempbuf[i];
    }
    HAL_Delay(5);
}

/*!
 *  @brief  Writes num bytes from specified buffer into a given register
 *  @param  reg
 *          Register
 *  @param  *buf
 *          Buffer
 *  @param  num
 *          Number of bytes
 */
void APDS9960::write(uint8_t reg, uint8_t value, uint8_t num) {
  uint8_t writedata[2] = {reg, value};
  HAL_I2C_Master_Transmit(&hi2c1, APDS9960_ADDRESS, &writedata[0], num, HAL_MAX_DELAY);
}

color_t APDS9960::colorSort(uint16_t r, uint16_t g, uint16_t b, uint32_t color_total, uint8_t color_to_find) {
  uint32_t rgb_total = r + g + b;
  float rPer = 1.0*r/rgb_total*100.0;
  float gPer = 1.0*g/rgb_total*100.0;
  float bPer = 1.0*b/rgb_total*100.0;
  color_t color;

  // Add fudge factor to improve probability of success
  if (color_to_find == RED) {
    rPer = rPer + 2;
    gPer = gPer - 2;
  } else if (color_to_find == ORANGE) {
    rPer = rPer + 2;
    gPer = gPer + 2;
  } else if (color_to_find == BLUE) {
    bPer = bPer + 2;
    rPer = rPer - 2;
  } else if (color_to_find == GREEN) {
    gPer = gPer + 2;
  } else if (color_to_find == YELLOW) {
    rPer = rPer + 2;
    gPer = gPer - 2;
  } else if (color_to_find == PURPLE) {
    bPer = bPer + 2;
    rPer = rPer + 2;
  } else if (color_to_find == PINK) {
    rPer = rPer + 2;
  }

  if ((rPer > 35.0) & (gPer <= 28.0)) {
    color = RED;
  } else if ((rPer > 36.0) & (gPer > 28.0)) {
    color = ORANGE;
  } else if ((bPer > 49.0) & (rPer < 20.0)) {
    color = BLUE;
  } else if (gPer > 38.5) {
    color = GREEN;
  } else if ((gPer > 34.0) & (rPer > 26.0)) {
    color = YELLOW;
  } else if ((bPer > 48.0) & (rPer > 21.0)) {
      color = PURPLE;
  } else if ((rPer > 28.0) & (bPer > 39.0)){
    color = PINK;
  } else {
    color = UNKNOWN;
  }

  // shift new value into color filter
  for (uint8_t i = 1; i < len_color_filt; i++) {
    color_filt[i-1] = color_filt[i];
  }
  color_filt[len_color_filt-1] = color;

  // check if all values in filter match
  uint8_t match = 0;
  for (uint8_t i = 1; i < len_color_filt; i++) {
    if (color_filt[i-1] == color_filt[i]) {
      match = match + 1;
    }
  }
  if (match == len_color_filt-1) {
    for (uint8_t i = 0; i < len_color_filt; i++) {
	  color_filt[i] = UNKNOWN;
	}
	return color;
  } else {
    return UNKNOWN;
  }
}

void APDS9960::calibrate_sensor(uint16_t *g_cal, uint16_t *b_cal, uint32_t *total_cal) {
  uint16_t r_samp; 
  uint16_t g_samp; 
  uint16_t b_samp;
  uint16_t c_samp;
  uint16_t r_avg; 
  uint16_t g_avg; 
  uint16_t b_avg; 
  uint16_t c_avg; 
  uint32_t r_total = 0;
  uint32_t g_total = 0;
  uint32_t b_total = 0;
  uint32_t c_total = 0;

  for (uint8_t i = 0; i<4; i++) {
    getColorData(&r_samp, &g_samp, &b_samp, &c_samp);
    r_total = r_total + r_samp;
    g_total = g_total + g_samp;
    b_total = b_total + b_samp;
    c_total = c_total + c_samp;
    HAL_Delay(50);
  }
  r_avg = r_total/4;
  g_avg = g_total/4;
  b_avg = b_total/4;
  c_avg = c_total/4;
  *g_cal = g_avg-r_avg;
  *b_cal = b_avg-r_avg;
  *total_cal = r_avg + g_avg + b_avg + c_avg;
}
