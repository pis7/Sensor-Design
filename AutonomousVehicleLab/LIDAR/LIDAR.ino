#include <Wire.h>

#define LIDARADDR 0x62 // I2C address of sensor

int busy_flag = 0;
uint8_t upper_reading = 0x00;
uint8_t lower_reading = 0x00;
char output[100];

void setup() {
  Wire.begin();
  Wire.setClock(400000); // Sensor operates in fast mode (400 kHz)
  Serial.begin(9600);
}

void loop() {

  // Setup sensor
  Wire.beginTransmission(LIDARADDR);
  Wire.write(byte(0x00)); // ACQ_COMMAND register
  Wire.write(byte(0x04)); // Take distance measurement with receiver bias correction
  Wire.endTransmission();

  delay(70);

  // Wait until device is ready for new command
  while (busy_flag == 1) {
    Wire.beginTransmission(LIDARADDR);
    Wire.write(0x01); // Poll STATUS register
    Wire.endTransmission(); 
    Wire.requestFrom(LIDARADDR, 1);
    if (Wire.available() == 1) {
      busy_flag = Wire.read() & 0x01; // Busy flag is LSB of register - high if device busy, low if device ready for new command
    }
  }

  // Get high byte of reading
  Wire.beginTransmission(LIDARADDR);
  Wire.write(0x0F); // FULL_DELAY_HIGH register
  Wire.endTransmission();
  Wire.requestFrom(LIDARADDR, 1);
  if (Wire.available() == 1){
    upper_reading = Wire.read(); // Receive upper byte of 16-bit reading
  }

  // Get low byte of reading
  Wire.beginTransmission(LIDARADDR);
  Wire.write(0x10); // FULL_DELAY_LOW register
  Wire.endTransmission();
  Wire.requestFrom(LIDARADDR, 1);
  if (Wire.available() == 1){
    lower_reading = Wire.read(); // Receive lower byte of 16-bit reading
  }
  
  // Concatenate full 16-bit reading and format output string
  sprintf(output, "Distance: %u cm", ((upper_reading << 8) | lower_reading));
  Serial.println(output);
}
