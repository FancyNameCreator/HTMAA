#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial BTSensor(11, 10); // TX | RX
Servo Servo1;

char command = ' ';
const int MOTOR_SWITCH = 8;
const int RATE = 9600;
const int servoPin = 3;

void setup()
{
  Serial.begin(RATE);
  BTSensor.begin(RATE);

  Servo1.attach(servoPin);
  
  pinMode(MOTOR_SWITCH, OUTPUT);
  
  Serial.println("Ready to connect\nDefualt password is 1234 or 000");
}
void loop()
{
  delay(100);
  if (BTSensor.available()) {
    command = BTSensor.read();
    Serial.println(command);
  }

  switch (command){
    case 'f':
      Serial.println("Forward!");
      digitalWrite(MOTOR_SWITCH, HIGH);
      Servo1.write(100);
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
      break;
    default:
      break;
  }

  command = ' ';
  //  if (command == 1)
  //  {
  //    digitalWrite(LED, HIGH);
  //    Serial.println("LED On");
  //  }
  //  else if (command == 0)
  //  {
  //    digitalWrite(LED, LOW);
  //    Serial.println("LED Off");
  //  }
}
