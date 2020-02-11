#include <math.h>

#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define BUTTON3_PIN 4
#define RELAY1_PIN 5
#define RELAY2_PIN 6
#define LOOP_INTERVAL 10
#define CONTACT_BOUNCE_DELAY 10
#define COMMAND_TIMEOUT 200
#define MAX_BUTTON_VALUE 10000

unsigned long previousMillis = 0;
boolean relay1 = false;
boolean relay2 = false;
signed int button1;
signed int button2;
signed int button3;
signed int lastButton1;
signed int lastButton2;
signed int lastButton3;

int pinToButtonValue(int pin) {
  return digitalRead(pin) ? -1 : 1;
}

boolean sameSign(int a, int b) {
  return (a > 0 ) == (b > 0);
}

int calculateNewButtonValue(int buttonValue, int newButtonValue) {
  if (sameSign(buttonValue, newButtonValue)) {
    return int(copysign(min(abs(buttonValue) + 1, MAX_BUTTON_VALUE), buttonValue));
  } else {
    return -(int(copysign(1, buttonValue)));
  }
}


void setup() {
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, relay1);
  digitalWrite(RELAY2_PIN, relay2);

  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);
  button1 = copysign(MAX_BUTTON_VALUE, pinToButtonValue(BUTTON1_PIN));
  button2 = copysign(MAX_BUTTON_VALUE, pinToButtonValue(BUTTON2_PIN));
  button3 = copysign(MAX_BUTTON_VALUE, pinToButtonValue(BUTTON3_PIN));
  lastButton1 = button1;
  lastButton2 = button2;
  lastButton3 = button3;
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= LOOP_INTERVAL) {
    previousMillis = currentMillis;

    int newButton1 = calculateNewButtonValue(button1, pinToButtonValue(BUTTON1_PIN));
    if (abs(newButton1) == 1) {
      lastButton1 = button1;
    }
    button1 = newButton1;
    int newButton2 = calculateNewButtonValue(button2, pinToButtonValue(BUTTON2_PIN));
    if (abs(newButton2) == 1) {
      lastButton2 = button2;
    }
    button2 = newButton2;
    int newButton3 = calculateNewButtonValue(button3, pinToButtonValue(BUTTON3_PIN));
    if (abs(newButton3) == 1) {
      lastButton3 = button3;
    }
    button3 = newButton3;

    if (
      (relay1 != relay2) &&
      (sameSign(button1, button2)) &&
      (
        abs(button1) == CONTACT_BOUNCE_DELAY && abs(button2) <= CONTACT_BOUNCE_DELAY ||
        abs(button2) == CONTACT_BOUNCE_DELAY && abs(button1) <= CONTACT_BOUNCE_DELAY
      )
    ) {
        relay1 = false;
        relay2 = false;
    } else if (abs(button1) == CONTACT_BOUNCE_DELAY) {
      relay1 = !relay1;
    } else if (abs(button2) == CONTACT_BOUNCE_DELAY) {
      relay2 = !relay2;
    } else if (abs(button3) == CONTACT_BOUNCE_DELAY) {
      if (
        (relay1 || relay2) &&
        (abs(lastButton3) > COMMAND_TIMEOUT)
      ) {
        relay1 = false;
        relay2 = false;
      } else if (!relay1 && !relay2) {
        relay1 = true;
        relay2 = true;
      } else if (relay1 && relay2) {
        relay1 = true;
        relay2 = false;
      } else if (relay1 && !relay2) {
        relay1 = false;
        relay2 = true;
      } else if (!relay1 && relay2) {
        relay1 = true;
        relay2 = true;
      }
    }
    digitalWrite(RELAY1_PIN, relay1);
    digitalWrite(RELAY2_PIN, relay2);
  }
}
