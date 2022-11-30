#include <NimBLEDevice.h>
#include <BleOneKeyboard.h>
#include <ESP32Servo.h>

BleOneKeyboard bleEsp("!Lock", "IcyHool", 100);
Servo servo;

const int ledPin = 2;
const int servoPin = 13;
int usPos = 2100;


void setup() {
  Serial.begin(921600);
  pinMode(ledPin, OUTPUT);

  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	servo.setPeriodHertz(50);
	servo.attach(servoPin, 500, 2500);

  bleEsp.begin();
}

void loop() {
  if(!bleEsp.isConnected()) { // LOCK
    delay(2000);
    if(!bleEsp.isConnected()) {
      if (servo.readMicroseconds() > 1480) {
        digitalWrite(ledPin, LOW);
        for (usPos = servo.readMicroseconds(); usPos >= 1480; usPos -= 7) {
          if(bleEsp.isConnected()) {
            break;
          }
          servo.writeMicroseconds(usPos);
          delay(10); //minimum at 5V is 3
        }
      }
    }
  }
  else { // UNLOCK
    if (servo.readMicroseconds() < 2170) {
      digitalWrite(ledPin, HIGH);
      for (usPos = servo.readMicroseconds(); usPos <= 2170; usPos += 7) {
        servo.writeMicroseconds(usPos);
        delay(10); //minimum at 5V is 3
      }
      digitalWrite(ledPin, LOW);
    }
  }
}
