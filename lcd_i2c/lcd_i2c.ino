#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//initialize the liquid crystal library
//the first parameter is  the I2C address
//the second parameter is how many rows are on your screen
//the  third parameter is how many columns are on your screen
LiquidCrystal_I2C lcd(0x27, 20, 4);

int delayLCD = 500;

void setup() {

  //initialize lcd screen
  lcd.begin();
  // turn on the backlight
  lcd.backlight();
}

void loop() {
  //wait  for a second
  delay(delayLCD);
  lcd.clear();

  // tell the screen to write on the top row
  lcd.setCursor(0, 0);
  // tell the screen to write “hello, from” on the top  row
  lcd.print("12345678901234567890");
  delay(delayLCD);
  lcd.clear();

  // tell the screen to write on the bottom  row
  lcd.setCursor(0, 1);
  // tell the screen to write “Arduino_uno_guy”  on the bottom row
  // you can change whats in the quotes to be what you want  it to be!
  lcd.print("Hello, From");
  delay(delayLCD);
  lcd.clear();

  // tell the screen to write on the top row
  lcd.setCursor(0, 2);
  // tell the screen to write “hello, from” on the top  row
  lcd.print("Hello, From");
  delay(delayLCD);
  lcd.clear();

  // tell the screen to write on the bottom  row
  lcd.setCursor(0, 3);
  // tell the screen to write “Arduino_uno_guy”  on the bottom row
  // you can change whats in the quotes to be what you want  it to be!
  lcd.print("Hello, From");
}
