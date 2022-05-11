/* sudo arduino-cli compile --upload -b arduino:avr:uno -p /dev/ttyUSB0  &&  sudo cat /dev/ttyUSB0 */

#include <Servo.h>
#include <SoftwareSerial.h>
#include <Ultrasonic.h>
#include <arduino-timer.h>

SoftwareSerial BTSensor(11, 10);  // TX | RX
Servo Servo1;
auto timer = timer_create_default();
Ultrasonic ultrasonic(3, 4);

char command = ' ';
const int MOTOR_SWITCH = 8;
const int RATE = 9600;
const int SERVO_PIN = 13;

const int TAIL_LIGHTS = 6;

const int LEFT_BLINK = 5;
const int RIGHT_BLINK = 7;

unsigned int LEFT_BLINK_TIMER = 0;
unsigned int RIGHT_BLINK_TIMER = 0;

void setup() {
  Serial.begin(RATE);
  BTSensor.begin(RATE);

  Servo1.attach(SERVO_PIN);

  pinMode(MOTOR_SWITCH, OUTPUT);
  pinMode(RIGHT_BLINK, OUTPUT);
  pinMode(LEFT_BLINK, OUTPUT);
  pinMode(TAIL_LIGHTS, OUTPUT);

  Serial.println("Ready to connect\nDefualt password is 1234 or 000");
}

bool blink(void *argument) {
  int pin = (int)argument;
  digitalWrite(pin, !digitalRead(pin));
  return true;
}

void resetTurnLights() {
  timer.cancel(RIGHT_BLINK_TIMER);
  timer.cancel(LEFT_BLINK_TIMER);
  digitalWrite(RIGHT_BLINK, LOW);
  digitalWrite(LEFT_BLINK, LOW);
}

void handleBluetooth() {
  if (BTSensor.available()) {
    command = BTSensor.read();
    Serial.println(command);
  }
}

void handleTailLights() {
  if (!digitalRead(MOTOR_SWITCH)) {
    digitalWrite(TAIL_LIGHTS, HIGH);
  } else {
    digitalWrite(TAIL_LIGHTS, LOW);
  }
}

void handleAutoBreak() {
  auto distance = ultrasonic.read();
  Serial.println(("distance " + String(distance)));
  if (distance <= 5) {
    digitalWrite(MOTOR_SWITCH, LOW);
  }
}

void handleCommands() {
  switch (command) {
    case 'f':
      digitalWrite(MOTOR_SWITCH, HIGH);
      Servo1.write(100);
      resetTurnLights();
      break;
    case 'd':
      digitalWrite(MOTOR_SWITCH, LOW);
      break;
    case 'l':
      Servo1.write(130);
      resetTurnLights();
      LEFT_BLINK_TIMER = timer.every(500, blink, (void *)LEFT_BLINK);
      break;
    case 'r':
      Servo1.write(83);
      resetTurnLights();
      RIGHT_BLINK_TIMER = timer.every(500, blink, (void *)RIGHT_BLINK);
      break;
  }
  command = ' ';
}

void loop() {
  delay(50);
  timer.tick();
  handleBluetooth();
  handleTailLights();
  handleCommands();
  handleAutoBreak();
}
