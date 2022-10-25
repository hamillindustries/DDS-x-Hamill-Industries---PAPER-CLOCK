#include <AccelStepper.h>

#define PUL 10                      //Stepper PUL
#define DIR 11                      //Stepper DIR
AccelStepper stepper(1, PUL, DIR);  //Definition of traveling stepper using pins 2 (STEP) and 3 (DIR).
int ini;

void setup() {
  Serial.begin(9600);
  stepper.setMaxSpeed(100.0);      //Reset max speed
  stepper.setAcceleration(100.0);  //Reset acceleration

  // put your setup code here, to run once:
  ini = stepper.currentPosition();
  Serial.println(ini);
}

void loop() {
  // put your main code here, to run repeatedly:

  stepper.setMaxSpeed(100.0);      //Reset max speed
  stepper.setAcceleration(100.0);  //Reset acceleration

  if (Serial.available()) {
    int mvto = Serial.parseInt();
    if (mvto != 0) {
      stepper.moveTo(mvto);
      Serial.println(mvto);
    }
  }
  stepper.run();
}