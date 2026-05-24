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

// Mod functionare
bool modAuto = false;   // pornim in MANUAL

// Valori pentru servo 360
const int STOP_SERVO = 90;
const int LEFT_SERVO = 80;
const int RIGHT_SERVO = 105;

// Reglaje
const int pragDiferenta = 100;
const unsigned long debounceDelay = 200;
const unsigned long intervalLCD = 250;

// Debounce buton MOD
bool ultimMod = HIGH;
unsigned long ultimTimpMod = 0;

// Variabile senzori
int ldrStanga = 0;
int ldrDreapta = 0;
int diferenta = 0;

// Unghi estimat
int unghiEstimat = 90;
const int unghiMin = 20;
const int unghiMax = 160;

// stare miscare: -1 = stanga, 0 = stop, 1 = dreapta
int stareMiscare = 0;

// cat de des actualizam unghiul estimat
unsigned long ultimUpdateUnghi = 0;
const unsigned long intervalUnghi = 80;

// Pentru LCD
unsigned long ultimUpdateLCD = 0;

void opresteServo() {
  servoMotor.write(STOP_SERVO);
  stareMiscare = 0;
}

void mergiStanga() {
  servoMotor.write(LEFT_SERVO);
  stareMiscare = -1;
}

void mergiDreapta() {
  servoMotor.write(RIGHT_SERVO);
  stareMiscare = 1;
}

void actualizeazaUnghiEstimat() {
  if (millis() - ultimUpdateUnghi >= intervalUnghi) {
    ultimUpdateUnghi = millis();

    if (stareMiscare == -1) {
      unghiEstimat--;
      if (unghiEstimat < unghiMin) unghiEstimat = unghiMin;
    }
    else if (stareMiscare == 1) {
      unghiEstimat++;
      if (unghiEstimat > unghiMax) unghiEstimat = unghiMax;
    }
  }
}

void afiseazaEcran() {
  lcd.setCursor(0, 0);
  lcd.print("Mod:");
  if (modAuto) {
    lcd.print("AUTO   ");
  } else {
    lcd.print("MANUAL ");
  }

  lcd.setCursor(11, 0);
  if (unghiEstimat < 100) lcd.print("U:0");
  else lcd.print("U:");
  lcd.print(unghiEstimat);

  lcd.setCursor(0, 1);
  lcd.print("S:");
  lcd.print(ldrStanga);
  lcd.print(" D:");
  lcd.print(ldrDreapta);
  lcd.print("   ");
}

void setup() {
  lcd.begin(16, 2);

  pinMode(pinButonMod, INPUT_PULLUP);
  pinMode(pinButonStanga, INPUT_PULLUP);
  pinMode(pinButonDreapta, INPUT_PULLUP);

  servoMotor.attach(pinServo);
  opresteServo();

  lcd.clear();
  afiseazaEcran();
}

void loop() {
  bool stareMod = digitalRead(pinButonMod);
  bool stareStanga = digitalRead(pinButonStanga);
  bool stareDreapta = digitalRead(pinButonDreapta);

  ldrStanga = analogRead(pinLdrStanga);
  ldrDreapta = analogRead(pinLdrDreapta);
  diferenta = ldrStanga - ldrDreapta;

  // schimbare mod
  if (stareMod == LOW && ultimMod == HIGH && millis() - ultimTimpMod > debounceDelay) {
    modAuto = !modAuto;
    ultimTimpMod = millis();
    opresteServo();
  }
  ultimMod = stareMod;

  // MANUAL
 if (!modAuto) {
  if (stareStanga == LOW && stareDreapta == HIGH) {
    if (unghiEstimat > unghiMin) {
      mergiStanga();
    } else {
      opresteServo();
    }
  }
  else if (stareDreapta == LOW && stareStanga == HIGH) {
    if (unghiEstimat < unghiMax) {
      mergiDreapta();
    } else {
      opresteServo();
    }
  }
  else {
    opresteServo();
  }
}

  // AUTO
  else {
  if (abs(diferenta) <= pragDiferenta) {
    opresteServo();
  }
  else if (diferenta > 0) {
    if (unghiEstimat > unghiMin) {
      mergiStanga();
    } else {
      opresteServo();
    }
  }
  else {
    if (unghiEstimat < unghiMax) {
      mergiDreapta();
    } else {
      opresteServo();
    }
  }
}

  actualizeazaUnghiEstimat();

  if (millis() - ultimUpdateLCD > intervalLCD) {
    afiseazaEcran();
    ultimUpdateLCD = millis();
  }

  delay(20);
}
