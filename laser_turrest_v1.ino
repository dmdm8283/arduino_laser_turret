#include <Servo.h>

Servo servoX; 
Servo servoY; 

const int pinServoX = 9;
const int pinServoY = 10;
const int pinJoyX = A0;
const int pinJoyY = A1;
const int pinJoySW = A2;
const int pinLaser1 = 6;
const int pinLaser2 = 5;

const int deadband = 20;
const float smoothAlpha = 0.18;
const int minAngleX = 20;
const int maxAngleX = 160;
const int minAngleY = 20;
const int maxAngleY = 160;

int angleX = 90;
int angleY = 90;

bool lasersOn = false;
bool lastButtonState = HIGH; 

void setup() {
  servoX.attach(pinServoX);
  servoY.attach(pinServoY);
  servoX.write(angleX);
  servoY.write(angleY);

  pinMode(pinLaser1, OUTPUT);
  pinMode(pinLaser2, OUTPUT);
  digitalWrite(pinLaser1, LOW);
  digitalWrite(pinLaser2, LOW);

  pinMode(pinJoySW, INPUT_PULLUP);
}

int joyToAngle(int raw, int minA, int maxA) {
  int centered = raw - 512;
  if (abs(centered) <= deadband) return (minA + maxA) / 2;
  int angle = map(raw, 0, 1023, maxA, minA);
  return constrain(angle, minA, maxA);
}

void loop() {
  int rawX = analogRead(pinJoyX);
  int rawY = analogRead(pinJoyY);

  int targetX = joyToAngle(rawX, minAngleX, maxAngleX);
  int targetY = joyToAngle(rawY, minAngleY, maxAngleY);

  angleX = round(angleX * (1.0 - smoothAlpha) + targetX * smoothAlpha);
  angleY = round(angleY * (1.0 - smoothAlpha) + targetY * smoothAlpha);

  servoX.write(angleX);
  servoY.write(angleY);

  bool currentButtonState = digitalRead(pinJoySW);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    lasersOn = !lasersOn;
    digitalWrite(pinLaser1, lasersOn ? HIGH : LOW);
    digitalWrite(pinLaser2, lasersOn ? HIGH : LOW);
  }
  lastButtonState = currentButtonState;

  delay(20);
}
