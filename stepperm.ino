#include <Stepper.h>
#include "pitches.h"
#include <LiquidCrystal.h>

int melody[] = {
   NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
int duration = 1000;  // 500 miliseconds
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int count =0;

// ---- Import des pins StepMotor ----

const int nbPas = 2048; //  effectuer un tour complet avec le StepMotor
const int Min1Pin = 3;  
const int Min2Pin = 5;
const int Min3Pin = 6;
const int Min4Pin = 4;

// ---- Import des pins pour mesurer la tension ----

const int CanalogPin1 = A0; // broche A0
const int CanalogPin2 = A1;

// ---- Booléens d'éxécution de boucle ----

bool loopExecuted = false; // variable pour suivre si la boucle a été exécutée
bool shouldRun = true; // variable pour contrôler l'exécution de la boucle principale

// ---- Création d'un StepMotor ----

Stepper myStepper(nbPas, Min1Pin, Min2Pin, Min3Pin, Min4Pin);

// ---- Void Setup ----

void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(9);
  lcd.begin(16, 2);
  lcd.print("Le dechetecteur 62");
}

// ---- Void Loop ----

void loop() {
  if (!shouldRun) {
    return; // sortir de la boucle principale
  }

  // Lire la valeur de tension sur la broche A0
  int CanalogValue1 = analogRead(CanalogPin1);
  int CanalogValue2 = analogRead(CanalogPin2);
  int Value = CanalogValue1 - CanalogValue2;

  // Vérifier si la tension est supérieure ou égale à 2V
  if (Value < 150 && !loopExecuted) { // 2V correspond à environ 410 (sur une plage de 0-1023)
    // Faire tourner le moteur dans un sens pour 180 degrés
    tone(13, melody[1], duration);
    lcd.setCursor(0, 1);
    lcd.print(count+=1);
    myStepper.step(nbPas / 2);
    delay(1000);
    

    // Faire tourner le moteur dans le sens opposé pour 180 degrés
    myStepper.step(-nbPas / 2);
    delay(1000);

    loopExecuted = true; // marquer que la boucle a été exécutée une fois
  }

  // Vérifier si la tension est à nouveau supérieure ou égale à 2V et que la boucle a déjà été exécutée
  if (Value < 150 && loopExecuted) {
    loopExecuted = false; // remettre la variable à "faux" pour que la boucle puisse être réexécutée
  }

  // Si la boucle a été exécutée, désactiver la boucle continue
  if (loopExecuted) {
    shouldRun = false; // désactiver la boucle principale
  }
}
