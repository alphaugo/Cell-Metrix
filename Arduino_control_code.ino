#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// === Pin Definitions ===
#define ONE_WIRE_BUS 8
#define FAN_PIN 9
#define BUZZER_PIN 10
#define RS 7
#define EN 6
#define D4 5
#define D5 4
#define D6 3
#define D7 2

#define ESP_RX 12  // Nano receives from ESP TX (D7)
#define ESP_TX 11  // Nano sends to ESP RX (D8, optional)

SoftwareSerial espSerial(ESP_RX, ESP_TX);  // RX, TX

// === Thresholds ===
const float TEMP_THRESHOLD = 40.0;
const float VOLTAGE_THRESHOLD = 3.0;  // For testing with 9V battery

// === Global Variables ===
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

float voltage = 0.0;
bool isCharging = false;

unsigned long lastBuzzerEvent = 0;
int buzzerStage = 0;

void setup() {
  Serial.begin(9600);      // For debug
  espSerial.begin(9600);   // Communicate with ESP

  pinMode(FAN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  sensors.begin();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("System Starting...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // === Read Data from ESP8266 ===
  if (espSerial.available()) {
    String data = espSerial.readStringUntil('\n');
    data.trim();

    int sepIndex = data.indexOf(':');
    if (sepIndex != -1) {
      voltage = data.substring(0, sepIndex).toFloat();
      String status = data.substring(sepIndex + 1);
      isCharging = (status == "charging");
    }
  }

  // === Temperature ===
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  // === Fan Control ===
  digitalWrite(FAN_PIN, tempC > TEMP_THRESHOLD ? HIGH : LOW);

  // === Buzzer Cycle Control ===
  unsigned long now = millis();
  if (voltage < VOLTAGE_THRESHOLD && !isCharging) {
    switch (buzzerStage) {
      case 0:
      case 2:
      case 4:
        if (now - lastBuzzerEvent >= 1000) {
          digitalWrite(BUZZER_PIN, HIGH);
          lastBuzzerEvent = now;
          buzzerStage++;
        }
        break;
      case 1:
      case 3:
      case 5:
        if (now - lastBuzzerEvent >= 500) {
          digitalWrite(BUZZER_PIN, LOW);
          lastBuzzerEvent = now;
          buzzerStage++;
        }
        break;
      case 6:
        if (now - lastBuzzerEvent >= 20000) {
          buzzerStage = 0;
        }
        break;
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    buzzerStage = 0;
  }

  // === LCD Display ===
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(voltage, 1);
  lcd.print("V   ");

  lcd.setCursor(9, 0);
  lcd.print("T:");
  lcd.print(tempC, 1);
  lcd.print("C ");

  lcd.setCursor(0, 1);
  lcd.print("Status:");
  lcd.print(isCharging ? "Charging   " : "NotCharging");

  delay(200);
}

