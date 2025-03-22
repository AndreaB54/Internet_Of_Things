#include <DHT.h>

// Pin konfigurasi
const int trigPin = 5;
const int echoPin = 16;
const int dhtPin = 2;
const int ledPin = 15;
const int buttonPin = 4; 

// DHT11 setup
#define DHTTYPE DHT11
DHT dht(dhtPin, DHTTYPE);

// Variabel sensor ultrasonik
long duration;
int distance;

// Variabel button dan mode
bool modeDHT = true; // true = DHT11, false = Ping Sensor
bool lastButtonState = HIGH;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); 
  Serial.begin(9600);
  dht.begin();
}


void loop() {
  // Cek tombol untuk switch mode
  bool buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && lastButtonState == HIGH) {
    modeDHT = !modeDHT; // Toggle mode
    // Serial.print("Mode: ");
    // Serial.println(modeDHT ? "DHT11" : "Ping Sensor");
    // delay(300); // Debounce
  }
  lastButtonState = buttonState;

  if (modeDHT) {
    // Mode DHT11
    float suhu = dht.readTemperature();
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.println("°C");

    if (suhu > 35) {
      kedipLED();
    } else {
      digitalWrite(ledPin, LOW);
    }
  } else {
    // Mode Ping Sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;

    Serial.print("Jarak: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance < 10) {
      kedipLED();
    } else {
      digitalWrite(ledPin, LOW);
    }
  }

  delay(200);
}

void kedipLED() {
  digitalWrite(ledPin, HIGH);
  delay(250);
  digitalWrite(ledPin, LOW);
  delay(250);
}
