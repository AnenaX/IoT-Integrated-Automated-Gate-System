#include <Arduino.h>
#include <IRremote.hpp>

// --- Configuration ---
const int buttonPin = 18; // Bouton poussoir
const int IR_PIN    = 4;  // LED IR
const int statusLED = 19; // LED de statut

// --- Codes NEC Identifiés ---
const uint32_t CODE_TOGGLE = 0x00FFA25D;
const uint32_t CODE_STOP   = 0x00FF629D;
const uint32_t CODE_PIETON = 0x00FFE21D;

bool dernierBouton = HIGH;
unsigned long dernierAppui = 0;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(statusLED, OUTPUT);
  IrSender.begin(IR_PIN);
  Serial.println("[READY] Telecommande prete");
}

void loop() {
  bool etat = digitalRead(buttonPin);
  // Détection front descendant + anti-rebond 300ms
  if (etat == LOW && dernierBouton == HIGH && (millis() - dernierAppui) > 300) {
    dernierAppui = millis();
    
    digitalWrite(statusLED, HIGH);
    IrSender.sendNEC(CODE_TOGGLE, 32);
    Serial.println("[IR] Commande Toggle envoyee");
    delay(100);
    digitalWrite(statusLED, LOW);
  }
  dernierBouton = etat;
  delay(10);
}
