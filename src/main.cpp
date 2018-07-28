/*
 * MIT License
 * Copyright 2018 Bolukan
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * I2C Scanner
 * Scans 7-bit address space
 * https://www.nxp.com/docs/en/user-guide/UM10204.pdf
 */

#include <Arduino.h>

// TwoWire.h - TWI/I2C library for Arduino & Wiring
#include <Wire.h>

#define I2C_ADDRESS_MIN                         8
#define I2C_ADDRESS_MAX                       127

#define I2C_ERROR_SUCCESS                       0
#define I2C_ERROR_DATA_TOO_LONG                 1
#define I2C_ERROR_NACK_ON_TRANSMIT_OF_ADDRESS   2
#define I2C_ERROR_NACK_ON_TRANSMIT_OF_DATA      3
#define I2C_ERROR_OTHER_ERROR                   4

/*
 *   Convert byte to hex '0x00'
 */
char* ByteToHexChar(byte b)
{
  static char res[5] = "0x..";
  res[2] = (b >> 4)   | '0'; if (res[2] > '9') res[2] += 7;
  res[3] = (b & 0x0F) | '0'; if (res[3] > '9') res[3] += 7;
  return res;
}

void setup()
{
  // Serial
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");

  // I2C
  Wire.begin();

  // Led off
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  byte error, address;

  Serial.println("\nScanning ...");

  // Header of table
  Serial.print("    ");
  for(address = 0; address < 0x10; address++)
  {
    Serial.print(" ");
    Serial.print(ByteToHexChar(address));
  }

  // Loop all addresses
  for(address = 0; address <= I2C_ADDRESS_MAX; address++)
  {
    // new line
    if ((address & 0x0F) == 0)
    {
      Serial.println();
      Serial.print(ByteToHexChar(address));
    }

    // don't scan but insert spaces for non-used address space
    if ((address < I2C_ADDRESS_MIN) || (address > I2C_ADDRESS_MAX))
    {
      Serial.print("     ");
    }
    else
    {
      // Test address
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
      if (error == I2C_ERROR_SUCCESS)
      {
        // success: slave responses to address
        Serial.print(" ");
        Serial.print(ByteToHexChar(address));
      }
      else
      {
        Serial.print(" ....");
      }
    }
  }
  Serial.println();
  delay(10000);

  // Serial.println("Finished. Sleeping for 10 seconds");
  // ESP.deepSleep(10e6);
}
