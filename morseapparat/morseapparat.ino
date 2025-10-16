#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int unit = 100;
const int buzzer = 8;

//Pause mellom ulike signaler kan endre seg, default er en unit. dette er hva som er mellom bokstaver
volatile unsigned long pause = unit;

LiquidCrystal_I2C lcd(0x27, 16, 2);

String morse[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};

char letters[] = {'A','B','C','D','E','F','G','H','I',
  'J','K','L','M','N','O','P','Q','R',
  'S','T','U','V','W','X','Y','Z'};

void setup() {
  Serial.begin(9600);
  Serial.println("Skriv beskjeden pÃ¥ Morse: ");

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Morse Ready");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    String decoded = decoMessage(input);

    Serial.print("Mottatt: ");
    Serial.println(input);
    Serial.print("Oversatt: ");
    Serial.println(decoded);
    Serial.println();

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Morse:");
    lcd.setCursor(0,1);

    if (decoded.length() > 16) {
      lcd.print(decoded.substring(0,16));
    } else {
      lcd.print(decoded);
    }
  }
}

char decoMorse(String code) {
  for (int i = 0; i < 26; i++) {
    if (code == morse[i]) {
      return letters[i];
    }
  }
  return '?'; 
}

String decoMessage(String message) {
  String result = "";
  String token = "";

  message += ' ';

  for (unsigned int i = 0; i < message.length(); i++) {
    char c = message[i];

    if (c == ' ') {
      if (token.length() > 0) {
        result += decoMorse(token);
        token = "";
      }
    } 
    else if (c == '/') {
      if (token.length() > 0) {
        result += decoMorse(token);
        token = "";
      }
      result += ' ';
    } 
    else {
      token += c;
    }
  }
  return result;
}


