#define TRIG 9
#define ECHO 8

int leds[] = {A0, A1, A2, A3, A4, A5, A6, A7};
int numLeds = 8;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

void loop() {
  float distance = getDistance();
  Serial.println(distance);
  
  lightUpPath(distance);
  delay(150);
}

float getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  return duration * 0.0343 / 2; // convert to cm
}

void lightUpPath(float distance) {
  // Adjust according to your measurements
  float thresholds[] = {19, 16.5, 14.9, 12.5, 10.5, 8, 5.5, 2.5};
  int ledsOn = 0;

  for (int i = 0; i < numLeds; i++) {
    if (distance <= thresholds[i]) ledsOn = i + 1;
  }

  // Turn on LEDs up to that point
  for (int i = 0; i < numLeds; i++) {
    if (i < ledsOn)
      digitalWrite(leds[i], HIGH);
    else
      digitalWrite(leds[i], LOW);
  }
}
