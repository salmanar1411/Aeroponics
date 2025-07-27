#include <SoftwareSerial.h>
#include <ModbusMaster.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// RS485 Serial
#define RX_PIN 16
#define TX_PIN 17
#define DE_RE_PIN 4  // untuk kontrol arah RS485

const int relay = 32;

// Buat SoftwareSerial
SoftwareSerial rs485Serial(RX_PIN, TX_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Buat objek Modbus
ModbusMaster node;

float temperature = 0;
float humidity = 0;
bool relayON = 0;
bool relayOFF = 1;
bool relayStatus = false;
float batasBawah = 78.5;
float batasAtas = 90;

// Fungsi untuk atur DE/RE pin
void preTransmission() {
  digitalWrite(DE_RE_PIN, HIGH);
}
void postTransmission() {
  digitalWrite(DE_RE_PIN, LOW);
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Setup DE/RE
  pinMode(DE_RE_PIN, OUTPUT);
  digitalWrite(DE_RE_PIN, LOW);

  // Setup RS485 serial
  rs485Serial.begin(9600);  // Baudrate RS485

  // Setup Modbus dengan slave ID 1
  node.begin(1, rs485Serial);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  lcd.begin();
  // turn on the backlight
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Modbus SHT20-MODBUS Reader Ready");
  Serial.println("Modbus SHT20-MODBUS Reader Ready");

  pinMode(relay, OUTPUT);

  digitalWrite(relay, relayOFF);
}

void loop() {
  uint8_t result = node.readInputRegisters(0x0001, 2);  // Read 2 registers

  delay(2000);
  lcd.clear();

  if (result == node.ku8MBSuccess) {
    uint16_t rawTemp = node.getResponseBuffer(0);
    uint16_t rawHum = node.getResponseBuffer(1);

    temperature = rawTemp / 10.0;
    humidity = rawHum / 10.0;
    // humidity = ;

    if (humidity >= batasBawah && humidity <= batasAtas) {
      digitalWrite(relay, relayON);
      relayStatus = true;
    } else {
      digitalWrite(relay, relayOFF);
      relayStatus = false;
    }

  } else {
    Serial.print("Modbus read error: ");
    Serial.println(result);

    // lcd.setCursor(0, 0);
    // lcd.print(result);
  }

  display(temperature, humidity, relayStatus);
}

void display(float temperature, float humidity, bool relayStatus) {
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.setCursor(14, 0);
  lcd.print(temperature);
  lcd.setCursor(18, 0);
  lcd.print(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  lcd.setCursor(0, 1);
  lcd.print("Humidity   :");
  lcd.setCursor(14, 1);
  lcd.print(humidity);
  lcd.setCursor(18, 1);
  lcd.print(" %");

  if (relayStatus) {
    Serial.println("Kipas : ON, Mistmaker : ON");

    lcd.setCursor(0, 2);
    lcd.print("Kipas: ON");

    lcd.setCursor(0, 3);
    lcd.print("Mistmaker: ON");
  } else {
    Serial.println("Kipas : OFF, Mistmaker : OFF");

    lcd.setCursor(0, 2);
    lcd.print("Kipas: OFF");

    lcd.setCursor(0, 3);
    lcd.print("Mistmaker: OFF");
  }
}