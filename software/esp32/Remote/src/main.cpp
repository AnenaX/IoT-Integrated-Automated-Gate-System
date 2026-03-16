#include <Arduino.h>
#include <IRremote.hpp>  // Librairie IRremote pour ESP32

// --- PINS ---
const int buttonPin = 18;
const int ledPin    = 19;  // LED de statut
const int IR_PIN    = 4;   // Émetteur IR

// --- CODES IR (protocole NEC) ---
const uint32_t CODE_OUVRIR = 0x00FF30CF;
const uint32_t CODE_FERMER = 0x00FF18E7;
const uint32_t CODE_STOP   = 0x00FF7A85;

// --- ÉTAT ---
bool portailOuvert = false;
bool dernierBouton = LOW;
unsigned long dernierAppui = 0;
const int DEBOUNCE = 300;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  IrSender.begin(IR_PIN);  // Init émetteur IR
  Serial.println("[READY] Télécommande prête.");
}

void envoyerCode(uint32_t code, const char* label) {
  IrSender.sendNEC(code, 32);
  Serial.print("[IR] Envoyé : ");
  Serial.println(label);
}

void loop() {
  bool etatBouton = digitalRead(buttonPin);

  // Détection front montant + anti-rebond
  if (etatBouton == HIGH && dernierBouton == LOW &&
      (millis() - dernierAppui) > DEBOUNCE) {

    dernierAppui = millis();

    if (!portailOuvert) {
      envoyerCode(CODE_OUVRIR, "OUVRIR");
      digitalWrite(ledPin, HIGH);
      portailOuvert = true;
      Serial.println("[PORTAIL] Commande ouverture envoyée.");
    } else {
      envoyerCode(CODE_FERMER, "FERMER");
      digitalWrite(ledPin, LOW);
      portailOuvert = false;
      Serial.println("[PORTAIL] Commande fermeture envoyée.");
    }
  }

  // Bouton maintenu > 2s = STOP
  if (etatBouton == HIGH && (millis() - dernierAppui) > 2000) {
    envoyerCode(CODE_STOP, "STOP");
    Serial.println("[PORTAIL] Commande STOP envoyée !");
    delay(500);
  }

  dernierBouton = etatBouton;
  delay(10);
}