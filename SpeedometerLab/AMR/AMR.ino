/*
Title: Calculate Linear Speed of Wheel from AMR Sensor
Author: Parker Schless (pis7)
Date: 6/1/23
Version: 1.1

Summary:
This code reads the ADC value from an AMR sensor and uses the time between magnet detections (signal peaks) to calculate speed from
the equation v = d / t where d is the circumference of the wheel and t is the time between magnet detections.

References:
- ADC: https://www.electronicwings.com/arduino/adc-in-arduino, https://cdn.arduino.cc/reference/en/language/functions/analog-io/analogreference/
- Serial: https://www.arduino.cc/reference/en/language/functions/communication/serial/
*/

float maxDigVal = 1023; // Maximum digital value of ADC (10-bit ADC for the boards we are using)

// Initialize variables - CHANGE FOLLOWING PARAMETERS AS NEEDED (MUST MATCH IN PYTHON) -----------------------------------------
#define BAUDRATE 9600 // Serial baudrate
#define ANALOGPIN 0 // Analog pin used for Vout
#define DELAYTIME 1 // Delay time in msec
#define CIRCUM 2*3.14159265*0.5625 // 2*pi*radius (ft)
float vref = 5.0; // Reference voltage used
int prevTime = 0;
int thisTime = 0;
float threshold = 3.0;
bool getPtDone = false;
float thisDiff = 0;
float prevDiff = 0;

// Main functions, REMEMBER TO FLASH CODE TO ARDUINO IF CHANGES MADE -----------------------------------------------------------

void setup() {
 analogReference(EXTERNAL); // Sets ADC reference voltage to AREF pin voltage
 Serial.begin(BAUDRATE); // Sets up serial with specified baudrate
}

void loop() {
  prevDiff = thisDiff;
  float digitalVal0 = analogRead(ANALOGPIN); // Read in digital value in range 0 - 1023 from ADC pin
  float analogVoltage0 = vref*digitalVal0/maxDigVal; // Convert digital value to analog voltage

  Serial.print(0);
  Serial.print(" ");
  Serial.print(5);
  Serial.print(" ");
  Serial.println(analogVoltage0);

  if (analogVoltage0 > threshold && getPtDone == false) { // If large difference between new and previous values and crosses 
    prevTime = thisTime;
    thisTime = millis(); // Obtain time points
    float speed = CIRCUM / (float) (thisTime - prevTime); // v = d/t in ft/ms
    speed = 1000.0 * 3600.0 * speed / 5280.0; // get v in mph
    Serial.println(speed);
    getPtDone = true;
  }
  if (analogVoltage0 < threshold && getPtDone == true) { // Once no longer large distance, reset bool to trigger on next point
    getPtDone = false;
  }
  delay(DELAYTIME); // delay for specified time before sending next sample
}