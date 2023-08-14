/*
Title: Read CO2 Level from Integrated CO2 Sensor
Author: Parker Schless (pis7)
Date: 6/1/23
Version: 1.1

Summary:
This code reads the CO2 level from the interated CO2 sensor in ppm using I2C

References:
- Wire: https://www.arduino.cc/reference/en/language/functions/communication/wire/
*/

#include <Wire.h>
#define ADDR 0x52 // I2C peripheral address of sensor

bool new_data = false;

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {

  // Change opmode to standard gas sensing mode (starts up in idle mode from POR)
  Wire.begin(ADDR);
  Wire.write(byte(0x10)); // Address of OPMODE register
  Wire.write(byte(0x02)); // OPMODE register
  Wire.endTransmission();

  // Read status register until new data ready
  while (new_data == false) {
    Wire.begin(ADDR);
    Wire.write(byte(0x20)); // DEVICE_STATUS register
    Wire.endTransmission();
    delay(70);
    Wire.requestFrom(ADDR, 1); // Read 1 byte from status register
    if (Wire.available() == 1) {
      if ((Wire.read() & 0x02) != 0) { // New data avilable if bit 1 (NEWDAT) is high
        new_data = true;
      }
    }
  }
  new_data = false;

  // Read eCO2
  Wire.begin(ADDR);
  Wire.write(byte(0x24)); // DATA_ECO2 register
  Wire.endTransmission();
  delay(70);
  Wire.requestFrom(ADDR, 2); // Read 2 bytes from eCO2 register
  uint16_t reading = (Wire.read() << 8) | Wire.read(); // Shift MSB to left 8 bits and bitwise or with LSB to get full 16-bit eCO2 value
  char output[256];
  sprintf(output, "eCO2: %u ppm", reading); // Format output string
  Serial.println("eCO2");
}
