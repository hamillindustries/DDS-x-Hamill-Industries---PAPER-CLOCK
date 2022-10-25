// Functions for stepper motor
// Copyright: Canye Oest 2022

long homing_stepper(AccelStepper stepper, int home_switch, int end_switch) {
  /* 
  HOMING_STEPPER is the function that allows steppers to move untill they reach their home-switch.
  Once they do, the position of the stepper is set to 0, in order to have absolute measurments of position.
  */
  long initial_homing;  //Variable for homing.
  long distance;

  //HOMING SEQUENCE
  //Stepper moves step by step untill reaching the home-switch position.
  stepper.setMaxSpeed(max_stepper_speed_X);         // Set Max Speed of Stepper (Slow to get better accuracy)
  stepper.setAcceleration(stepper_acceleration_X);  // Set Acceleration of Stepper
  stepper.setCurrentPosition(0);                    //Set an absolute value to measure distance

  while (digitalRead(home_switch)) {
    stepper.setSpeed(max_stepper_speed_X);  //Set speed to non-configuration
    stepper.runSpeed();                     //stepper moves at set speed
  }

  //When switch is reached it moves back untill released.
  distance = stepper.currentPosition();
  //Stepper moves step by step untill releases the switch (i.e. opposite direction).
  stepper.setCurrentPosition(0.0);  // Set the current position as zero for now
  stepper.setMaxSpeed(100.0);       // Set Max Speed of Stepper (Slow to get better accuracy)
  stepper.setAcceleration(100.0);   // Set Acceleration of Stepper
  initial_homing = -1;              //Resets value

  while (!digitalRead(home_switch)) {
    stepper.moveTo(initial_homing);
    stepper.run();
    initial_homing--;
    delay(5);  //Safety delay
  }

  //When switch is released, the position is set to 0
  stepper.setCurrentPosition(0.0);  // Set the current position as zero (home)

  return -distance;
}

long findRealEndPos(int end_switch) {
  // Find the real value of end position
  stepper.setMaxSpeed(max_stepper_speed_X);         // Set Max Speed of Stepper (Slow to get better accuracy)
  stepper.setAcceleration(stepper_acceleration_X);  // Set Acceleration of Stepper

  Serial.print("Max position before: ");
  Serial.println(maximum_position);

  while (digitalRead(end_switch) == HIGH) {
    stepper.setSpeed(-max_stepper_speed_X);  //Set speed to non-configuration
    stepper.runSpeed();                      //stepper moves at set speed
  }

  maximum_position = stepper.currentPosition() / relval;
  end_pos = starting_val = maximum_position;
  Serial.print("Max position after: ");
  Serial.println(maximum_position);
  real_end_pos = true;
}

long reverse_homing(AccelStepper stepper, int home_switch, int end_switch) {
  /* 
  REVERSE_HOMING allows to perform a position check with the end_switch instead of the home switch, to ease
  movements of the axis and reduce lost time.
  */
  long initial_homing;
  long distance;

  stepper.setMaxSpeed(max_stepper_speed_X);         // Set Max Speed of Stepper (Slow to get better accuracy)
  stepper.setAcceleration(stepper_acceleration_X);  // Set Acceleration of Stepper
  stepper.setCurrentPosition(0);                    //Set an absolute value to measure distance

  while (digitalRead(end_switch) == HIGH) {
    stepper.setSpeed(-max_stepper_speed_X);  //Set speed to non-configuration
    stepper.runSpeed();                      //stepper moves at set speed
  }

  distance = stepper.currentPosition();
  stepper.setCurrentPosition(maximum_position * relval);  // Set the current position as maximum for now
  stepper.setMaxSpeed(100.0);                             // Set Max Speed of Stepper (Slow to get better accuracy)
  stepper.setAcceleration(100.0);                         // Set Acceleration of Stepper
  initial_homing = maximum_position * relval;             //Resets value

  delay(10);  //Safety delay

  while (digitalRead(end_switch) == LOW) {
    stepper.moveTo(initial_homing);
    stepper.run();
    initial_homing++;
    delay(5);  //Safety delay
  }

  //When switch is released, the position is set to max
  stepper.overrideCurrentPosition(maximum_position * relval);  // Set the current position as maximum

  stepper.setMaxSpeed(max_stepper_speed_X);         // Reset Max Speed of Stepper (Slow to get better accuracy)
  stepper.setAcceleration(stepper_acceleration_X);  // Reset Acceleration of Stepper

  return distance;
}