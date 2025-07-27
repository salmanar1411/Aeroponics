#include <SoftwareSerial.h>
#include <ModbusMaster.h>

// RS485 Serial
#define RX_PIN 16
#define TX_PIN 17
#define DE_RE_PIN 4  // untuk kontrol arah RS485

// Buat SoftwareSerial
SoftwareSerial rs485Serial(RX_PIN, TX_PIN); 

// Buat objek Modbus
ModbusMaster node;

// Fungsi untuk atur DE/RE pin
void preTransmission()  { digitalWrite(DE_RE_PIN, HIGH); }
void postTransmission() { digitalWrite(DE_RE_PIN, LOW); }

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

  Serial.println("Modbus SHT20-MODBUS Reader Ready");
}

void loop() {
  uint8_t result = node.readInputRegisters(0x0001, 2); // Read 2 registers

  if (result == node.ku8MBSuccess) {
    uint16_t rawTemp = node.getResponseBuffer(0);
    uint16_t rawHum  = node.getResponseBuffer(1);

    float temperature = rawTemp / 10.0;
    float humidity    = rawHum / 10.0;

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    Serial.print("Modbus read error: ");
    Serial.println(result);
  }

  delay(2000);
}
