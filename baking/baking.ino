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

#include <PID_v1.h>

float maxDigVal = 1023; // Maximum digital value of ADC (10-bit ADC for the boards we are using)

// Initialize variables - CHANGE FOLLOWING PARAMETERS AS NEEDED (MUST MATCH IN PYTHON) -----------------------------------------
#define BAUDRATE 9600 // Serial baudrate
#define ANALOGPIN0 0 // Analog pin used for sensor input, V+
#define ANALOGPIN1 1 // Analog pin used for sensor input, GND
#define DELAYTIME 1 // Delay time in msec
#define RelayPin 22 
float vref = 5.0; // Reference voltage used

double Setpoint, Input, Output;
double aKp = 4, aKi = .2, aKd = 1;
double cKp = 5, cKi = .5, cKd = 0;

PID baking(&Input, &Output, &Setpoint, cKp, cKi, cKd, DIRECT);

// Main functions, REMEMBER TO FLASH CODE TO ARDUINO IF CHANGES MADE -----------------------------------------------------------

void setup() {
  analogReference(EXTERNAL); // Sets ADC reference voltage to AREF pin voltage
  Serial.begin(BAUDRATE); // Sets up serial with specified baudrate
  pinMode(RelayPin, OUTPUT);

  float digitalVal = analogRead(ANALOGPIN0) - analogRead(ANALOGPIN1); // Read in digital value in range 0 - 1023 from ADC pin
  float analogVoltage = vref*digitalVal/maxDigVal; // Convert digital value to analog voltage
  Input = analogVoltage;

  Setpoint = 2.7;
  baking.SetMode(AUTOMATIC);
  baking.SetOutputLimits(0, 1);
}

void loop() {
  float digitalVal = analogRead(ANALOGPIN0) - analogRead(ANALOGPIN1); // Read in digital value in range 0 - 1023 from ADC pin
  float analogVoltage = vref*digitalVal/maxDigVal; // Convert digital value to analog voltage
  Input = analogVoltage;
  
  double gap = abs(Setpoint - Input);
  //if(gap < 0.5) baking.SetTunings(cKp, cKi, cKd);
  //else baking.SetTunings(aKp, aKi, aKd);

  Serial.println(Input); // Write analog voltage to serial
  Serial.print(" ");
  Serial.println(Output);

  baking.Compute();

  if (Output > 0.5) digitalWrite(RelayPin, HIGH);
  else digitalWrite(RelayPin, LOW);



}