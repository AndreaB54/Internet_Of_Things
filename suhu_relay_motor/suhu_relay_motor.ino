// AUTHENTICATION
#define BLYNK_TEMPLATE_ID "TMPL2ycUUkgNt"
#define BLYNK_TEMPLATE_NAME "Blynk Praktikum"
#define BLYNK_AUTH_TOKEN "_gl-cKSBPPF6IMgv6eN2-deDfzUTyRcP"

// LIBRARIES
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// WIFI CREDENTIALS
char ssid[] = "";
char pass[] = "";

// PIN CONFIGURATION
const int dhtPin = 2;      // GPIO2 (D4)
const int lampNormal = 15; // GPIO15 (D8)
const int lampHot = 5;     // GPIO5 (D1)
const int lampCold = 16;   // GPIO16 (D0)
const int relay = 4;       // GPIO4 (D2) -> Mengontrol motor DC via relay

DHT dht(dhtPin, DHT11);
bool manualMotor = false; // Mode manual motor (OFF default)
bool autoMotor = false;   // Status motor otomatis

// BLYNK SWITCH UNTUK MOTOR (V3)
BLYNK_WRITE(V3) {
  manualMotor = param.asInt();  // Jika 1, mode manual aktif
  Serial.print("Manual Motor: "); Serial.println(manualMotor ? "ON" : "OFF");

  if (manualMotor) {
    digitalWrite(relay, LOW);  // Motor ON dalam mode manual
  } else if (!autoMotor) {
    digitalWrite(relay, HIGH); // Motor OFF jika otomatis juga mati
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();

  pinMode(lampNormal, OUTPUT);
  pinMode(lampHot, OUTPUT);
  pinMode(lampCold, OUTPUT);
  pinMode(relay, OUTPUT);

  digitalWrite(lampNormal, LOW);
  digitalWrite(lampHot, LOW);
  digitalWrite(lampCold, LOW);
  digitalWrite(relay, HIGH);  // Default motor mati
}

void loop() {
  Blynk.run();
  sensor_read();
  delay(1000);
}

void sensor_read() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print("Humidity: "); Serial.print(h);
  Serial.print("%  Temperature: "); Serial.print(t); Serial.println("°C");

  // Kirim data ke Blynk
  Blynk.virtualWrite(V0, t);  // Suhu
  Blynk.virtualWrite(V1, h);  // Kelembaban

  // Logika kontrol suhu
  control_temperature(t);

  // Update Gauge V2 (Campuran)
  String motorStatus = (digitalRead(relay) == LOW) ? "ON" : "OFF";
  String gaugeText = "Temp: " + String(t) + "°C | Hum: " + String(h) + "% | Motor: " + motorStatus;
  Blynk.virtualWrite(V2, gaugeText);
}

void control_temperature(float t) {
  if (t > 36) {  // PANAS -> Motor ON (otomatis)
    digitalWrite(lampNormal, LOW);
    digitalWrite(lampHot, HIGH);
    digitalWrite(lampCold, LOW);
    Serial.println("Status: PANAS - Motor ON");

    Blynk.virtualWrite(V4, 255); // LED Panas ON
    Blynk.virtualWrite(V5, 0);   // LED Dingin OFF
    Blynk.virtualWrite(V6, 0);   // LED Normal OFF

    autoMotor = true;
    Blynk.virtualWrite(V3, 1); // Set switch motor di Blynk ke ON

    if (!manualMotor) {
      digitalWrite(relay, LOW);  // Motor menyala otomatis
    }
  }
  else if (t < 27) {  // DINGIN -> Motor OFF
    digitalWrite(lampNormal, LOW);
    digitalWrite(lampHot, LOW);
    digitalWrite(lampCold, HIGH);
    Serial.println("Status: DINGIN - Motor OFF");

    Blynk.virtualWrite(V4, 0);   // LED Panas OFF
    Blynk.virtualWrite(V5, 255); // LED Dingin ON
    Blynk.virtualWrite(V6, 0);   // LED Normal OFF

    autoMotor = false;
    Blynk.virtualWrite(V3, 0); // Set switch motor di Blynk ke OFF

    if (!manualMotor) {
      digitalWrite(relay, HIGH);  // Motor mati
    }
  }
  else {  // NORMAL (27 - 35) -> Motor OFF
    digitalWrite(lampNormal, HIGH);
    digitalWrite(lampHot, LOW);
    digitalWrite(lampCold, LOW);
    Serial.println("Status: NORMAL - Motor OFF");

    Blynk.virtualWrite(V4, 0);   // LED Panas OFF
    Blynk.virtualWrite(V5, 0);   // LED Dingin OFF
    Blynk.virtualWrite(V6, 255); // LED Normal ON

    autoMotor = false;
    Blynk.virtualWrite(V3, 0); // Set switch motor di Blynk ke OFF

    if (!manualMotor) {
      digitalWrite(relay, HIGH);  // Motor mati
    }
  }
}
