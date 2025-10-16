// Anis sine variabler
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Martin sine variabler
const int unit = 100;
const int buzzer = 8;
volatile unsigned long pause = unit;
//Pause mellom ulike signaler kan endre seg, default er en unit. dette er hva som er mellom bokstaver

//Markus sine variabler
static const int buttonPin = 12;
int buttonStatePrevious = LOW;

unsigned long minButtonLongPressDuration = 500;
unsigned long buttonLongPressMillis;
bool buttonStateLongPress = false;

const int intervalButton = 50;
unsigned long previousButtonMillis;

unsigned long buttonPressDuration;

unsigned long currentMillis;
unsigned long lastReleaseMillis; 

const int letterPause = 2000;
const int readMessage = 10000;


String morse[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};

char letters[] = {'A','B','C','D','E','F','G','H','I',
  'J','K','L','M','N','O','P','Q','R',
  'S','T','U','V','W','X','Y','Z'};

void setup() {
  Serial.begin(9600);
  Serial.println("Skriv beskjeden pÃ¥ Morse: ");

  pinMode(buttonPin, INPUT);
  Serial.println("Press button");

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
//button string
String readButtonState(){
  static String buttonSignal = "";
  static bool pauseDetected = false; 
  static bool messageComplete = false;

  if(currentMillis - previousButtonMillis > intervalButton){
    int buttonState = digitalRead(buttonPin);

    if(buttonState == HIGH && buttonStatePrevious == LOW){
      buttonLongPressMillis = currentMillis;
      buttonStatePrevious = HIGH;
      pauseDetected = false;
      messageComplete = false;
    }

    
    if(buttonState == LOW && buttonStatePrevious == HIGH){
      buttonStatePrevious = LOW;
      buttonPressDuration = currentMillis - buttonLongPressMillis;
      lastReleaseMillis = currentMillis;

      if(buttonPressDuration >= minButtonLongPressDuration){
        Serial.println("Langt trykk (-)");
        buttonSignal += "-";
        //currentMorse += "-";
      }
      else{
      Serial.println("Kort trykk (.)");
      buttonSignal += ".";
      //currentMorse += ".";
      }
    }
    if (buttonState == LOW && (currentMillis - lastReleaseMillis) >= letterPause && !pauseDetected) {
    Serial.println("Mellomrom ( )");
    buttonSignal += " ";
    pauseDetected = true; 
  }

  if ((currentMillis - lastReleaseMillis) >= readMessage && !messageComplete) {
    Serial.print("Fullt signal: ");
    Serial.println(buttonSignal);
    return buttonSignal;
    buttonSignal = ""; 
    messageComplete = true;
  }
    previousButtonMillis = currentMillis;
  }
  return "";
}


