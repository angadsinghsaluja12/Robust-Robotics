// button_sender.ino
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASS";
const char* mqtt_server = "BROKER_IP_OR_HOST";
const int mqtt_port = 1883;

const int buttonPin = 0; // change to your button pin (GPIO)
const int debounceMs = 50;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastDebounce = 0;
int lastButtonState = HIGH;
int buttonState;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to "); Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("WiFi connected");
}

void callback(char* topic, byte* payload, unsigned int length) {
  // not used for sender
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_Button")) { 
      // connected
    } else {
      delay(2000);
    }
  }
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounce = millis();
  }
  if ((millis() - lastDebounce) > debounceMs) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) { // pressed (assuming pullup)
        Serial.println("Button pressed -> publish launch");
        client.publish("drone/launch", "MANUAL");
      }
    }
  }
  lastButtonState = reading;
}
