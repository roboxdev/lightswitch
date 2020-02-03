#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define BUTTON3_PIN 4
#define RELAY1_PIN 5
#define RELAY2_PIN 6
#define CONTACT_BOUNCE_DELAY 150
#define COMMAND_TIMEOUT 2000

boolean button_1;
boolean button_2;
boolean button_3;
boolean relay_1 = false;
boolean relay_2 = false;
unsigned long b1_last_press;
unsigned long b2_last_press;
unsigned long b3_last_press;

void setup() {
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  button_1 = !digitalRead(BUTTON1_PIN);
  button_2 = !digitalRead(BUTTON2_PIN);
  button_3 = !digitalRead(BUTTON3_PIN);
  digitalWrite(RELAY1_PIN, relay_1);
  digitalWrite(RELAY2_PIN, relay_2);
}

void loop() {
  boolean new_button_1 = !digitalRead(BUTTON1_PIN);
  boolean new_button_2 = !digitalRead(BUTTON2_PIN);
  boolean new_button_3 = !digitalRead(BUTTON3_PIN);

  if (
    (
      new_button_1 != button_1 ||
      new_button_2 != button_2 ||
      new_button_3 != button_3
    ) && (
      (millis() - max(max(b1_last_press, b2_last_press), b3_last_press)) > CONTACT_BOUNCE_DELAY
    )
  ) {
    if (new_button_1 != button_1 || new_button_2 != button_2) {
      if ((relay_1 != relay_2) && ()) {
        // горит только один, нажаты обе кнопки
      } else {
        if (new_button_1 != button_1) {
          relay_1 = !relay_1;
          button_1 = new_button_1;
          b1_last_press = millis();
        } else if (new_button_2 != button_2) {
          relay_2 = !relay_2;
          button_2 = new_button_2;
          b2_last_press = millis();
        }
      }
    } else if (new_button_3 != button_3) {
      if ((relay_1 || relay_2) && millis() - b3_last_press > COMMAND_TIMEOUT) {
        relay_1 = false;
        relay_2 = false;
      } else {
        if (!relay_1 && !relay_2) {
          relay_1 = true;
          relay_2 = true;
        } else if (relay_1 && relay_2) {
          relay_1 = true;
          relay_2 = false;
        } else if (relay_1 && !relay_2) {
          relay_1 = false;
          relay_2 = true;
        } else if (!relay_1 && relay_2) {
          relay_1 = true;
          relay_2 = true;
        }
      }
      button_3 = new_button_3;
      b3_last_press = millis();
    }

    digitalWrite(RELAY1_PIN, relay_1);
    digitalWrite(RELAY2_PIN, relay_2);
  }
}
