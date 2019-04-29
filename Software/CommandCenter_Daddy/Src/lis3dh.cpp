/**************************************************************************/
/*!
    @file     LIS3DH.cpp
    @author   K. Townsend / Limor Fried (Adafruit Industries)
    @license  BSD (see license.txt)

    This is a library for the Adafruit LIS3DH Accel breakout board
    ----> https://www.adafruit.com/products/2809

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

    @section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/

#include "lis3dh.hpp"
#include "i2c.h"

/**************************************************************************/
/*!
    @brief  Instantiates a new LIS3DH class in I2C mode
*/
/**************************************************************************/
// I2C
LIS3DH::LIS3DH() {}

/**************************************************************************/
/*!
    @brief  Setups the HW (reads coefficients values, etc.)
*/
/**************************************************************************/
bool LIS3DH::begin(uint8_t i2caddr) {
  _i2caddr = i2caddr;

  /*
  Serial.println("Debug");
  
  for (uint8_t i=0; i<0x30; i++) {
    Serial.print("$");
    Serial.print(i, HEX); Serial.print(" = 0x");
    Serial.println(readRegister8(i), HEX);
  }
  */

  /* Check connection */
  uint8_t deviceid = readRegister8(LIS3DH_REG_WHOAMI);
  if (deviceid != 0x33)
  {
    /* No LIS3DH detected ... return false */
    //Serial.println(deviceid, HEX);
    return false;
  }

  // enable all axes, normal mode
  writeRegister8(LIS3DH_REG_CTRL1, 0x07);
  // 400Hz rate
  setDataRate(LIS3DH_DATARATE_400_HZ);

  // High res & BDU enabled
  writeRegister8(LIS3DH_REG_CTRL4, 0x88);

  // DRDY on INT1
  writeRegister8(LIS3DH_REG_CTRL3, 0x10);

  // Turn on orientation config
  //writeRegister8(LIS3DH_REG_PL_CFG, 0x40);

  // enable adcs
  writeRegister8(LIS3DH_REG_TEMPCFG, 0x80);

  /*
  for (uint8_t i=0; i<0x30; i++) {
    Serial.print("$");
    Serial.print(i, HEX); Serial.print(" = 0x");
    Serial.println(readRegister8(i), HEX);
  }
  */

  return true;
}


void LIS3DH::read(void) {
  // read x y z at once

  data_buf[0] = LIS3DH_REG_OUT_X_L | 0x80;
  HAL_I2C_Master_Transmit(&hi2c1, _i2caddr, &data_buf[0], 1, HAL_MAX_DELAY);
  HAL_I2C_Master_Receive(&hi2c1, _i2caddr, &data_buf[0], 6, HAL_MAX_DELAY);
  x = data_buf[0];
  y = data_buf[2];
  z = data_buf[4];
  x |= (uint16_t)data_buf[1] << 8;
  y |= (uint16_t)data_buf[3] << 8;
  z |= (uint16_t)data_buf[5] << 8;

  uint8_t range = getRange();
  uint16_t divider = 1;
  if (range == LIS3DH_RANGE_16_G) divider = 1365; // different sensitivity at 16g
  if (range == LIS3DH_RANGE_8_G) divider = 4096;
  if (range == LIS3DH_RANGE_4_G) divider = 8190;
  if (range == LIS3DH_RANGE_2_G) divider = 16380;

  x_g = (float)x / divider;
  y_g = (float)y / divider;
  z_g = (float)z / divider;

}

/**************************************************************************/
/*!
    @brief  Set INT to output for single or double click
*/
/**************************************************************************/

void LIS3DH::setClick(uint8_t c, uint8_t clickthresh, uint8_t timelimit, uint8_t timelatency, uint8_t timewindow) {
  if (!c) {
    //disable int
    uint8_t r = readRegister8(LIS3DH_REG_CTRL3);
    r &= ~(0x80); // turn off I1_CLICK
    writeRegister8(LIS3DH_REG_CTRL3, r);
    writeRegister8(LIS3DH_REG_CLICKCFG, 0);
    return;
  }
  // else...

  writeRegister8(LIS3DH_REG_CTRL3, 0x80); // turn on int1 click
  writeRegister8(LIS3DH_REG_CTRL5, 0x08); // latch interrupt on int1

  if (c == 1)
    writeRegister8(LIS3DH_REG_CLICKCFG, 0x15); // turn on all axes & singletap
  if (c == 2)
    writeRegister8(LIS3DH_REG_CLICKCFG, 0x2A); // turn on all axes & doubletap


  writeRegister8(LIS3DH_REG_CLICKTHS, clickthresh); // arbitrary
  writeRegister8(LIS3DH_REG_TIMELIMIT, timelimit); // arbitrary
  writeRegister8(LIS3DH_REG_TIMELATENCY, timelatency); // arbitrary
  writeRegister8(LIS3DH_REG_TIMEWINDOW, timewindow); // arbitrary
}

uint8_t LIS3DH::getClick(void) {
  return readRegister8(LIS3DH_REG_CLICKSRC);
}


/**************************************************************************/
/*!
    @brief  Sets the g range for the accelerometer
*/
/**************************************************************************/
void LIS3DH::setRange(lis3dh_range_t range)
{
  uint8_t r = readRegister8(LIS3DH_REG_CTRL4);
  r &= ~(0x30);
  r |= range << 4;
  writeRegister8(LIS3DH_REG_CTRL4, r);
}

/**************************************************************************/
/*!
    @brief  Sets the g range for the accelerometer
*/
/**************************************************************************/
lis3dh_range_t LIS3DH::getRange(void)
{
  /* Read the data format register to preserve bits */
  return (lis3dh_range_t)((readRegister8(LIS3DH_REG_CTRL4) >> 4) & 0x03);
}

/**************************************************************************/
/*!
    @brief  Sets the data rate for the LIS3DH (controls power consumption)
*/
/**************************************************************************/
void LIS3DH::setDataRate(lis3dh_dataRate_t dataRate)
{
  uint8_t ctl1 = readRegister8(LIS3DH_REG_CTRL1);
  ctl1 &= ~(0xF0); // mask off bits
  ctl1 |= (dataRate << 4);
  writeRegister8(LIS3DH_REG_CTRL1, ctl1);
}

/**************************************************************************/
/*!
    @brief  Sets the data rate for the LIS3DH (controls power consumption)
*/
/**************************************************************************/
lis3dh_dataRate_t LIS3DH::getDataRate(void)
{
  return (lis3dh_dataRate_t)((readRegister8(LIS3DH_REG_CTRL1) >> 4)& 0x0F);
}

/**************************************************************************/
/*!
    @brief  Writes 8-bits to the specified destination register
*/
/**************************************************************************/
void LIS3DH::writeRegister8(uint8_t reg, uint8_t value) {
  data_buf[0] = reg;
  data_buf[1] = value;
  HAL_I2C_Master_Transmit(&hi2c1, _i2caddr, &data_buf[0], 2, HAL_MAX_DELAY);
}

/**************************************************************************/
/*!
    @brief  Reads 8-bits from the specified register
*/
/**************************************************************************/
uint8_t LIS3DH::readRegister8(uint8_t reg) {
  data_buf[0] = reg;
  HAL_I2C_Master_Transmit(&hi2c1, _i2caddr, &data_buf[0], 1, HAL_MAX_DELAY);
  HAL_I2C_Master_Receive(&hi2c1, _i2caddr, &data_buf[0], 1, HAL_MAX_DELAY);
  return data_buf[0];
}
