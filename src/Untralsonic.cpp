#include "Arduino.h"

#define UNTRAL_ECHO_PIN A0
#define UNTRAL_TRIG_PIN A1

int readBoxHight() {
  pinMode(UNTRAL_TRIG_PIN, OUTPUT);
  pinMode(UNTRAL_ECHO_PIN, INPUT);

  digitalWrite(UNTRAL_TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(UNTRAL_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(UNTRAL_TRIG_PIN, LOW);
  unsigned int PulseWidth = pulseIn(UNTRAL_ECHO_PIN, HIGH);
  unsigned int distance = PulseWidth * 0.0173681;

  Serial.println(distance);

  return distance;
}

