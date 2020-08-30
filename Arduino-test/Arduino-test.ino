const int ledPin = 13;      // the pin that the LED is attached to
float out = 0;
int startStopwatchTime = millis();

void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  // initialize the ledPin as an output:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  byte in;

  // check if data has been sent from the computer:
  if (Serial.available()) {
    // read the most recent byte (which will be from 0 to 255):
    in = Serial.read();
    if (in == 65)
    {
      digitalWrite(ledPin,HIGH);
      delay(10);
    }
    if (in == 66)
    {
      digitalWrite(ledPin,HIGH);
      delay(500);
    }
    
  }
  else
  {
    out = (millis() - startStopwatchTime)/1000;
    out = 200 * sin(out)+300;
    Serial.println((int)out);
    digitalWrite(ledPin,LOW);
   
    }
}
