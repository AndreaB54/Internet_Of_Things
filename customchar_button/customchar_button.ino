#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16x2 display

// Inisialisasi button
const int butdown = 2; // D4
const int butup = 0;   // D3

int count = 0;

byte orang[8] = {
  0b01110,
  0b01110,
  0b01110,
  0b00100,
  0b11111,
  0b00100,
  0b01110,
  0b10001
};

void setup() {
  lcd.init();   
  lcd.backlight();
  
  pinMode(butup, INPUT_PULLUP);
  pinMode(butdown, INPUT_PULLUP);

  lcd.createChar(0, orang); 

  lcd.setCursor(0, 0);
  lcd.print("Count: 0");
  lcd.setCursor(0, 1);
}

void loop() {
  buttonUp();  
  buttonDown(); 
}

void buttonUp() {
  if (digitalRead(butup) == LOW) {
    if (count < 16) {
      count++;
      updateDisplay();
    }
    delay(200); 
  }
}

void buttonDown() {
  if (digitalRead(butdown) == LOW) {
    if (count > 0) {
      count--;
      updateDisplay();
    }
    delay(200); 
  }
}

void updateDisplay() {
  lcd.setCursor(7, 0);
  lcd.print("   "); 
  lcd.setCursor(7, 0);
  lcd.print(count); 

  showCharacters(); 
}

void showCharacters() {
  lcd.setCursor(0, 1);
  lcd.print("                "); 
  lcd.setCursor(0, 1);
  
  for (int i = 0; i < count; i++) {
    lcd.write(byte(0)); // Cetak karakter "orang"
  }
}
