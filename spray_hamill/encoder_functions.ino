// Functions for what used to be an encoder.
// Copyright: Canye Oest 2022

void modify_value(int btn_pin, int increment, bool *pulsed) {
  if (digitalRead(btn_pin) == LOW) {
    relative_btn_val += increment;
    delay(50);
  }
}

void setup_minus_btn_read() {
  switch (general_mode) {
    case 0:
      //If in setup, act as a minus button
      modify_value(SETUP_MINUS_BTN_PIN, -INCREMENT, &pulsed_minus);
      break;
    case 1:
    case 4:
      if (digitalRead(SETUP_MINUS_BTN_PIN) == LOW) {
        //If in pause (include moving, just in case code is modified) -> go to setup
        if (!started) {
          //First run -> homing
          printHome();
          last_position = homing_stepper(stepper, PLSL, PLSR);
        }
        general_mode = 0;
        printSet();
        setup_menu_page = 0;     //Set to first page
        relative_btn_val = 0;    //Reset relative button
        starting_val = minutes;  //Set starting val to minutes
        printTime();
        break;
      }
  }
}

void change_plus_btn_read() {
  switch (general_mode) {
    case 0:
      //If in setup, act as a plus button
      modify_value(CHANGE_PLUS_BTN_PIN, +INCREMENT, &pulsed_plus);
      break;
    case 1:
    case 4:
      //If in pause (include moving, just in case code is modified) -> change spray
      if (digitalRead(CHANGE_PLUS_BTN_PIN) == LOW) {
        general_mode = 2;
        break;
      }
  }
}

void pressHandler(BfButton *btn, BfButton::press_pattern_t pattern) {
  //Handles main button events
  switch (general_mode) {
    case 0:
      // In setup mode, main button skips page untill long press, that returns to GO.
      switch (pattern) {
        case BfButton::SINGLE_PRESS:
        case BfButton::DOUBLE_PRESS:
          changeSetupPage();
          break;
        case BfButton::LONG_PRESS:
          save_all_memory();
          started = true;
          general_mode = 1;
          break;
      }
      break;
    //Nothing during move or change, since stepper blocks any input.
    case 3:
      //If pressed during change, print back then resume (by not adding break)
      printBack();
    case 4:
      //Resume if pressed during pause or change
      general_mode = 1;
      break;
  }
}

void changeSetupPage() {
  switch (setup_menu_page) {
    case 0:
      //Move from time to start
      printStart();
      setup_menu_page = 1;
      relative_btn_val = 0;  //Reset encoder
      starting_val = initial_pos;
      break;
    case 1:
      //Move from start to end
      printEnd();
      setup_menu_page = 2;
      relative_btn_val = 0;  //Reset encoder
      starting_val = end_pos;
      break;
    case 2:
      //Move from end to time
      printTime();
      setup_menu_page = 0;
      relative_btn_val = 0;  //Reset encoder
      starting_val = minutes;
      break;
  }
}