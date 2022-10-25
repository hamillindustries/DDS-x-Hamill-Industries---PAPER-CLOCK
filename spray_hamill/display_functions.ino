// Functions for segment display
// Copyright: Canye Oest 2022

int keep_text_time = 500;  //Time to display menu name

void blink_display(int times, int deltime) {
  //Blink the display
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
  for (int i = 0; i < times; i++) {
    display.setSegments(data);
    delay(deltime);
    display.setSegments(blank);
    delay(deltime);
  }
}

void printErr() {
  //Print Err in display
  uint8_t data[] = {
    SEG_A | SEG_F | SEG_G | SEG_E | SEG_D,  // E
    SEG_E | SEG_G,                          // r
    SEG_E | SEG_G,                          // r
    0x00
  };
  display.setSegments(data);
}


void printChng() {
  //Print CHnG in display
  uint8_t data[] = {
    SEG_A | SEG_D | SEG_E | SEG_F,                  // C
    SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,          // H
    SEG_A | SEG_B | SEG_C | SEG_F | SEG_E,          // n
    SEG_A | SEG_F | SEG_G | SEG_C | SEG_E | SEG_D,  // G
  };
  display.setSegments(data);
}

void printPaus() {
  //Print Paus in display
  uint8_t data[] = {
    SEG_E | SEG_F | SEG_A | SEG_B | SEG_G,          // P
    SEG_E | SEG_F | SEG_A | SEG_B | SEG_C | SEG_G,  // A
    SEG_E | SEG_F | SEG_D | SEG_C | SEG_B,          // U
    SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,          // S
  };
  display.setSegments(data);
}

void printBack() {
  //Print owo in display
  uint8_t data[] = {
    SEG_F | SEG_A | SEG_B | SEG_G,  // O
    SEG_E | SEG_D | SEG_C,          // w
    SEG_E | SEG_D | SEG_C,          // w
    SEG_F | SEG_A | SEG_B | SEG_G,  // O
  };
  display.setSegments(data);
}

void printHome() {
  //Print HOmE in display
  uint8_t data[] = {
    SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,          // H
    SEG_A | SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,  // O
    SEG_A | SEG_E | SEG_C,                          // m
    SEG_A | SEG_F | SEG_G | SEG_E | SEG_D           // E
  };
  display.setSegments(data);
}

void printSet() {
  //Print SEt in display
  uint8_t data[] = {
    SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,  // S
    SEG_A | SEG_F | SEG_G | SEG_E | SEG_D,  // E
    SEG_F | SEG_G | SEG_E | SEG_D,          // t
    0x00
  };
  display.setSegments(data);
  delay(keep_text_time);
}

void printTime() {
  //Print timE in display
  uint8_t data[] = {
    SEG_F | SEG_G | SEG_E | SEG_D,         // t
    SEG_F | SEG_E,                         // i
    SEG_A | SEG_E | SEG_C,                 // m
    SEG_A | SEG_F | SEG_G | SEG_E | SEG_D  // E
  };
  display.setSegments(data);
  delay(keep_text_time);
}

void printStart() {
  //Print StRt in display
  uint8_t data[] = {
    SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,  // S
    SEG_F | SEG_G | SEG_E | SEG_D,          // t
    SEG_A | SEG_F | SEG_B | SEG_E,          // R
    SEG_F | SEG_G | SEG_E | SEG_D,          // t
  };
  display.setSegments(data);
  delay(keep_text_time);
}

void printEnd() {
  //Print End in display
  uint8_t data[] = {
    SEG_A | SEG_F | SEG_G | SEG_E | SEG_D,  // E
    SEG_A | SEG_B | SEG_C | SEG_F | SEG_E,  // n
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,  // d
    0x00
  };
  display.setSegments(data);
  delay(keep_text_time);
}

void printGo() {
  //Print GO in display
  uint8_t data[] = {
    SEG_A | SEG_F | SEG_G | SEG_C | SEG_E | SEG_D,  // G
    SEG_A | SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,  // O
    0x00, 0x00
  };
  display.setSegments(data);
}

void printPlay() {
  //Print Play in display
  uint8_t data[] = {
    SEG_E | SEG_F | SEG_A | SEG_B | SEG_G,          // P
    SEG_D | SEG_E | SEG_F,                          // L
    SEG_E | SEG_F | SEG_A | SEG_B | SEG_C | SEG_G,  // A
    SEG_B | SEG_C | SEG_F | SEG_G                   // Y
  };
  display.setSegments(data);
}

void printStop() {
  //Print Stop in display
  uint8_t data[] = {
    SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,          // S
    SEG_F | SEG_G | SEG_E | SEG_D,                  // t
    SEG_A | SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,  // O
    SEG_E | SEG_F | SEG_A | SEG_B | SEG_G           // P
  };
  display.setSegments(data);
}

void printFin() {
  //Print Fin in display
  uint8_t data[] = {
    SEG_A | SEG_E | SEG_F | SEG_G,          // F
    SEG_F | SEG_E,                          // i
    SEG_A | SEG_B | SEG_C | SEG_F | SEG_E,  // n
    0x00
  };
  display.setSegments(data);
}

uint8_t snake[] = { SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F };
int snake_pos = 0;

void printSnake() {

  uint8_t data[] = {
    snake[snake_pos / 4], snake[(snake_pos / 4 + 1) % 6], snake[(snake_pos / 4 + 2) % 6], snake[(snake_pos / 4 + 3) % 6]
  };
  display.setSegments(data);
  snake_pos = (snake_pos + 1) % 24;
}

int value_displayed(int min, int max) {
  //Update value and display it
  if (starting_val + relative_btn_val < min) {
    relative_btn_val = min - starting_val;  //Lock relative button value within range TODO check this works or return to old
  } else {
    if (starting_val + relative_btn_val > max) {
      relative_btn_val = max - starting_val;  //Lock relative button value within range
    }
  }
  int encoder_value = starting_val + relative_btn_val;  //OLD constrain(starting_val + relative_btn_val, min, max);
  display.showNumberDec(encoder_value, false);

  return encoder_value;
}

void display_countdown(int seconds) {
  for (int i = seconds; i >= 0; i--) {
    display.showNumberDec(i, false);
    delay(1000);
  }
}