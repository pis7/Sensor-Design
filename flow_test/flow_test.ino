#define TransducerWriteUpstream   22
#define TransducerWriteDownstream 23
#define TransducerReadUpstream 0
#define TransducerReadDownstream 1

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(TransducerWriteUpstream, OUTPUT);
  pinMode(TransducerWriteDownstream, OUTPUT);
  digitalWrite(TransducerWriteUpstream, LOW);
  digitalWrite(TransducerWriteDownstream, LOW); 
}

void loop() {
  int current_time = micros();
  int cycle = current_time / 10000;
  int start_time;
  int end_time; 
  int diff;

  float TX = 5 * analogRead(1) / 1023; 

  if (cycle < 0) {
    cycle = cycle * -1;
  }
  //Serial.println(cycle);

  if (cycle % 2 == 0) {
    digitalWrite(TransducerWriteUpstream, HIGH);
    start_time = current_time;
    if (abs(TX) > 0.16) {
      end_time = current_time;
      diff = abs(end_time - start_time);
      Serial.println(diff);
    }
  }

  else {
    digitalWrite(TransducerWriteUpstream, LOW);
  }

  /*
  int current_time = micros();  
  
  // Want to have a toggle where one of the transducers is on within 5 second intervals
  int cycle = current_time / 2500;
  if (cycle < 0) {
    cycle = cycle * -1;
  }

  if (cycle % 4 == 0) {
    digitalWrite(TransducerWriteDownstream, LOW);
    digitalWrite(TransducerWriteUpstream, HIGH);
    Serial.print(0);
    Serial.print(" ");
    //digitalVal = analogRead(TransducerReadUpstream) - analogRead(TransducerReadDownstream);
  }
  if (cycle % 4 == 1) {
    digitalWrite(TransducerWriteDownstream, LOW);
    digitalWrite(TransducerWriteUpstream, LOW);
    Serial.print(1);
    Serial.print(" ");
  }
  if (cycle % 4 == 2) {
    // digitalWrite(TransducerWriteDownstream, HIGH);
    // digitalWrite(TransducerWriteUpstream, LOW);
    // Serial.print(2);
    // Serial.print(" ");
    //digitalVal = analogRead(TransducerReadDownstream) - analogRead(TransducerReadUpstream);
  }
  if (cycle % 4 == 3) {
    digitalWrite(TransducerWriteDownstream, LOW);
    digitalWrite(TransducerWriteUpstream, LOW);
    Serial.print(3);
    Serial.print(" ");
  }
  
  float upstream = abs(5.0 * analogRead(TransducerReadUpstream) / 1023);
  float downstream = abs(5.0 * analogRead(TransducerReadDownstream) / 1023);
  float digitalVal = abs(analogRead(TransducerReadUpstream) - analogRead(TransducerReadDownstream));
  float diff = 5.0 * digitalVal/1023;
  // Serial.print(0);
  // Serial.print(" ");
  // Serial.print(2);
  // Serial.print(" ");
  Serial.print(upstream);
  Serial.print(" ");
  Serial.println(downstream);
  // Serial.print(downstream);
  // Serial.print(" ");
  // Serial.println(diff);
  */


    /*
  digitalWrite(TransducerWriteDownstream, LOW);
  delay(1);
  digitalWrite(TransducerWriteUpstream, HIGH);
  float upstream = 5.0 * analogRead(TransducerReadUpstream) / 1023;
  float downstream = 5.0 * analogRead(TransducerReadDownstream) / 1023;
  float digitalVal = abs(analogRead(TransducerReadUpstream) - analogRead(TransducerReadDownstream));
  float diff = 5.0 * digitalVal/1023;
  Serial.print(0);
  Serial.print(" ");
  Serial.print(1.2);
  Serial.print(" ");
  Serial.print(upstream);
  Serial.print(" ");
  Serial.println(downstream);
  delay(3);
  digitalWrite(TransducerWriteUpstream, LOW);
  delay(1);
  digitalWrite(TransducerWriteDownstream, HIGH);
  upstream = 5.0 * analogRead(TransducerReadUpstream) / 1023;
  downstream = 5.0 * analogRead(TransducerReadDownstream) / 1023;
  digitalVal = abs(analogRead(TransducerReadUpstream) - analogRead(TransducerReadDownstream));
  diff = 5.0 * digitalVal/1023;
  Serial.print(0);
  Serial.print(" ");
  Serial.print(1.2);
  Serial.print(" ");
  Serial.print(upstream);
  Serial.print(" ");
  Serial.println(downstream);
  delay(1.5);
  */
  //Serial.print(" ");
  //Serial.println(diff);
  
  /*
  TX = 5 * analogRead(TransducerReadDownstream) / 1023;
  RX = 5 * analogRead(TransducerReadUpstream) / 1023;
  Serial.println(TX);
  Serial.print(",");
  Serial.println(RX);
  Serial.print(",");

  Serial.println(difference);
  Serial.print(",");




  int cycle = current_time / 1000;
  int start_time = current_time - 1000 * cycle; 
  int difference, RX;
  Serial.println(11);
  if (cycle % 2 == 0) {
    RX = 5 * analogRead(TrandsucerReadDownstream)/1023;
    digitalWrite(TransducerWriteUpstream, HIGH);
    if (RX >= 0.2) {
      difference = start_time - current_time;
      Serial.println("first");
      Serial.println(difference);
    } 
    else{
      Serial.println(0);
    }

  }
  else{
     RX = 5 * analogRead(TransducerReadUpstream)/1023;
    digitalWrite(TransducerWriteDownstream, HIGH);
    if (RX >= 0.2) {
      difference = start_time - current_time;
      Serial.println("second");
      Serial.println(difference);
    } 
    else {
      Serial.println(0);
    }
    }
    */
  }


  