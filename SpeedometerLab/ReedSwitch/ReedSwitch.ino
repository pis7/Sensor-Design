/*
Title: Calculate Linear Speed of Wheel from Reed Switch
Author: Parker Schless (pis7)
Date: 7/26/23
Version: 1.0

Summary:
This code reads the ADc value from a reed switch and uses the time between magnet detections (signal peaks) to calculate speed from
the equation v = d / t where d is the circumference of the wheel and t is the time between magnet detections.

References:
- ADC: https://www.electronicwings.com/arduino/adc-in-arduino, https://cdn.arduino.cc/reference/en/language/functions/analog-io/analogreference/
- Serial: https://www.arduino.cc/reference/en/language/functions/communication/serial/
*/

float maxDigVal = 1023; // Maximum digital value of ADC (10-bit ADC for the boards we are using)

// Initialize variables - CHANGE FOLLOWING PARAMETERS AS NEEDED (MUST MATCH IN PYTHON) -----------------------------------------
#define BAUDRATE 9600 // Serial baudrate
#define DIGPIN 2 // GPIO pin used for reed switch input
#define DELAYTIME 1 // Delay time in msec
#define CIRCUM 2*3.14159265*0.5625 // 2*pi*radius (ft)
float vref = 5.0; // Reference voltage used
int prevTime = 0;
int thisTime = 0;
bool getPtDone = false;


// Main functions, REMEMBER TO FLASH CODE TO ARDUINO IF CHANGES MADE -----------------------------------------------------------

void setup() {
 pinMode(DIGPIN, INPUT_PULLUP); // Configure GPIO pin to be pulled high when reed switch is off (not conducting)
 Serial.begin(BAUDRATE); // Sets up serial with specified baudrate
}

void loop() {
  int magState = digitalRead(DIGPIN);
  if (magState == HIGH && getPtDone == false) { // If pin goes high and it was not previously high then calculate speed and print to console
    prevTime = thisTime;
    thisTime = millis(); // Obtain time points
    float speed = CIRCUM / (float) (thisTime - prevTime); // v = d/t in ft/ms
    speed = 1000.0 * 3600.0 * speed / 5280.0; // get v in mph
    Serial.println(speed);
    getPtDone = true;
  }
  if (magState == LOW && getPtDone == true) { // If pin goes low and it was previously high then reset time trigger
    getPtDone = false;
  }
  delay(DELAYTIME); // delay for specified time before sending next sample
}