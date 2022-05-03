/* sudo arduino-cli compile --upload -b arduino:avr:uno -p /dev/ttyUSB0 */

#include <Servo.h>
#include <SoftwareSerial.h>
#include <arduino-timer.h>

SoftwareSerial BTSensor(11, 10); // TX | RX
Servo Servo1;

char command = ' ';
const int MOTOR_SWITCH = 8;
const int RATE = 9600;
const int SERVO_PIN = 3;
const int RIGHT_BLINK = 4;

auto timer = timer_create_default();

void setup() {
  Serial.begin(RATE);
  BTSensor.begin(RATE);

  Servo1.attach(SERVO_PIN);

  pinMode(MOTOR_SWITCH, OUTPUT);
  pinMode(RIGHT_BLINK, OUTPUT);

  Serial.println("Ready to connect\nDefualt password is 1234 or 000");
}

bool blink(void *argument) {
  int pin = (int)argument;
  digitalWrite(pin, !digitalRead(pin));
  return true;
}

void loop() {
  timer.tick();
  delay(100);
  if (BTSensor.available()) {
    command = BTSensor.read();
    Serial.println(command);
  }

  switch (command) {
  case 'f':
    Serial.println("Forward!");
    digitalWrite(MOTOR_SWITCH, HIGH);
    Servo1.write(100);
    digitalWrite(RIGHT_BLINK, LOW);
    break;
  case 'd':
    Serial.println("Reversing!");
    digitalWrite(MOTOR_SWITCH, LOW);
    break;
  case 'l':
    Serial.println("Left!");
    Servo1.write(145);
    break;
  case 'r':
    Serial.println("Right!");
    Servo1.write(55);
    timer.every(1000, blink, (void *)RIGHT_BLINK);
    break;
  }

  command = ' ';
}
