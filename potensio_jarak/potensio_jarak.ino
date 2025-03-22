#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Alamat LCD 0x27, 16 karakter, 2 baris

const char* ssid = "";
const char* pass = "";
const char* mqtt_broker = "broker.emqx.io";
const char* topic_kondisi = "nim/kondisi";
const char* mqtt_username = "";
const char* mqtt_password = "";
const int mqtt_port = 1883;

int kondisi1 = 0;  // D3
int kondisi2 = 2;  // D4
const int trigPin = 15; 
const int echoPin = 13;
int potensio = A0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");
}

void reconnect_mqtt() {
  while (!client.connected()) {
    Serial.print("Reconnecting to MQTT...");
    String clientID = "ESP8266-" + String(ESP.getChipId());
    if (client.connect(clientID.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT!");
    } else {
      Serial.print("Failed, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(kondisi1, OUTPUT);
  pinMode(kondisi2, OUTPUT);

  lcd.init();
  lcd.backlight();
  
  setup_wifi();
  client.setServer(mqtt_broker, mqtt_port);
  reconnect_mqtt();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, reconnecting...");
    setup_wifi();
  }

  if (!client.connected()) {
    reconnect_mqtt();
  }
  
  client.loop();

  long duration;
  float distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; 

  String kondisi;
  if (distance < 8) kondisi = "Dekat";
  else if (distance < 15) kondisi = "Sedang";
  else if (distance < 30) kondisi = "Jauh";
  else kondisi = "Sangat jauh";

  int potValue = analogRead(potensio);
  float voltage = potValue * (5.0 / 1023.0);
  int pwmValue = map(potValue, 0, 1023, 0, 255);

  analogWrite(kondisi1, pwmValue);
  analogWrite(kondisi2, 255 - pwmValue);

  delay(1000);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("V: " + String(voltage, 2) + " V ");
  lcd.setCursor(3, 1);
  lcd.print("Int : " + String(pwmValue));
  delay(1000);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("D: " + String(distance, 1) + " cm ");
  lcd.setCursor(3, 1);
  lcd.print(kondisi);

  if (client.connected()) {
    String kirimpesan = "Kondisi: " + kondisi + " | Jarak: " + String(distance, 1) + "cm";
    client.publish(topic_kondisi, kirimpesan.c_str());
    Serial.println(kirimpesan);
  }

  delay(500);
}
