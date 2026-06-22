#include <Servo.h>

Servo miServo;

const int pinServo = 9;

int pos = 90;

void setupServo() {
  miServo.attach(pinServo);
  
  miServo.write(pos);
}

void loopServo() {
  for(int i = 0; i <= 180; i+=10) {

    miServo.write(i);

    delay(50);
  }
  for(int i = 180; i >= 0; i-=10) {

    miServo.write(i);

    delay(50);
  }
}