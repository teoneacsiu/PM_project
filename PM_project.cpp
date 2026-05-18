#include <LiquidCrystal.h>
#include <Servo.h>

// LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
Servo servoMotor;

// Pini
const int pinServo = 9;
const int pinButonMod = 10;
const int pinButonStanga = 11;
const int pinButonDreapta = 12;

const int pinLdrStanga = A0;
const int pinLdrDreapta = A1;

// Stare sistem
bool modAuto = false;   // pornim in MANUAL
int unghiServo = 90;

// Reglaje
const int pragDiferenta = 100;   // il poti schimba: 20, 30, 40, 50
const int pasAuto = 1;          // cat misca in AUTO
const int pasManual = 2;        // cat misca in MANUAL
const int unghiMin = 20;
const int unghiMax = 160;

// Debounce simplu pentru buton MOD
bool ultimMod = HIGH;
unsigned long ultimTimpMod = 0;
const unsigned long debounceDelay = 200;

// Pentru actualizare LCD
unsigned long ultimUpdateLCD = 0;
const unsigned long intervalLCD = 250;

// Variabile senzori
int ldrStanga = 0;
int ldrDreapta = 0;
int diferenta = 0;

void afiseazaEcran() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Mod:");
  if (modAuto) {
    lcd.print("AUTO ");
  } else {
    lcd.print("MAN  ");
  }

  lcd.print(" U:");
  lcd.print(unghiServo);

  lcd.setCursor(0, 1);
  lcd.print("S:");
  lcd.print(ldrStanga);
  lcd.print(" D:");
  lcd.print(ldrDreapta);
}

void setup() {
  lcd.begin(16, 2);

  pinMode(pinButonMod, INPUT_PULLUP);
  pinMode(pinButonStanga, INPUT_PULLUP);
  pinMode(pinButonDreapta, INPUT_PULLUP);

  servoMotor.attach(pinServo);
  servoMotor.write(unghiServo);

  afiseazaEcran();
}

void loop() {
  // Citire butoane
  bool stareMod = digitalRead(pinButonMod);
  bool stareStanga = digitalRead(pinButonStanga);
  bool stareDreapta = digitalRead(pinButonDreapta);

  // Citire senzori
  ldrStanga = analogRead(pinLdrStanga);
  ldrDreapta = analogRead(pinLdrDreapta);
  diferenta = ldrStanga - ldrDreapta;

  // Buton MOD
  if (stareMod == LOW && ultimMod == HIGH && millis() - ultimTimpMod > debounceDelay) {
    modAuto = !modAuto;
    ultimTimpMod = millis();
    afiseazaEcran();
  }
  ultimMod = stareMod;

  // MOD MANUAL
  if (!modAuto) {
    if (stareStanga == LOW) {
      unghiServo -= pasManual;
      if (unghiServo < unghiMin) unghiServo = unghiMin;
      servoMotor.write(unghiServo);
      delay(40);
    }

    if (stareDreapta == LOW) {
      unghiServo += pasManual;
      if (unghiServo > unghiMax) unghiServo = unghiMax;
      servoMotor.write(unghiServo);
      delay(40);
    }
  }

  // MOD AUTO
  else {
    if (abs(diferenta) > pragDiferenta) {

      // Daca LDR stanga vede mai multa lumina
      if (diferenta > 0) {
        unghiServo -= pasAuto;
        if (unghiServo < unghiMin) unghiServo = unghiMin;
        servoMotor.write(unghiServo);
      }

      // Daca LDR dreapta vede mai multa lumina
      else {
        unghiServo += pasAuto;
        if (unghiServo > unghiMax) unghiServo = unghiMax;
        servoMotor.write(unghiServo);
      }

      delay(30);
    }
  }

  // Update LCD
  if (millis() - ultimUpdateLCD > intervalLCD) {
    afiseazaEcran();
    ultimUpdateLCD = millis();
  }
}
