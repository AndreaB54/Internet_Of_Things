#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 14  // Pin D5 untuk sensor DHT11
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Alamat LCD 0x27, 16 karakter, 2 baris

int hot = 16;    // D0 (LED merah)
int normal = 0;  // D3 (LED kuning)
int cold = 2;    // D4 (LED biru)

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(hot, OUTPUT);
  pinMode(normal, OUTPUT);
  pinMode(cold, OUTPUT);
}

void loop() {
  float suhu = dht.readTemperature(); // Baca suhu dalam Celsius

  if (isnan(suhu)) {
    Serial.println("Gagal membaca DHT11!");
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.println(" °C");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Suhu: ");
  lcd.print(suhu);
  lcd.print(" C");

  // Menentukan kondisi suhu
  if (suhu >= 28) {
    lcd.setCursor(0, 1);
    lcd.print("Status: Panas!");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Led1: 1");
    lcd.setCursor(0, 1);
    lcd.print("Led2: 0");
    lcd.setCursor(9, 1);
    lcd.print("Led3: 0");
    digitalWrite(hot, HIGH);
    digitalWrite(normal, LOW);
    digitalWrite(cold, LOW);
  } else if (suhu >= 20) {
    lcd.setCursor(0, 1);
    lcd.print("Status: Normal");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Led1: 0");
    lcd.setCursor(0, 1);
    lcd.print("Led2: 1");
    lcd.setCursor(9, 1);
    lcd.print("Led3: 0");
    digitalWrite(hot, LOW);
    digitalWrite(normal, HIGH);
    digitalWrite(cold, LOW);
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Status: Dingin");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Led1: 0");
    lcd.setCursor(0, 1);
    lcd.print("Led2: 0");
    lcd.setCursor(9, 1);
    lcd.print("Led3: 1");
    digitalWrite(hot, LOW);
    digitalWrite(normal, LOW);
    digitalWrite(cold, HIGH);
  }

  delay(500); // Tunggu 2 detik sebelum membaca ulang
}
