/*
Title: Read ADC Values and Send to Serial
Author: Parker Schless (pis7)
Date: 6/1/23
Version: 1.1

Summary:
This is the Arduino support code for plotting sensor data in the Sensor Design course. This code initializes some variables including reference
voltage used and delay time between samples which must match those in the additional Python code. Then, it simply reads in an analog voltage from
a specified analog pin on the Arduino every delayTime milliseconds and converts it from the digital value back to a voltage to be sent over serial
and used by the Python plotting code.

References:
- ADC: https://www.electronicwings.com/arduino/adc-in-arduino, https://cdn.arduino.cc/reference/en/language/functions/analog-io/analogreference/
- Serial: https://www.arduino.cc/reference/en/language/functions/communication/serial/
*/

float maxDigVal = 1023; // Maximum digital value of ADC (10-bit ADC for the boards we are using)

// Initialize variables - CHANGE FOLLOWING PARAMETERS AS NEEDED (MUST MATCH IN PYTHON) -----------------------------------------
#define BAUDRATE 9600 // Serial baudrate
#define ANALOGPIN0 0 // Analog pin used for temperature
#define ANALOGPIN1 1 // Analog pin used for humidity
#define DELAYTIME 100 // Delay time in msec
float vref = 5.0; // Reference voltage used

// Main functions, REMEMBER TO FLASH CODE TO ARDUINO IF CHANGES MADE -----------------------------------------------------------

void setup() {
 analogReference(EXTERNAL); // Sets ADC reference voltage to AREF pin voltage
 Serial.begin(BAUDRATE); // Sets up serial with specified baudrate
}

void loop() {
  float digitalVal0 = analogRead(ANALOGPIN0); // Read in digital value in range 0 - 1023 from ADC pin
  float analogVoltage0 = vref*digitalVal0/maxDigVal; // Convert digital value to analog voltage
  float digitalVal1 = analogRead(ANALOGPIN1); // Read in digital value in range 0 - 1023 from ADC pin
  float analogVoltage1 = vref*digitalVal1/maxDigVal; // Convert digital value to analog voltage

  // Comment below out if don't want bounds on serial plotter (if using this code to take actual data then having the bounds will introduce unwanted values)
  Serial.print(0);
  Serial.print(" ");
  Serial.print(1);
  Serial.print(" ");
  // ---
  
  //Serial.println(analogVoltage0); // Write analog voltage to serial
  //Serial.println(analogVoltage1); // Write analog voltage to serial
  Serial.println(analogVoltage0 - analogVoltage1);
  delay(DELAYTIME); // delay for specified time before sending next sample
}