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
#define ANALOGPIN 0 // Analog pin used for sensor input
#define LEDPIN 52 // Digital pin used for LED
#define DELAYTIME 1 // Delay time in msec
#define LEDDELAY 100 // Time until LED can turn off
float vref = 5.0; // Reference voltage used
int ledTime = 0; // Time since LED turned on, used for delay function

// Change this based on sensor used
float threshold = 0.1; // Voltage threshold for LED to turn on
float dclevel = 0.5; // Voltage of DC bias point

// Main functions, REMEMBER TO FLASH CODE TO ARDUINO IF CHANGES MADE -----------------------------------------------------------

void setup() {
 analogReference(EXTERNAL); // Sets ADC reference voltage to AREF pin voltage
 pinMode(LEDPIN, OUTPUT); // Set pin 
 Serial.begin(BAUDRATE); // Sets up serial with specified baudrate
}

void loop() {
  float digitalVal = analogRead(ANALOGPIN); // Read in digital value in range 0 - 1023 from ADC pin
  float analogVoltage = vref*digitalVal/maxDigVal; // Convert digital value to analog voltage

  // To print out live view of data, change bounds accordingly
  Serial.print(0);
  Serial.print(" ");
  Serial.print(1);
  Serial.print(" ");
  Serial.println(analogVoltage);

  if (abs(analogVoltage - dclevel) >= threshold) {
    digitalWrite(LEDPIN, LOW);
    ledTime = 0;
  } else if (ledTime > LEDDELAY) {
    digitalWrite(LEDPIN, HIGH);
    ledTime = 0;
  }
  ledTime++;
  delay(DELAYTIME); // delay for specified time before sending next sample
}
