#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>

const int ledPin = 4;

// --- Configuration Wi-Fi ---
const char* ssid = "iPhone de Pierre";
const char* password = "feur1234567";

// --- Configuration MQTT ---
const char* mqtt_server = "172.20.10.7"; // IP de ton RPI3-Pierre
const char* topic = "portail/logs";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  Serial.print("Connexion au Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connecté !");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentative de connexion MQTT...");
    // On crée un ID client unique (ex: ESP32-CAM-Pierre)
    if (client.connect("ESP32-CAM-Pierre")) {
      Serial.println("Connecté au Broker !");
    } else {
      Serial.print("Échec, rc=");
      Serial.print(client.state());
      Serial.println(" Nouvelle tentative dans 5s");
      delay(5000);
    }
  }
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Désactive brownout
  Serial.begin(115200);
  delay(1000);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Envoyer un message une seule fois pour le test
    client.publish(topic, "Hello RPI3 - Test de l'ESP32-CAM");
    Serial.println("Message envoyé au RPI3 !");
    delay(5000);
}