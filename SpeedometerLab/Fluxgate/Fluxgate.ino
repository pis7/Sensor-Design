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
#define ANALOGPIN0 0 // Analog pin used for Vout
#define ANALOGPIN1 1
#define DELAYTIME 1 // Delay time in msec
#define CIRCUM 2*3.14159265*0.5625 // 2*pi*radius (ft)
float vref = 5.0; // Reference voltage used
int prevTime = 0;
int thisTime = 0;
float threshold = 0.02;
bool getPtDone = false;
int nSamples = 0;
float sampSum = 0;

// Main functions, REMEMBER TO FLASH CODE TO ARDUINO IF CHANGES MADE -----------------------------------------------------------

void setup() {
 analogReference(EXTERNAL); // Sets ADC reference voltage to AREF pin voltage
 pinMode(ANALOGPIN0, INPUT);
 pinMode(ANALOGPIN1, INPUT);
 Serial.begin(BAUDRATE); // Sets up serial with specified baudrate
}

void loop() {
  nSamples++;
  float digitalVal0 = analogRead(ANALOGPIN0); // Read in digital value in range 0 - 1023 from ADC pin
  float analogVoltage0 = vref*digitalVal0/maxDigVal; // Convert digital value to analog voltage
  float digitalVal1 = analogRead(ANALOGPIN1); // Read in digital value in range 0 - 1023 from ADC pin
  float analogVoltage1 = vref*digitalVal1/maxDigVal; // Convert digital value to analog voltage
  sampSum += abs(analogVoltage1 - analogVoltage0);
  if (nSamples == 3){
    float avg = sampSum/3;
    // Serial.print(0);
    // Serial.print(" ");
    // Serial.print(0.1);
    // Serial.print(" ");
    // Serial.println(avg);
    if (avg < threshold && getPtDone == false) { // If large difference between new and previous values and crosses
      prevTime = thisTime;
      thisTime = millis(); // Obtain time points
      float speed = CIRCUM / (float) (thisTime - prevTime); // v = d/t in ft/ms
      speed = 1000.0 * 3600.0 * speed / 5280.0; // get v in mph
      Serial.println(speed);
      getPtDone = true;
    }
    if (avg > threshold && getPtDone == true) { // Once no longer large distance, reset bool to trigger on next point
      getPtDone = false;
    }
    sampSum = 0;
    nSamples = 0;
  }
  
  delay(DELAYTIME); // delay for specified time before sending next sample
}
