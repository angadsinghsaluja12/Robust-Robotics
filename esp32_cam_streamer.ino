// valve_receiver.ino
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASS";
const char* mqtt_server = "BROKER_IP_OR_HOST";
const int mqtt_port = 1883;

const int valvePin = 26; // GPIO to drive solenoid MOSFET/relay (change as needed)
const int ledPin = 2;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastTrigger = 0;
const unsigned long triggerCooldownMs = 5000; // 5s cooldown

void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  Serial.print("Received ["); Serial.print(topic); Serial.print("] ");
  Serial.println(msg);

  if (String(topic) == "drone/launch") {
    if (millis() - lastTrigger > triggerCooldownMs) {
      triggerValve();
      lastTrigger = millis();
    } else {
      Serial.println("Trigger blocked by cooldown");
    }
  }
}

void triggerValve() {
  Serial.println("Triggering valve...");
  digitalWrite(valvePin, HIGH); // ON
  digitalWrite(ledPin, HIGH);
  delay(300); // pulse width, tune for pneumatic launch
  digitalWrite(valvePin, LOW);
  digitalWrite(ledPin, LOW);
  Serial.println("Valve pulse done");
}

void setup_wifi() {
  Serial.println();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("WiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_Valve")) {
      client.subscribe("drone/launch");
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(valvePin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(valvePin, LOW);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}
