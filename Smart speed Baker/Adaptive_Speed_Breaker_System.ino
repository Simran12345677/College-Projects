#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the I2C LCD with the default address (usually 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;  // create servo object

// Speed detection pins (IR sensors)
#define SENSOR_1_PIN 2  // Pin for first IR sensor (INT0)
#define SENSOR_2_PIN 3  // Pin for second IR sensor (INT1)

// Ultrasonic sensor pins (reassigned to avoid conflicts with IR sensors)
const int TRIG = 4;
const int ECHO = 5;

// LED pins (reassigned to digital pins to avoid I2C conflict on A4/A5)
const int leds[] = {6, 7, 8, 10, 11, 12};
const int numLeds = 6;        // Number of LEDs (must match leds[] size)
const int thresholds[numLeds] = {22, 19, 14, 11, 9, 7};  // Thresholds in cm (descending order)

// Speed detection variables
volatile unsigned long timeSensor1 = 0;
volatile unsigned long timeSensor2 = 0;
volatile bool sensor1Triggered = false;
volatile bool sensor2Triggered = false;
const float distanceBetweenSensors = 1.0;  // Distance between sensors in meters 
float speed = 0;  // Speed in meters per second (m/s)

// Ultrasonic variables
const int numReadings = 3;    // Number of readings to average for stability
const int loopDelay = 150;   // Delay between measurements in ms

// For non-blocking servo actions
unsigned long lastServoAction = 0;
int servoState = 90;  // 0: idle, 1: moving to position, 2: holding

void setup() {
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  
  myServo.attach(9);
  myServo.write(90);  // Start at neutral
  
  pinMode(SENSOR_1_PIN, INPUT);
  pinMode(SENSOR_2_PIN, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(SENSOR_1_PIN), sensor1ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(SENSOR_2_PIN), sensor2ISR, RISING);
  
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

void loop() {
  // Timeout Catch: If car passes sensor 1 but never hits sensor 2
  if (sensor1Triggered && !sensor2Triggered && (micros() - timeSensor1 > 5000000)) { 
    sensor1Triggered = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ready...");
  }

  // Handle speed detection (event-driven via interrupts)
  if (sensor1Triggered && sensor2Triggered && timeSensor1 < timeSensor2) {
    sensor1Triggered = false;
    sensor2Triggered = false;
    
    unsigned long timeDifference = timeSensor2 - timeSensor1;  // Time in microseconds
    lcd.clear();
    
    if (timeDifference > 0) {
      speed = distanceBetweenSensors / (timeDifference / 1000000.0);  // Convert µs to seconds
      float speedKmh = speed * 3.6;  // Convert m/s to km/h
      
      if (speedKmh < 5) {
        lcd.setCursor(0, 0);
        lcd.print("Speed Low");
        startServoAction(90);  // Rotate to 0 degrees
      } else if (speedKmh >= 5 && speedKmh < 8) {
        lcd.setCursor(0, 0);
        lcd.print("Under Limit, Good");
        startServoAction(130);  // Rotate to 90 degrees
      } else if (speedKmh >= 8 && speedKmh < 14) {
        lcd.setCursor(0, 0);
        lcd.print("Speed High, Slow");
        startServoAction(180);  // Rotate to 45 degrees (caution)
      } else {  // speedKmh >= 50
        lcd.setCursor(0, 0);
        lcd.print("Over Limit, Danger");
        startServoAction(180);  // Rotate to 180 degrees (alert)
      }
      
      lcd.setCursor(0, 1);
      lcd.print(speedKmh, 2);
      lcd.print(" km/h");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Invalid Reading!");
    }
  }
  
  // Handle ultrasonic distance measurement and LEDs (polled)
  float distance = getDistance();
  lightUpPath(distance);
  updateServo();
  delay(loopDelay);  
}

float getDistance() {
  float total = 0;
  int validReadings = 0;
  
  for (int i = 0; i < numReadings; i++) {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    
    unsigned long duration = pulseIn(ECHO, HIGH, 30000UL);
    if (duration > 0) {
      float dist = duration * 0.0343 / 2.0;  
      total += dist;
      validReadings++;
    }
    delay(10);  
  }
  
  if (validReadings == 0) {
    return 999.0;  
  }
  return total / validReadings;  
}

void lightUpPath(float distance) {
  int ledsOn = 0;
  
  for (int i = 0; i < numLeds; i++) {
    if (distance <= thresholds[i]) {
      ledsOn = i + 1;
    }
  }
  
  for (int i = 0; i < numLeds; i++) {
    if (i < ledsOn) {
      digitalWrite(leds[i], HIGH);
    } else {
      digitalWrite(leds[i], LOW);
    }
  }
}

void startServoAction(int angle) {
  myServo.write(angle);
  lastServoAction = millis();
  servoState = 1;  
}

void updateServo() {
  if (servoState == 1 && millis() - lastServoAction > 1000) {  
    servoState = 2;  
  }
  if (servoState == 2 && millis() - lastServoAction > 2000) {  
    myServo.write(90);  
    servoState = 90;  
  }
}

void sensor1ISR() {
  static unsigned long lastTrigger = 0;
  unsigned long now = micros();
  if (now - lastTrigger > 10000) {  
    if (!sensor1Triggered) {
      timeSensor1 = now;
      sensor1Triggered = true;
    }
    lastTrigger = now;
  }
}

void sensor2ISR() {
  static unsigned long lastTrigger = 0;
  unsigned long now = micros();
  if (now - lastTrigger > 10000) {  
    if (!sensor2Triggered) {
      timeSensor2 = now;
      sensor2Triggered = true;
    }
    lastTrigger = now;
  }
}
