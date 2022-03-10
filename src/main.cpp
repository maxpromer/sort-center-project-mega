#include <Arduino.h>

#define SENSOR_E_PIN   4  // เซ็นเซอร์ภาคตะวันออก
#define SOLENOID_E_PIN 11 // ภาคตะวันออก

#define SENSOR_S_PIN   7  // เซ็นเซอร์ภาคใต้
#define SOLENOID_S_PIN 10 // ภาคใต้

#define SENSOR_C_PIN   5  // เซ็นเซอร์ภาคกลาง
#define SOLENOID_C_PIN 9  // ภาคกลาง

#define SENSOR_N_PIN   6  // เซ็นเซอร์ภาคเหนือ
#define SOLENOID_N_PIN 8  // ภาคเหนือ

#define MOTOR_PIN 12 

#define SENSOR_IS_FOUND(pin) (digitalRead(pin) == LOW)

#define SOLENOID_OPEN(pin) digitalWrite(pin, LOW)
#define SOLENOID_CLOSE(pin) digitalWrite(pin, HIGH)

#define MOTOR_ON() digitalWrite(MOTOR_PIN, LOW)
#define MOTOR_OFF() digitalWrite(MOTOR_PIN, HIGH)

#define DROP_BOX(pin) { \
                        SOLENOID_OPEN(pin); \
                        delay(500); \
                        SOLENOID_CLOSE(pin); \
                      }

void setup() {
  Serial.begin(9600);

  pinMode(SENSOR_E_PIN, INPUT_PULLUP);
  pinMode(SOLENOID_E_PIN, OUTPUT);
  pinMode(SENSOR_S_PIN, INPUT_PULLUP);
  pinMode(SOLENOID_S_PIN, OUTPUT);
  pinMode(SENSOR_C_PIN, INPUT_PULLUP);
  pinMode(SOLENOID_C_PIN, OUTPUT);
  pinMode(SENSOR_N_PIN, INPUT_PULLUP);
  pinMode(SOLENOID_N_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  SOLENOID_CLOSE(SOLENOID_E_PIN);
  SOLENOID_CLOSE(SOLENOID_S_PIN);
  SOLENOID_CLOSE(SOLENOID_C_PIN);
  SOLENOID_CLOSE(SOLENOID_N_PIN);

  MOTOR_ON();

  // SOLENOID_OPEN(SOLENOID_N_PIN);
}

extern int readBoxHight();

void loop() {
  while(Serial.available()) Serial.read();

  // New round
  Serial.println("Wait sensor found box (North)");
  while(!SENSOR_IS_FOUND(SENSOR_N_PIN)) delay(10);

  Serial.println("Found");
  uint32_t start = millis();
  while(SENSOR_IS_FOUND(SENSOR_N_PIN)) delay(10);
  uint16_t using_time = millis() - start;
  Serial.print("Using ");
  Serial.println(using_time);

  String boxSize = "";
  if ((using_time >= (1200 - 200)) && (using_time <= (1200 + 200))) { // กล่อง 00
    boxSize = "00";
  } else if ((using_time >= (1500 - 200)) && (using_time <= (1500 + 200))) { // กล่อง AA
    boxSize = "AA";
  } else if ((using_time >= (2150 - 200)) && (using_time <= (2150 + 200))) { // กล่อง B
    boxSize = "B";
  } else if ((using_time >= (2700 - 200)) && (using_time <= (2700 + 200))) { // กล่อง C
    boxSize = "C";
  }

  Serial.println("Found box size is " + boxSize);
  
  uint32_t start2 = millis();
  if (Serial.available() == 0) {
    Serial.println("Wait CAM send region");
    while(Serial.available() == 0) delay(10);
  }
  char region = Serial.read();
  Serial.print("Got region is ");
  Serial.println(region);

  uint16_t using_time2 = millis() - start2;
  if (boxSize == "00") {
    delay(1100 - using_time2);
  } else if (boxSize == "AA") {
    delay(800 - using_time2);
  } else if (boxSize == "B") {
    delay(500 - using_time2);
  } else if (boxSize == "C") {
    delay(300 - using_time2);
  }

  if (region == 'N') {
    Serial.println("Dropbox in N region");
    DROP_BOX(SOLENOID_N_PIN);
    return;
  }

  // Center
  Serial.println("Wait sensor found box (Center)");
  while(!SENSOR_IS_FOUND(SENSOR_C_PIN)) delay(10);
  Serial.println("Found");
  while(SENSOR_IS_FOUND(SENSOR_C_PIN)) delay(10);
  if (boxSize == "00") {
    delay(1100 - using_time2);
  } else if (boxSize == "AA") {
    delay(800 - using_time2);
  } else if (boxSize == "B") {
    delay(500 - using_time2);
  } else if (boxSize == "C") {
    delay(300 - using_time2);
  }

  if (region == 'C') {
    Serial.println("Dropbox in C region");
    DROP_BOX(SOLENOID_C_PIN);
    return;
  }



  Serial.println("Wait sensor found box (South)");
  while(!SENSOR_IS_FOUND(SENSOR_S_PIN)) delay(10);
  Serial.println("Found");
  while(SENSOR_IS_FOUND(SENSOR_S_PIN)) delay(10);
  if (boxSize == "00") {
    delay(1100 - using_time2);
  } else if (boxSize == "AA") {
    delay(800 - using_time2);
  } else if (boxSize == "B") {
    delay(500 - using_time2);
  } else if (boxSize == "C") {
    delay(300 - using_time2);
  }

  if (region == 'S') {
    Serial.println("Dropbox in S region");
    DROP_BOX(SOLENOID_S_PIN);
    return;
  }


  // Final
  Serial.println("Unknow, press somethings to continual");
  while(Serial.read() == -1) delay(100);
  return;
}
