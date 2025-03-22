#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WIFI CREDENTIALS
const char* ssid = "";
const char* pass = "";

// MQTT Broker
const char* mqtt_broker = "broker.emqx.io";
const char* topic_dht = "andrea/DHT";
const char* topic_ping = "andrea/PING";
const char* mqtt_username = "";
const char* mqtt_password = "";
const int mqtt_port = 1883;

// Konfigurasi pin
#define DHTTYPE DHT11
const int dhtPin = 2; // D4
const int trigPin = 5; // D1
const int echoPin = 16; // D0
const int hotlamp = 15; // D8
const int normallamp = 13; // D7
const int coldlamp = 12; // D6
const int buttonPin = 4; // D2

DHT dht(dhtPin, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);
bool sendDHT = true; // True = publish suhu, False = publish jarak

void connectWiFi() {
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

void connectMQTT() {
  client.setServer(mqtt_broker, mqtt_port);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266", mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT Broker!");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5s...");
      delay(5000);
    }
  }
}

float readDHT() {
  float suhu = dht.readTemperature();
  return suhu;
}

float readPing() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}

void updateLEDs(float value, bool isTemperature) {
  digitalWrite(hotlamp, LOW);
  digitalWrite(normallamp, LOW);
  digitalWrite(coldlamp, LOW);

  if (isTemperature) {
    if (value < 20) {
      digitalWrite(coldlamp, HIGH);
    } else if (value >= 20 && value < 28) {
      digitalWrite(normallamp, HIGH);
    } else {
      digitalWrite(hotlamp, HIGH);
    }
  } else {
    if (value < 10) {
      digitalWrite(coldlamp, HIGH);
    } else if (value >= 10 && value < 19) {
      digitalWrite(normallamp, HIGH);
    } else {
      digitalWrite(hotlamp, HIGH);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(hotlamp, OUTPUT);
  pinMode(normallamp, OUTPUT);
  pinMode(coldlamp, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  dht.begin();
  connectWiFi();
  connectMQTT();
}

void loop() {
  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();

  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(buttonPin);

  if (lastButtonState == HIGH && currentButtonState == LOW) { // Tombol ditekan
    sendDHT = !sendDHT;
  }
  lastButtonState = currentButtonState;

  if (sendDHT) {
    float suhu = readDHT();
    String dht = "Suhu: " + String(suhu, 2); 
    client.publish(topic_dht, dht.c_str());
    Serial.println(dht);
  } else {
    float jarak = readPing();
    String ping = "Jarak: " + String(jarak, 2); 
    client.publish(topic_ping, ping.c_str());
    Serial.println(ping);
  }

  delay(1000);
}
