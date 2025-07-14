#include <SoftwareSerial.h>

#define VOLTAGE_PIN A0         // Reads battery voltage
#define CHARGE_CTRL_PIN D5     // Controls P-MOSFET gate
#define TX_PIN D7              // To Arduino RX (D12)
#define RX_PIN D8              // Not used here (can be left unconnected)

// ESP8266 built-in divider maps 0–3.3V battery to 0–1V on A0
const float SCALE_FACTOR = (12.0 / 1024.0) / 0.1667;  // ≈ 0.234
const float CUTOFF_THRESHOLD = 40.0;  // Updated threshold

SoftwareSerial uart(RX_PIN, TX_PIN);  // RX, TX (ESP to Arduino)

void setup() {
  pinMode(CHARGE_CTRL_PIN, OUTPUT);
  digitalWrite(CHARGE_CTRL_PIN, HIGH);  // Stop charging by default

  uart.begin(9600);
  Serial.begin(9600);  // Optional for debugging
}

void loop() {
  int raw = analogRead(VOLTAGE_PIN);
  float voltage = raw * SCALE_FACTOR;

  String status;
  if (voltage >= CUTOFF_THRESHOLD) {
    digitalWrite(CHARGE_CTRL_PIN, HIGH);  // Stop charging
    status = "not_charging";
  } else {
    digitalWrite(CHARGE_CTRL_PIN, LOW);   // Charging allowed
    status = "charging";
  }

  String message = String(voltage, 2) + ":" + status;
  uart.println(message);
  Serial.println(message);  // Debug output

  delay(1000);
}
