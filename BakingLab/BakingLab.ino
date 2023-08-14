/*
Title: Read ADC Values and Send to Serial
Author: Karl Chen (kc684)
Date: 8/10/23
Version: 1.1
Summary:
This is the Arduino code to enable PID for the hacked hotplate. The code utilizes the PID library sourced from Brett 
Beauregard, and the individual tuning values for the PID controller have been experimentally tested and 
set. The code also utilizes the same serial functionality from the black box lab, which allows the user to 
read data from their computers serially.

The PID library used can be referenced at this link: https://playground.arduino.cc/Code/PIDLibrary/
*/

#include <PID_v1.h>

float maxDigVal = 1023; // Maximum digital value of ADC (10-bit ADC for the boards we are using)

// Initialize variables - CHANGE FOLLOWING PARAMETERS AS NEEDED (MUST MATCH IN PYTHON) -----------------------------------------
#define BAUDRATE 9600 // Serial baudrate
#define ANALOGPIN0 0 // Analog pin used for sensor input, V+
#define ANALOGPIN1 1 // Analog pin used for sensor input, GND
#define DELAYTIME 1 // Delay time in msec
#define RelayPin 22 // Digital Output pin, set either high or low
float vref = 5.0; // Reference voltage used

double Setpoint, Input, Output;
double Kp = 5, Ki = .5, Kd = 0;

PID baking(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// Main functions, REMEMBER TO FLASH CODE TO ARDUINO IF CHANGES MADE -----------------------------------------------------------

void setup() {
  analogReference(EXTERNAL); // Sets ADC reference voltage to AREF pin voltage
  Serial.begin(BAUDRATE); // Sets up serial with specified baudrate
  pinMode(RelayPin, OUTPUT);

  float digitalVal = analogRead(ANALOGPIN0) - analogRead(ANALOGPIN1); // Read in digital value in range 0 - 1023 from ADC pin
  float analogVoltage = vref*digitalVal/maxDigVal; // Convert digital value to analog voltage
  Input = analogVoltage;

  Setpoint = 2.7;               // Change value to respective voltage for 400 degrees F
  baking.SetMode(AUTOMATIC);    // Turn PID controller on
  baking.SetOutputLimits(0, 1); // Bound the output of the controller in between 0 and 1
}

void loop() {
  float digitalVal = analogRead(ANALOGPIN0) - analogRead(ANALOGPIN1); // Read in digital value in range 0 - 1023 from ADC pin
  float analogVoltage = vref*digitalVal/maxDigVal; // Convert digital value to analog voltage
  Input = analogVoltage;
 
  Serial.println(Input); // Write analog voltage to serial
  Serial.print(" ");
  Serial.println(Output);

  baking.Compute();

  if (Output > 0.5) digitalWrite(RelayPin, HIGH);   // If PID output is greater than 0.5, then the hotplate heats up
  else digitalWrite(RelayPin, LOW);                 // Else, the hotplate is turned off, and the system cools down



}