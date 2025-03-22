#include <DHT.h>
#include "ping.h"

const int dht_pin = 4;
DHT dht(dht_pin, DHT11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  float suhu_c = dht.readTemperature();
  float suhu_f = dht.readTemperature(true);
  float kelembapan = dht.readHumidity();
  Serial.println(String("Suhu C: ") + String(suhu_c));
  Serial.println(String("Suhu F: ") + String(suhu_f));
  Serial.println(String("Humid: ") + String(kelembapan));

  delay(200);
}
