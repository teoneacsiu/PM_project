# PM_project

# Solar Tracker cu Arduino Uno

## Descriere

Acest proiect implementeaza un sistem de tip **solar tracker pe o axa**, folosind un **Arduino Uno**, doi senzori de lumina de tip **LDR**, un **servomotor**, un **LCD 16x2** si trei **butoane** pentru control manual si schimbarea modului de functionare.

Sistemul poate functiona in doua moduri:

- **MANUAL** - utilizatorul controleaza direct miscarea servomotorului cu ajutorul butoanelor
- **AUTO** - sistemul compara valorile celor doi senzori LDR si orienteaza servomotorul spre partea mai luminata

Pe LCD sunt afisate:
- modul curent de functionare
- unghiul curent al servomotorului
- valorile celor doi senzori de lumina

---

## Functionalitati

- afisare pe LCD 16x2
- citire a doi senzori LDR pe intrari analogice
- control manual al servo-ului din butoane
- mod automat de urmarire a sursei de lumina
- comutare intre modurile MANUAL si AUTO
- limitare software a unghiului servo-ului

---

## Componente utilizate

- Arduino Uno
- LCD 16x2 compatibil cu biblioteca `LiquidCrystal`
- 2 x LDR
- 2 x rezistente de 10k ohm
- 1 x servomotor
- 3 x butoane
- breadboard
- fire de conexiune

---

## Conexiuni hardware

### LCD
LCD-ul este conectat in modul 4-bit:

- RS -> D2
- E -> D3
- D4 -> D4
- D5 -> D5
- D6 -> D6
- D7 -> D7

### Servomotor
- semnal servo -> D9

### Butoane
- buton schimbare mod -> D10
- buton stanga -> D11
- buton dreapta -> D12

Butoanele sunt configurate cu `INPUT_PULLUP`, deci sunt conectate intre pin si GND.

### Senzori LDR
- LDR stanga -> A0
- LDR dreapta -> A1

Fiecare LDR este folosit intr-un divizor de tensiune impreuna cu o rezistenta de 10k ohm.

---

## Modul de functionare

### Modul MANUAL
La pornire, sistemul intra in modul **MANUAL**.

In acest mod:
- butonul din stanga deplaseaza servo-ul spre stanga
- butonul din dreapta deplaseaza servo-ul spre dreapta
- unghiul servo-ului este limitat intre `unghiMin` si `unghiMax`

### Modul AUTO
In modul **AUTO**, sistemul:
1. citeste valorile celor doi senzori LDR
2. calculeaza diferenta dintre ele
3. daca diferenta depaseste un prag, muta servo-ul spre partea mai luminata
4. daca diferenta este mica, servo-ul ramane in pozitia curenta
