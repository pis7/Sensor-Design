/*
Title: Read ADC Values and Send to Serial
Author: Parker Schless (pis7)
Date: 6/1/23
Version: 1.0

References:
- ADC: https://www.electronicwings.com/arduino/adc-in-arduino, https://cdn.arduino.cc/reference/en/language/functions/analog-io/analogreference/
- Serial: https://www.arduino.cc/reference/en/language/functions/communication/serial/
*/

float maxDigVal = 1023; // Maximum digital value of ADC (10-bit ADC)

// Initialize variables - CHANGE FOLLOWING PARAMETERS AS NEEDED (MUST MATCH IN PYTHON) -----------------------------------------
#define BAUDRATE 9600 // Serial baudrate
float vref = 5.0; // Reference voltage used
int delayTime = 100; // Delay time in msec

// Main functions, REMEMBER TO FLASH CODE TO ARDUINO IF CHANGES MADE -----------------------------------------------------------

void setup() {
 analogReference(EXTERNAL); // Sets ADC reference voltage to AREF pin voltage
 Serial.begin(BAUDRATE); // Sets up serial with specified baudrate
}

void loop() {
  float digitalVal = analogRead(0); // Read in digital value in range 0 - 1023 from ADC pin 0 (change number if using different pin)
  float analogVoltage = vref*digitalVal/maxDigVal; // Convert digital value to analog voltage 
  Serial.println(analogVoltage); // Write analog voltage to serial
  delay(delayTime); // delay for specified time before sending next sample
}