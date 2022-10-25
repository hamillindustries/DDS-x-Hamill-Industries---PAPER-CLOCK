// Spray machine code 4 Hamill Industries
// Copyright: Canye Oest 2022

/*

Horas dedicadas:
 - Miércoles 5: 14:00 a 18:00 + 21:30 a 23:30 = 6h
 - Sábado 8: 12:30 a 13:30 + 14:30 a 17:00 = 4h30
 - Domingo 9: 15:30 a 21:00 = 5h30
 - Lunes 10: 7:00 a 9:30 + 10:30 a 11:30 + 12:00 a 17:30 + 21:00 a 21:30 = 9h30
 - Martes 11: 10:30 a 14:00 = 4h
 - Miércoles 12: 11:15 a 17:15 = 6h
 -- TOTAL : 31h30

*/

#include <Arduino.h>        //Basic Arduino library
#include <EEPROM.h>         //Library for non-volatile memory.
#include <TM1637Display.h>  //Library for segment display
#include <AccelStepper.h>   //Library for steppers
#include <BfButton.h>       //Library for button pattern detection

// EEPROM MAP (to be used or not)
// | int maximum_position (stored during setup or negative homing)  @ 0
// | int minutes (stored during setup)                              @ sizeof(maximum_position)

// BASIC VALUES
#define ENABLE_MOTOR true  //To be deprecated, for debug purposes
#define INCREMENT 10       //Increment for buttons

int maximum_position = 2400;  //Maximimum position of rail; relative to home position set in stepper (minimum position is not in a variable since it's always 0)
int relval = -10;             //Relative value from display to actual steps.
int initial_pos = 0;          //Stepper initial position (not necessary to be home position 0)
int end_pos = 0;              //Stepper ending position (not necessary to be end of rail position)
float minutes = 6000;         //Time it takes in minutes (for display) - default: 10 hours
float full_rotation_seconds;  //Time to perform its full movement, for instance 10 hours is 10*60*60=36000
float stepper_speed;          //This will be equal to (final_position-homing_position)/full_rotation_seconds
int last_position = 0;        //Backup position for spray change
bool started = false;         //Flag for first setup
bool real_end_pos = false;    //Flag for correct real_end_pos finding

// STEPPER
#define PLSL 0                             //Left limit button pin
#define PLSR 2                             //Right limit button pin
#define PUL 10                             //Stepper PUL pin
#define DIR 11                             //Stepper DIR pin
#define max_stepper_speed_X 2000.0         //Max speed of stepper
#define stepper_acceleration_X 500.0       //Acceleration of stepper
#define stepper_max_acceleration_X 2000.0  //Acceleration of max stepper

//DISPLAY
#define DCLK 9                      //CLK pin of display
#define DDIO 6                      //DDIO pin of display
TM1637Display display(DCLK, DDIO);  //Define display
AccelStepper stepper(1, PUL, DIR);  //Definition of traveling stepper using pins 2 (STEP) and 3 (DIR).

//BUTTONS
#define SETUP_MINUS_BTN_PIN 3  //Minus button pin
#define CHANGE_PLUS_BTN_PIN 4  //Plus button pin
#define MAIN_BTN_PIN 5         //Main button pin
BfButton main_btn(BfButton::STANDALONE_DIGITAL, MAIN_BTN_PIN, true, LOW);
bool pulsed_minus = false;   //Pressed state for minus button
bool pulsed_plus = false;    //Pressed state for plus button
int relative_btn_val = 0;    //Relative counter using buttons
int starting_val = minutes;  //Value to add to relative counter

// MODES
int general_mode = 4;     //0 is setup, 1 is go, 2 is spray change, 3 is stop, 4 is pause
int setup_menu_page = 0;  //0 is time, 1 is start position, 2 is end position

void setup() {
  // Start display
  display.setBrightness(0x0f);
  Serial.begin(9600);

  // Read memory
  read_all_memory();  //Not sure if using this is a good idea... but it's coded ;)

  // User buttons sides & modifiers
  pinMode(PLSL, INPUT_PULLUP);
  pinMode(PLSR, INPUT_PULLUP);
  pinMode(SETUP_MINUS_BTN_PIN, INPUT_PULLUP);
  pinMode(CHANGE_PLUS_BTN_PIN, INPUT_PULLUP);

  // Control button settings settings
  main_btn.onPress(pressHandler)
    .onDoublePress(pressHandler)      // default timeout
    .onPressFor(pressHandler, 1000);  // custom timeout for 1 second

  //Start in pause mode after blink (stepper stays in position and shall not be touched)
  blink_display(3, 100);
  printPaus();

  //Stepper acceleration and speed setup
  stepper.setMaxSpeed(max_stepper_speed_X);         //Set max speed
  stepper.setAcceleration(stepper_acceleration_X);  //Set acceleration

  //Compute default speed, from home to end in desired seconds
  end_pos = maximum_position;
  full_rotation_seconds = minutes * 60.0;
  stepper_speed = (end_pos - 0) * relval / full_rotation_seconds;
}

void setup_menu() {
  // Separate actions for menu mode
  stepper.setMaxSpeed(max_stepper_speed_X);             //Set max speed
  stepper.setAcceleration(stepper_max_acceleration_X);  //Set acceleration
  switch (setup_menu_page) {
    case 0:
      //Minutes of execution
      minutes = value_displayed(0, 9999);  //Use buttons to update and display value of minutes between 0 and 9999
      full_rotation_seconds = minutes * 60.0;
      break;
    case 1:
      //Initial position
      initial_pos = value_displayed(0, maximum_position);  //Use buttons to update and display value of initial position between 0 and maximum position
      if (ENABLE_MOTOR) stepper.runToNewPosition(initial_pos * relval);
      break;
    case 2:
      //End position
      if (!real_end_pos) {
        findRealEndPos(PLSR);
      }
      end_pos = value_displayed(initial_pos, maximum_position);  //Use buttons to update and display value of end position between initial_position and maximum position
      if (ENABLE_MOTOR) stepper.runToNewPosition(end_pos * relval);
      break;
  }
  if (!started) {
    last_position = initial_pos * relval;
  }

  stepper_speed = (end_pos - initial_pos) * relval / full_rotation_seconds;  //This implies that position in axis is left and that it's at the beginning of the function.
  /* ^^^^ Comment explained ^^^^
    If minutes are changed, the time it will actually take to reach the end will be (end - current_position) / stepper_speed;
    If we actually want it to be time till end, it should be stepper_speed = (end - current_position) / full_rotation_secods
  */
}

void loop() {
  switch (general_mode) {
    case 0:                    //SETUP
      main_btn.read();         //Constant button update
      setup_minus_btn_read();  //Constant button update
      change_plus_btn_read();  //Constant button update
      setup_menu();
      break;
    case 1:  //PLAY
      if (ENABLE_MOTOR) {
        if (!started) {
          //First run after interrupt or without setup -> homing
          printHome();
          last_position = homing_stepper(stepper, PLSL, PLSR);
          started = true;
        }
        //Go to last position (might be 0 if first run; can be any other if post-interruption) at full speed
        printBack();
        stepper.setSpeed(max_stepper_speed_X);  //TODO check this doesn't mess up
        stepper.setMaxSpeed(max_stepper_speed_X);
        stepper.setAcceleration(stepper_max_acceleration_X);
        stepper.runToNewPosition(last_position);

        //Start moving at computed speed.
        printPlay();
        stepper.setSpeed(stepper_speed);  //Set speed to non-configuration
        stepper.setMaxSpeed(stepper_speed);
        stepper.runToNewPosition(end_pos * relval);

        //Once runToNew Position has finished, go back to pause mode.
        printFin();
        delay(2000);
        printPaus();
        general_mode = 4;
      }
      //Servos that are connected to sprays are controlled outside the code by using sparkfun's servo trigger in SJ1-Mode.
      break;
    case 2:  //SPRAY CHANGE
      //Reverse homing
      printHome();
      last_position = (maximum_position * relval) - reverse_homing(stepper, PLSL, PLSR);
      started = true;  //Change already performs a relative homing, so no need to redo homing
      stepper.overrideCurrentPosition(maximum_position * relval);
      printChng();
      stepper.setMaxSpeed(max_stepper_speed_X);
      stepper.setAcceleration(stepper_max_acceleration_X);
      general_mode = 3;  //Once moved, go to wait mode
      break;
    case 3:  //POST-CHANGE WAIT
      //Wait for user input
      main_btn.read();  //Constant button update
      break;
    case 4:  //
      //Wait for user input
      main_btn.read();         //Constant button update
      setup_minus_btn_read();  //Constant button update
      change_plus_btn_read();  //Constant button update
      break;
  }
}