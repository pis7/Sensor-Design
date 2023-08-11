/*
* A Sensitive DIY Ultrasonic Range Sensor
* http://www.kerrywong.com
*/

#define FREQ 40000.0 // Hz
#define DUTYCYCLE 0.5 // 0 - 1 (0% - 100%)

// NOTE: THIS CODE IS CALIBRATED FOR RANGES UP TO 20 FT
#define RECVWINDOW 256 // 256 (empirically) corresponds to about 20 ms to search for peak value (can change this depending on desired range), assume peak comes from 1st echo

byte a = 0; // ADC value of current reading
unsigned long t_start = 0; // Start time after transducer sends signal
unsigned long t_peak = 0; // Time of largest peak
unsigned long t = 0; // Time of current reading
byte v_peak = 0; // ADC value of peak
const float SPEED_OF_SOUND_20C = 0.0003432; //per micro-second
float d = 0; // Distance in meters

void setup()
{
  Serial.begin(9600);

  pinMode(12, OUTPUT);
}

void loop()
{  
  // Generate PWM pulse on transducer
  startTransducer(FREQ, DUTYCYCLE);
  delayMicroseconds(300);
  stopTransducer();
  //delay(1000);

  v_peak = 0;
  t_start = micros(); // Get start time
  t_peak = t_start;
  delayMilliseconds(1); // Wait for initial receiver signal to fade

  
  for (int i = 0; i < RECVWINDOW; i++) {
    a = analogRead(0); // Read receiver analog pin
    t = micros();

    if (a > v_peak) { // If reading greater than previously detected max peak value then set new time and peak values
      t_peak = t;
      v_peak = a;
    }
  }

  t = t_peak - t_start; // Calculate time difference based on initial time and time of greates peak in interval
  d = (float) t * SPEED_OF_SOUND_20C / 2.0; // dist = time * c / 2
  Serial.println(d , 2); // Print distance to two decimal places in meters
}

void delayMilliseconds(int ms) {
  for (int i = 0; i < ms; i++) {
    delayMicroseconds(1000);
  }
}

void stopTransducer()
{
  cli(); // Disable interrupts
  TCCR1B = 0; // Disable PWM on timer register
  sei(); // Enable interrupts
  digitalWrite(12, LOW); // For OC1B
}

void startTransducer(float freq, float dutyCycle)
{
  if (dutyCycle > 0.5) dutyCycle = 0.5; // Make duty cycle in range of 0% to 50% if not already
  else if (dutyCycle < 0) dutyCycle = 0;

  cli(); // Disable interupts
  TCCR1B = _BV(WGM13) | _BV(CS10) | _BV(ICNC1); // Set Timer/Counter1 Control Register B with 10010001 for PWM
  //f0 = fclk / (2 * N * Top)
  long topv = (long) ((float) F_CPU /(freq * 2.0 * 1.0)); 
  ICR1 = topv; // Set Input Capcture Register with topv

  OCR1B = (int) ((float) topv * (dutyCycle)); // Set Ouput Capture Register lB
  DDRB |= _BV(PORTB6); // Set Port B data direction register pin B6 to high (output)
  TCCR1A = _BV(COM1A1) | _BV(COM1B1); // Set Timer/Counter1 Control Register A with 10100000
  sei(); // Enable interrupts
}