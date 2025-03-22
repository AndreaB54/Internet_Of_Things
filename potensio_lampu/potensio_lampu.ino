int potensio = A0 ;//ADC
int out_analog = 0 ;//D3

void setup() {
  // put your setup code here, to run once:
  pinMode(potensio, INPUT);
  pinMode(out_analog, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float nilai = analogRead(potensio);
  Serial.println(nilai);
  int persamaan = nilai/(1024/255);
  Serial.println(persamaan);
  analogWrite(out_analog, persamaan);
  delay(199);
  float tegangan = (nilai*5)/1024;
}
