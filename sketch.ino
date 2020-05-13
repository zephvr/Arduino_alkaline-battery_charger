#include <TimerOne.h>

#define STOPPED 0
#define RUNNING 1

const unsigned int duty_cycle = 983; // sur 255
const int powerPin = 10;
const int stopLedPin = 8;
const int switchPin = 2;
const int voltPin = A1;
const int tempPin = A0;
const unsigned long periode = 125000;

float voltMaxThreshold = 1.70;
float voltMinThreshold = 1.20;
float tempThreshold = 40.0;

float voltBattery = 1.5;
float tempBattery = 20.0;
int switchState = 0;
int state = STOPPED;
unsigned long time;

void start_power() {
  state = RUNNING;
  Timer1.pwm(powerPin, duty_cycle);
  digitalWrite(stopLedPin, LOW);
}

void stop_power() {
  state = STOPPED;
  Timer1.pwm(powerPin, 0);
  digitalWrite(stopLedPin, HIGH);
}

void setup()
{
  Serial.begin(9600);
  pinMode(powerPin, OUTPUT);
  pinMode(stopLedPin, OUTPUT);
  pinMode(switchPin, INPUT);
  Timer1.initialize(periode);
  digitalWrite(stopLedPin, HIGH);
}

void loop()
{
  voltBattery = analogRead(voltPin) / 1024.0 * 5.0;
  tempBattery = analogRead(tempPin) / 1024.0 * 5.0;
  tempBattery = (tempBattery - 0.5) * 100;
  switchState = digitalRead(switchPin);
  time = millis();

  if (voltBattery > voltMaxThreshold || voltBattery < voltMinThreshold || tempBattery > tempThreshold) {
    stop_power();
  }

  if (switchState == HIGH) {
    if (state == STOPPED) {
      start_power();
    } else {
      stop_power();
    }
  }
  if (state == STOPPED) {
    Serial.print(STOPPED);
  } else {
    Serial.print(RUNNING);
  }
  Serial.print("; ");
  Serial.print(time);
  Serial.print("; ");
  Serial.print(voltBattery);
  Serial.print("; ");
  Serial.print(tempBattery);
  Serial.println();
  delay(250);
}

