
int base = 2;
int gearPin = A0;
int gearValue = 0;
int gear = 0;
int dimPin = 9;
int dimValue = 127;
int watchdogPin = 13;
int watchdogCounter = 0;

// the setup routine runs once when you press reset:
void setup() {
   int i =0;  
  // initialize the digital pin as an output.
  for (i=0; i < 6; i++) {
    pinMode(base + i, OUTPUT);
  }
  pinMode(dimPin, OUTPUT);
  pinMode(watchdogPin, OUTPUT);
  analogWrite(dimPin, dimValue);
  Serial.begin(9600);
  // Sweep
  for (i=1; i < 7; i++) {
    gearlight(i);
    delay(90);
  }
  for (i=6; i > 0; i--) {
    gearlight(i);
    delay(90);
  }
}

// the loop routine runs over and over again forever:
void loop() {
  gearValue = analogRead(gearPin);
  // Serial.println(gearValue, DEC);
  if (gearValue < 160) {
    gear = 1;
  } else if (gearValue < 320) {
    gear = 2;
  } else if (gearValue < 480) {
    gear = 3;
  } else if (gearValue < 650) {
    gear = 4;
  } else if (gearValue < 800) {
    gear = 5;
  } else if (gearValue < 960) {
    gear = 6;
  } else {
    gear = 0;
  }
  gearlight(gear);
  analogWrite(dimPin, dimValue);
  if ((watchdogCounter % 20) == 0) {
    digitalWrite(watchdogPin, HIGH);
  } else if ((watchdogCounter % 20) == 10) {
    digitalWrite(watchdogPin, LOW);
  }
  dimValue++;
  if (dimValue > 250) {
    dimValue = 20;
  }
  delay(50);
  watchdogCounter++;
}

void gearlight(int g) {
  int i;
  
  // Serial.println(g, DEC);
  
  for (i=1; i<7; i ++) {
    if (i==g) {
      digitalWrite(base + i - 1, HIGH);
    } else {
      digitalWrite(base + i - 1, LOW);
    }
  }
}

