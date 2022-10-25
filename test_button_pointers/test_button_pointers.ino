int btn_plus_pin = 4;
int btn_minu_pin = 3;
int value = 0;
bool pulsed_pl = false;
bool pulsed_mi = false;

void modify_value(int btn_pin, int increment, bool *pulsed) {
  if (*pulsed) {
    if (digitalRead(btn_pin) == HIGH) {
      *pulsed = false;
    }
  } else {
    if (digitalRead(btn_pin) == LOW) {
      value += increment;
      *pulsed = true;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(btn_plus_pin, INPUT_PULLUP);
  pinMode(btn_minu_pin, INPUT_PULLUP);
}
void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Value: ");
  Serial.println(value);
  modify_value(btn_plus_pin, 1, &pulsed_pl);
  modify_value(btn_minu_pin, -1, &pulsed_mi);
}